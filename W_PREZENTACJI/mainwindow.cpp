#include "mainwindow.h"
#include "ParametryARX.h"
#include "ui_mainwindow.h"
#include "W_DANYCH/GeneratorWartosciZadanej.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_timerSymulacji(new QTimer(this))
{
    ui->setupUi(this);

    // 1. Layout i Marginesy
    ui->verticalLayout_Plot1->setContentsMargins(0, 5, 0, 0);
    ui->verticalLayout_Plot2->setContentsMargins(0, 5, 0, 0);
    ui->verticalLayout_Plot3->setContentsMargins(0, 5, 0, 0);
    ui->verticalLayout_Plot4->setContentsMargins(0, 5, 0, 0);

    // ==========================================
    // KONFIGURACJA WYKRESÓW
    // ==========================================

    // --- Wykres 1: Zadana i Wyjście ---
    m_seriesZadana = new QLineSeries();
    m_seriesZadana->setName("Zadana (w)");
    QPen penRed(Qt::red); penRed.setWidth(2);
    m_seriesZadana->setPen(penRed);

    m_seriesWyjscie = new QLineSeries();
    m_seriesWyjscie->setName("Wyjście (y)");
    QPen penBlue(QColor(0, 150, 255)); penBlue.setWidth(2);
    m_seriesWyjscie->setPen(penBlue);

    m_chartOutput = new QChart();
    m_chartOutput->addSeries(m_seriesZadana);
    m_chartOutput->addSeries(m_seriesWyjscie);
    m_chartOutput->createDefaultAxes();

    // Stylizacja z legendą
    stylizujWykres(m_chartOutput, true);

    m_chartViewOutput = new QChartView(m_chartOutput);
    m_chartViewOutput->setRenderHint(QPainter::Antialiasing);
    ui->verticalLayout_Plot1->addWidget(m_chartViewOutput);

    // --- Wykres 2: Uchyb ---
    m_seriesUchyb = new QLineSeries();
    m_seriesUchyb->setName("Uchyb (e)");
    QPen penGreen(Qt::green); penGreen.setWidth(2);
    m_seriesUchyb->setPen(penGreen);

    setupChart(ui->verticalLayout_Plot2, m_chartError, m_chartViewError);
    m_chartError->addSeries(m_seriesUchyb);
    m_chartError->createDefaultAxes();
    stylizujWykres(m_chartError, false); // Bez legendy

    // --- Wykres 3: Sterowanie ---
    m_seriesSterowanie = new QLineSeries();
    m_seriesSterowanie->setName("Sterowanie (u)");
    QPen penMagenta(Qt::magenta); penMagenta.setWidth(2);
    m_seriesSterowanie->setPen(penMagenta);

    setupChart(ui->verticalLayout_Plot3, m_chartControl, m_chartViewControl);
    m_chartControl->addSeries(m_seriesSterowanie);
    m_chartControl->createDefaultAxes();
    stylizujWykres(m_chartControl, false); // Bez legendy

    // --- Wykres 4: Składowe PID ---
    m_seriesP = new QLineSeries(); m_seriesP->setName("P"); m_seriesP->setColor(Qt::cyan);
    m_seriesI = new QLineSeries(); m_seriesI->setName("I"); m_seriesI->setColor(Qt::yellow);
    m_seriesD = new QLineSeries(); m_seriesD->setName("D"); m_seriesD->setColor(QColor(255, 100, 255));

    setupChart(ui->verticalLayout_Plot4, m_chartPID, m_chartViewPID);
    m_chartPID->addSeries(m_seriesP);
    m_chartPID->addSeries(m_seriesI);
    m_chartPID->addSeries(m_seriesD);
    m_chartPID->createDefaultAxes();

    // Stylizacja z legendą (PID ma 3 linie, więc legenda się przyda)
    stylizujWykres(m_chartPID, true);

    // ==========================================
    // LOGIKA I SYGNAŁY
    // ==========================================
    m_timerSymulacji->setInterval(ui->spinInterwal->value());
    connect(m_timerSymulacji, &QTimer::timeout, this, &MainWindow::aktualizujSymulacje);

    connect(ui->pushStart, &QPushButton::clicked, this, &MainWindow::on_pushStart_clicked);
    connect(ui->pushStop, &QPushButton::clicked, this, &MainWindow::on_pushStop_clicked);
    connect(ui->pushResetSym, &QPushButton::clicked, this, &MainWindow::on_pushResetSym_clicked);

    // Generator
    connect(ui->comboTypSygnalu, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::aktualizujParametryGeneratora);

    // POPRAWIONE LINIE: Zamiast pushZapisz jest QDoubleSpinBox
    connect(ui->spinAmplituda, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::aktualizujParametryGeneratora);

    connect(ui->spinOkres, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::aktualizujParametryGeneratora);

    connect(ui->spinSkladowaStala, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::aktualizujParametryGeneratora);

    connect(ui->spinWypelnienie, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::aktualizujParametryGeneratora);

    connect(ui->spinInterwal, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int val){
        m_timerSymulacji->setInterval(val);
        aktualizujParametryGeneratora();
    });

    // PID
    connect(ui->spinPidKp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::aktualizujParametryPID);

    connect(ui->spinPidTi, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::aktualizujParametryPID);

    connect(ui->spinPidTd, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::aktualizujParametryPID);
    aktualizujParametryGeneratora();
    aktualizujParametryPID();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ==========================================
// METODY POMOCNICZE
// ==========================================

void MainWindow::setupChart(QLayout* layout, QChart*& chart, QChartView*& view) {
    chart = new QChart();
    view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(view);
}

void MainWindow::stylizujWykres(QChart* chart, bool pokazLegende) {
    // 1. Ciemne tło
    chart->setBackgroundBrush(QBrush(QColor(30, 30, 30)));
    chart->setPlotAreaBackgroundVisible(false);

    // 2. Marginesy wewnętrzne layoutu na 0
    chart->layout()->setContentsMargins(0, 0, 0, 0);

    // 3. INTELIGENTNE MARGINESY (To jest kluczowa zmiana)
    if (pokazLegende) {
        // Jeśli jest legenda po lewej, ustawiamy margines lewy na 0.
        // Legenda sama zadba o odstęp, a my nie chcemy dodatkowej dziury.
        chart->setMargins(QMargins(0, 5, 5, 5));
    } else {
        // Jeśli nie ma legendy, musimy zostawić trochę miejsca na cyferki osi Y (np. 1.00).
        // 40 pikseli powinno wystarczyć, żeby liczby nie ucięło.
        chart->setMargins(QMargins(40, 5, 5, 5));
    }

    chart->setBackgroundRoundness(0);
    chart->setTitle("");

    // 4. Legenda
    if (pokazLegende) {
        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignLeft); // Legenda po lewej
        chart->legend()->setLabelBrush(QBrush(Qt::white));
        chart->legend()->setBackgroundVisible(false);

        // Zmniejszenie marginesów samej legendy, żeby "przykleiła się" bardziej do krawędzi
        chart->legend()->setContentsMargins(0, 0, 0, 0);

        QFont font = chart->legend()->font();
        font.setPointSize(8);
        chart->legend()->setFont(font);
    } else {
        chart->legend()->setVisible(false);
    }

    // 5. Osie
    auto axes = chart->axes();
    for (auto axis : axes) {
        axis->setLabelsBrush(QBrush(Qt::white));
        axis->setGridLineColor(QColor(60, 60, 60));
        axis->setTitleText("");
        axis->setLabelsVisible(true);

        QFont axisFont = axis->labelsFont();
        axisFont.setPointSize(8);
        axis->setLabelsFont(axisFont);
    }
}

void MainWindow::autoSkalujOsY(QChart* chart)
{
    if (!chart) return;

    // 1. Pobierz aktualny zakres osi X (czasu), żeby wiedzieć co jest widoczne
    double minX = 0;
    double maxX = 0;
    auto axesX = chart->axes(Qt::Horizontal);
    if (axesX.isEmpty()) return;

    // Zakładamy, że oś X to QValueAxis
    if (auto axisX = qobject_cast<QValueAxis*>(axesX.first())) {
        minX = axisX->min();
        maxX = axisX->max();
    }

    double minVal = 1e9;
    double maxVal = -1e9;
    bool hasData = false;

    // 2. Przeszukaj serie, ALE bierz tylko punkty z widocznego okna czasowego
    for (QAbstractSeries* series : chart->series()) {
        QLineSeries* lineSeries = qobject_cast<QLineSeries*>(series);
        if (!lineSeries || !lineSeries->isVisible()) continue;

        for (const QPointF& p : lineSeries->points()) {
            // KLUCZOWA ZMIANA: Ignoruj stare punkty, które wyjechały z lewej strony
            if (p.x() < minX || p.x() > maxX) continue;

            if (p.y() < minVal) minVal = p.y();
            if (p.y() > maxVal) maxVal = p.y();
            hasData = true;
        }
    }

    // Jeśli w aktualnym oknie nie ma danych (np. dopiero zaczęliśmy)
    if (!hasData) {
        minVal = -1.0;
        maxVal = 1.0;
    }

    // 3. Oblicz margines i ustaw oś
    double diff = maxVal - minVal;
    if (diff < 0.01) diff = 1.0; // Zabezpieczenie dla linii prostej
    double margin = diff * 0.1;  // 10% marginesu

    QList<QAbstractAxis*> axesY = chart->axes(Qt::Vertical);
    if (!axesY.isEmpty()) {
        QValueAxis* axisY = qobject_cast<QValueAxis*>(axesY.first());
        if (axisY) {
            axisY->setRange(minVal - margin, maxVal + margin);
            axisY->setLabelFormat("%.2f");
            axisY->setTickCount(6);
        }
    }
}
// ==========================================
// SLOTY
// ==========================================

void MainWindow::on_pushStart_clicked()
{
    // Startujemy timer GUI (odświeżanie)
    m_timerSymulacji->start();
    // Startujemy logikę (flaga m_czyDziala = true)
    m_logika.start();
    ui->statusbar->showMessage("Symulacja TRWA...");
}

void MainWindow::on_pushStop_clicked()
{
    m_timerSymulacji->stop();
    m_logika.stop();
    ui->statusbar->showMessage("Symulacja ZATRZYMANA.");
}

void MainWindow::on_pushResetSym_clicked()
{
    on_pushStop_clicked(); // Najpierw zatrzymaj

    m_logika.reset(); // Wyzeruj zmienne w logice (czas=0, y=0)

    // Wyzeruj wykresy
    m_seriesZadana->clear();
    m_seriesWyjscie->clear();
    m_seriesUchyb->clear();
    m_seriesSterowanie->clear();
    // ... i PID jeśli masz

    // Przywróć osie X do początku
    auto resetX = [&](QChart* chart) {
        if(!chart->axes(Qt::Horizontal).isEmpty())
            chart->axes(Qt::Horizontal).first()->setRange(0, 10);
    };
    resetX(m_chartOutput);
    resetX(m_chartError);
    resetX(m_chartControl);

    ui->statusbar->showMessage("Symulacja ZRESETOWANA.");
}

void MainWindow::aktualizujParametryGeneratora()
{
    // Pobranie wartości z kontrolek
    auto typ = static_cast<GeneratorWartosciZadanej::TypSygnalu>(ui->comboTypSygnalu->currentIndex());
    double ampl = ui->spinAmplituda->value();
    double okres = ui->spinOkres->value();
    int interwal = ui->spinInterwal->value();
    double stala = ui->spinSkladowaStala->value();
    double wypelnienie = ui->spinWypelnienie->value();

    // Wysłanie do logiki
    m_logika.nowyGenerator(ampl, okres, interwal, typ, stala, wypelnienie);
}

void MainWindow::aktualizujParametryPID()
{
    double kp = ui->spinPidKp->value();
    double ti = ui->spinPidTi->value();
    double td = ui->spinPidTd->value();

    // Wysłanie do logiki
    m_logika.nowyRegulator(kp, ti, td);
}

void MainWindow::aktualizujSymulacje()
{
    // 1. ZLECAMY OBLICZENIA (GUI -> Fasada -> Symulacja)
    m_logika.wykonajKrokSymulacji();

    // 2. POBIERAMY WYNIKI (Fasada -> GUI)
    double t = m_logika.getCzas();
    double w = m_logika.getWartoscZadana();
    double y = m_logika.getWartoscWyjscie();
    double e = m_logika.getUchyb();
    double u = m_logika.getSterowanie();

    // (Opcjonalnie: pobierz P, I, D jeśli masz do nich dostęp w KlasaUslugowa)
    // double p = ...

    // 3. AKTUALIZUJEMY WYKRESY (Dodajemy punkty)
    m_seriesZadana->append(t, w);
    m_seriesWyjscie->append(t, y);
    m_seriesUchyb->append(t, e);
    m_seriesSterowanie->append(t, u);

    // 4. PRZESUWAMY OKNO CZASOWE (Efekt "płynącego" wykresu)
    double windowSize = 20.0; // Pokaż ostatnie 20 sekund

    auto updateAxisX = [&](QChart* chart) {
        auto axes = chart->axes(Qt::Horizontal);
        if (axes.isEmpty()) return;
        QAbstractAxis* axis = axes.first();

        if (t > windowSize) {
            axis->setRange(t - windowSize, t);
        } else {
            axis->setRange(0, windowSize);
        }
    };

    updateAxisX(m_chartOutput);
    updateAxisX(m_chartError);
    updateAxisX(m_chartControl);
    updateAxisX(m_chartPID); // Jeśli używasz

    // 3. >>> NOWE: AUTO-SKALOWANIE OSI Y <<<
    autoSkalujOsY(m_chartOutput);   // Wykres Wyjścia i Zadanej
    autoSkalujOsY(m_chartError);    // Wykres Uchybu
    autoSkalujOsY(m_chartControl);  // Wykres Sterowania
    autoSkalujOsY(m_chartPID);      // Wykres PID

    // OPTYMALIZACJA: Usuwanie starych punktów, które są już daleko poza ekranem
    // np. jeśli są starsze niż 2x szerokość okna
    double limitCzasu = t - (windowSize * 2.0);

    if (m_seriesZadana->count() > 0 && m_seriesZadana->at(0).x() < limitCzasu) {
        m_seriesZadana->remove(0);
    }
    if (m_seriesWyjscie->count() > 0 && m_seriesWyjscie->at(0).x() < limitCzasu) {
        m_seriesWyjscie->remove(0);
    }
    // ... powtórz dla uchybu, sterowania, PID ...

    //PID
    m_seriesP->append(t, m_logika.getP());
    m_seriesI->append(t, m_logika.getI());
    m_seriesD->append(t, m_logika.getD());

}

void MainWindow::on_pushConfigARX_clicked()
{
    ParametryARX dialog(this);

    // 1. Ustawiamy aktualne wartości (żeby nie było pusto)
    dialog.ustawAktualne(
        m_logika.getWektorA(),
        m_logika.getWektorB(),
        m_logika.getOpoznienie(),
        m_logika.getSzum()
        );

    // 2. Otwieramy okno i czekamy na wynik
    // exec() zatrzymuje kod w tym miejscu, dopóki okno się nie zamknie.
    // Jeśli klikniesz "Zapisz" -> zwróci Accepted.
    // Jeśli klikniesz "Anuluj" lub "X" -> zwróci Rejected.

    if (dialog.exec() == QDialog::Accepted)
    {
        // === TUTAJ WCHODZIMY TYLKO JAK KLIKNIESZ ZAPISZ ===

        // 3. Pobieramy dane PRZED zniszczeniem obiektu dialog
        auto a = dialog.getA();
        auto b = dialog.getB();
        int op = dialog.getOpoznienie();
        double szum = dialog.getSzum();

        qDebug() << "Odebrano dane w MainWindow! A[0]:" << (a.empty() ? 0 : a[0]);

        // 4. Wysyłamy do logiki
        m_logika.nowyModelARX(a, b, op, szum);

        ui->statusbar->showMessage("Zaktualizowano parametry ARX.", 3000);
    }
    else
    {
        qDebug() << "Anulowano okno (nie kliknięto Zapisz)";
    }
}

void MainWindow::on_pushSaveConfig_clicked(){
    zapiszKonfiguracje();
}
void MainWindow::on_pushLoadConfig_clicked(){
    wczytajKonfiguracje();
}

void MainWindow::zapiszKonfiguracje() {
    QString fileName = QFileDialog::getSaveFileName( this, tr("Zapisz konfigurację"), "", tr("Pliki JSON (*.json)") );
    if (fileName.isEmpty()) return;
    QJsonObject root = m_logika.toJson();
    QJsonDocument doc(root);
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) return;
    file.write(doc.toJson());
    file.close();
}

void MainWindow::wczytajKonfiguracje() { QString fileName = QFileDialog::getOpenFileName( this, tr("Wczytaj konfigurację"), "", tr("Pliki JSON (*.json)") );
    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return;
    QByteArray data = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return; QJsonObject root = doc.object();
    m_logika.fromJson(root);
    odswiezGUI();
}

void MainWindow::odswiezGUI()
{
    // === GENERATOR ===
    const auto& gen = m_logika.getGenerator();

    ui->comboTypSygnalu->blockSignals(true);
    ui->spinAmplituda->blockSignals(true);
    ui->spinOkres->blockSignals(true);
    ui->spinInterwal->blockSignals(true);
    ui->spinSkladowaStala->blockSignals(true);
    ui->spinWypelnienie->blockSignals(true);

    ui->comboTypSygnalu->setCurrentIndex(static_cast<int>(gen.getTypSygnalu()));
    ui->spinAmplituda->setValue(gen.getAmplituda());
    ui->spinOkres->setValue(gen.getOkresRzeczywisty());
    ui->spinInterwal->setValue(gen.getInterwal());
    ui->spinSkladowaStala->setValue(gen.getSkladowaStala());
    ui->spinWypelnienie->setValue(gen.getWypelnienie());

    ui->comboTypSygnalu->blockSignals(false);
    ui->spinAmplituda->blockSignals(false);
    ui->spinOkres->blockSignals(false);
    ui->spinInterwal->blockSignals(false);
    ui->spinSkladowaStala->blockSignals(false);
    ui->spinWypelnienie->blockSignals(false);


    // === PID ===
    const auto& pid = m_logika.getRegulator();

    ui->spinPidKp->blockSignals(true);
    ui->spinPidTi->blockSignals(true);
    ui->spinPidTd->blockSignals(true);
    ui->comboMetCalk->blockSignals(true);

    ui->spinPidKp->setValue(pid.getWzmocnienie());
    ui->spinPidTi->setValue(pid.getStalaCalk());
    ui->spinPidTd->setValue(pid.getStalaRozn());
    ui->comboMetCalk->setCurrentIndex(static_cast<int>(pid.getLiczCalk()));

    ui->spinPidKp->blockSignals(false);
    ui->spinPidTi->blockSignals(false);
    ui->spinPidTd->blockSignals(false);
    ui->comboMetCalk->blockSignals(false);

    ui->statusbar->showMessage("Wczytano konfigurację z pliku JSON", 3000);
}





