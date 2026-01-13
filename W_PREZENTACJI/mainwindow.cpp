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

    // ... (początek konstruktora bez zmian)

    // ==========================================
    // KONFIGURACJA WYKRESÓW (POPRAWIONA)
    // ==========================================
    QVBoxLayout* layoutKontenera = new QVBoxLayout(ui->widgetWykresy);
    layoutKontenera->setContentsMargins(0, 0, 0, 0);

    // --- GÓRA (DUŻY) ---
    m_seriesZadana = new QLineSeries(); m_seriesZadana->setName("Wartość zadana (w)");
    m_seriesZadana->setPen(QPen(Qt::red, 2));
    m_seriesWyjscie = new QLineSeries(); m_seriesWyjscie->setName("Wartość regulowana (y)");
    m_seriesWyjscie->setPen(QPen(QColor(0, 150, 255), 2));

    m_chartOutput = new QChart();
    m_chartOutput->setTitle("Wartość zadana i regulowana"); // <--- DODANY TYTUŁ
    m_chartOutput->addSeries(m_seriesZadana);
    m_chartOutput->addSeries(m_seriesWyjscie);
    m_chartOutput->createDefaultAxes();
    stylizujWykres(m_chartOutput, true, "Odpowiedź układu");

    m_chartViewOutput = new QChartView(m_chartOutput);
    m_chartViewOutput->setRenderHint(QPainter::Antialiasing);

    // ZMIANA PROPORCJI: Zamiast 60 dajemy 1
    layoutKontenera->addWidget(m_chartViewOutput, 1);

    // --- DÓŁ (3 MNIEJSZE) ---
    QHBoxLayout* layoutDolny = new QHBoxLayout();

    // Uchyb
    m_seriesUchyb = new QLineSeries(); m_seriesUchyb->setName("Uchyb (e)"); m_seriesUchyb->setColor(Qt::green);
    m_chartError = new QChart();
    m_chartError->setTitle("Uchyb regulacji"); // <--- DODANY TYTUŁ
    m_chartError->addSeries(m_seriesUchyb);
    m_chartError->createDefaultAxes();
    stylizujWykres(m_chartError, true, "Uchyb");
    m_chartViewError = new QChartView(m_chartError);
    m_chartViewError->setRenderHint(QPainter::Antialiasing);
    layoutDolny->addWidget(m_chartViewError);

    // Sterowanie
    m_seriesSterowanie = new QLineSeries(); m_seriesSterowanie->setName("Sterowanie (u)"); m_seriesSterowanie->setColor(Qt::magenta);
    m_chartControl = new QChart();
    m_chartControl->setTitle("Sygnał sterujący"); // <--- DODANY TYTUŁ
    m_chartControl->addSeries(m_seriesSterowanie);
    m_chartControl->createDefaultAxes();
    stylizujWykres(m_chartControl, true, "Wyjście regulatora");
    m_chartViewControl = new QChartView(m_chartControl);
    m_chartViewControl->setRenderHint(QPainter::Antialiasing);
    layoutDolny->addWidget(m_chartViewControl);

    // PID
    m_seriesP = new QLineSeries(); m_seriesP->setName("P"); m_seriesP->setColor(Qt::cyan);
    m_seriesI = new QLineSeries(); m_seriesI->setName("I"); m_seriesI->setColor(Qt::yellow);
    m_seriesD = new QLineSeries(); m_seriesD->setName("D"); m_seriesD->setColor(QColor(255, 100, 255));
    m_chartPID = new QChart();
    m_chartPID->setTitle("Składowe sterowania"); // <--- DODANY TYTUŁ
    m_chartPID->addSeries(m_seriesP); m_chartPID->addSeries(m_seriesI); m_chartPID->addSeries(m_seriesD);
    m_chartPID->createDefaultAxes();
    stylizujWykres(m_chartPID, true, "Wartość PID");
    m_chartViewPID = new QChartView(m_chartPID);
    m_chartViewPID->setRenderHint(QPainter::Antialiasing);
    layoutDolny->addWidget(m_chartViewPID);

    // ZMIANA PROPORCJI: Zamiast 40 dajemy 1
    // Jeśli góra ma 1 i dół ma 1, to dzielą ekran po połowie (50%/50%)
    layoutKontenera->addLayout(layoutDolny, 1);

    // ... (reszta konstruktora bez zmian)

    // ==========================================
    // LOGIKA I SYGNAŁY (Reszta kodu bez zmian)
    // ==========================================
    m_timerSymulacji->setInterval(ui->spinInterwal->value());
    connect(m_timerSymulacji, &QTimer::timeout, this, &MainWindow::aktualizujSymulacje);

    // Sygnały UI - GUI
    connect(ui->pushStart, &QPushButton::clicked, this, &MainWindow::on_pushStart_clicked);
    connect(ui->pushStop, &QPushButton::clicked, this, &MainWindow::on_pushStop_clicked);
    connect(ui->pushResetSym, &QPushButton::clicked, this, &MainWindow::on_pushResetSym_clicked);
    connect(ui->pushResetPID, &QPushButton::clicked, this, &MainWindow::on_pushResetPID_clicked);

    // Generator
    connect(ui->comboTypSygnalu, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::aktualizujParametryGeneratora);

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
    connect(ui->comboMetCalk, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::aktualizujParametryPID);

    // ARX i Pliki
    connect(ui->pushConfigARX, &QPushButton::clicked, this, &MainWindow::on_pushConfigARX_clicked);
    connect(ui->pushSaveConfig, &QPushButton::clicked, this, &MainWindow::on_pushSaveConfig_clicked);
    connect(ui->pushLoadConfig, &QPushButton::clicked, this, &MainWindow::on_pushLoadConfig_clicked);

    // Inicjalizacja początkowa
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

void MainWindow::stylizujWykres(QChart* chart, bool pokazLegende, QString tytulOsY) {
    // 1. Podstawowy wygląd
    chart->setBackgroundBrush(QBrush(QColor(30, 30, 30)));
    chart->setPlotAreaBackgroundVisible(false);

    // Resetujemy marginesy layoutu, ale zostawiamy marginesy wykresu dla opisów
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    // Zwiększamy marginesy: Lewy (dla tytułu Y), Dolny (dla Czas [s]), Prawy (dla estetyki)
    chart->setMargins(QMargins(10, 0, 0, 10));
    chart->setBackgroundRoundness(0);

    chart->setTitleBrush(QBrush(Qt::white));
    chart->setTitleFont(QFont("Arial", 10, QFont::Bold));

    // 2. Legenda PO PRAWEJ STRONIE
    if (pokazLegende) {
        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignTop); // <--- ZMIANA: Prawa strona
        chart->legend()->setLabelBrush(QBrush(Qt::white));
        chart->legend()->setBackgroundVisible(false);
        QFont font = chart->legend()->font();
        font.setPointSize(8);
        chart->legend()->setFont(font);
    } else {
        chart->legend()->setVisible(false);
    }

    // 3. Konfiguracja OSI (Opisy)
    auto axes = chart->axes();

    // Szukamy osi X i Y
    QAbstractAxis* axisX = nullptr;
    QAbstractAxis* axisY = nullptr;

    // Pobieramy osie (zakładając, że createDefaultAxes() je utworzyło)
    auto axesX = chart->axes(Qt::Horizontal);
    auto axesY = chart->axes(Qt::Vertical);
    if (!axesX.isEmpty()) axisX = axesX.first();
    if (!axesY.isEmpty()) axisY = axesY.first();

    // Stylizacja Osi X (Czas)
    if (axisX) {
        axisX->setLabelsBrush(QBrush(Qt::white));
        axisX->setGridLineColor(QColor(60, 60, 60));

        // Ustawienie napisu "Czas [s]"
        axisX->setTitleText("Czas [s]");
        axisX->setTitleBrush(QBrush(Qt::white));
        axisX->setTitleVisible(true);

        QFont axisFont = axisX->labelsFont();
        axisFont.setPointSize(8);
        axisX->setLabelsFont(axisFont);
    }

    // Stylizacja Osi Y (Wartości)
    if (axisY) {
        axisY->setLabelsBrush(QBrush(Qt::white));
        axisY->setGridLineColor(QColor(60, 60, 60));

        // Ustawienie napisu z parametru (np. "Odpowiedź układu")
        axisY->setTitleText(tytulOsY);
        axisY->setTitleBrush(QBrush(Qt::white));
        axisY->setTitleVisible(true);

        QFont axisFont = axisY->labelsFont();
        axisFont.setPointSize(8);
        axisY->setLabelsFont(axisFont);
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
    m_seriesP->clear();
    m_seriesI->clear();
    m_seriesD->clear();

    // Przywróć osie X do początku
    auto resetX = [&](QChart* chart) {
        if(!chart->axes(Qt::Horizontal).isEmpty())
            chart->axes(Qt::Horizontal).first()->setRange(0, 10);
    };
    resetX(m_chartOutput);
    resetX(m_chartError);
    resetX(m_chartControl);

    ui->statusbar->showMessage("Symulacja ZRESETOWANA.");
    
    //zerowanie wartosci

    //delete m_logika.getRegulator();
    //delete m_logika.getGenerator();
    //delete m_logika.get();

    odswiezGUI();
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

    bool czyStala = (typ == GeneratorWartosciZadanej::SYGNAL_STALY);
    bool czyProstokat = (typ == GeneratorWartosciZadanej::SYGNAL_PROSTOKATNY);

    if (czyStala) {
        // --- Ustawienia dla WARTOŚCI STAŁEJ ---

        // Blokujemy Okres (bo sygnał stały nie ma okresu)
        ui->spinOkres->setEnabled(false);

        // Blokujemy Amplitudę (zgodnie z Twoją prośbą)
        ui->spinAmplituda->setEnabled(false);

        // Blokujemy Wypełnienie (nie dotyczy stałej)
        ui->spinWypelnienie->setEnabled(false);

        // Zostawiamy Składową Stałą OD BLOKOWANĄ - żebyś miał gdzie wpisać wartość!
        ui->spinSkladowaStala->setEnabled(true);

    } else {
        // --- Ustawienia dla POZOSTAŁYCH (Sinus, Prostokąt) ---

        ui->spinOkres->setEnabled(true);
        ui->spinAmplituda->setEnabled(true);
        ui->spinSkladowaStala->setEnabled(true); // Offset zazwyczaj dostępny zawsze

        // Wypełnienie aktywne zazwyczaj tylko dla prostokąta
        ui->spinWypelnienie->setEnabled(czyProstokat);
    }

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

    // Wykres 1: Zadana i Wyjście
    // Formatuje liczbę do 2 miejsc po przecinku (f, 2)
    m_seriesZadana->setName(QString("Wartość zadana (w): %1").arg(w, 0, 'f', 2));
    m_seriesWyjscie->setName(QString("Wartość regulowana (y): %1").arg(y, 0, 'f', 2));

    // Wykres 2: Uchyb (UWAGA: musisz włączyć legendę dla tego wykresu, patrz Krok 2)
    m_seriesUchyb->setName(QString("Uchyb (e): %1").arg(e, 0, 'f', 3));

    // Wykres 3: Sterowanie (UWAGA: musisz włączyć legendę dla tego wykresu)
    m_seriesSterowanie->setName(QString("Sterowanie (u): %1").arg(u, 0, 'f', 2));

    // Wykres 4: PID (opcjonalnie)
    m_seriesP->setName(QString("P: %1").arg(m_logika.getP(), 0, 'f', 2));
    m_seriesI->setName(QString("I: %1").arg(m_logika.getI(), 0, 'f', 2));
    m_seriesD->setName(QString("D: %1").arg(m_logika.getD(), 0, 'f', 2));

    // 3. AKTUALIZUJEMY WYKRESY (Dodajemy punkty)
    m_seriesZadana->append(t, w);
    m_seriesWyjscie->append(t, y);
    m_seriesUchyb->append(t, e);
    m_seriesSterowanie->append(t, u);

    // 4. PRZESUWAMY OKNO CZASOWE (Efekt "płynącego" wykresu)
    double windowSize = 10.0; // Pokaż ostatnie 20 sekund

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

    // OPTYMALIZACJA: Usuwanie starych punktów
    double limitCzasu = t - (windowSize * 2.0);

    // Funkcja pomocnicza lambda, żeby nie kopiować kodu
    auto czyscStareProbki = [&](QLineSeries* seria) {
        if (seria && seria->count() > 0 && seria->at(0).x() < limitCzasu) {
            seria->remove(0);
            // Jeśli interwał jest bardzo mały, można usunąć więcej próbek naraz:
            // seria->removePoints(0, seria->count() - ilesTam);
            // ale remove(0) przy każdym cyklu zazwyczaj wystarcza.
        }
    };

    // 1. Wykres główny
    czyscStareProbki(m_seriesZadana);
    czyscStareProbki(m_seriesWyjscie);

    // 2. Wykres Uchybu (TEGO BRAKOWAŁO)
    czyscStareProbki(m_seriesUchyb);

    // 3. Wykres Sterowania (TEGO BRAKOWAŁO)
    czyscStareProbki(m_seriesSterowanie);

    // 4. Wykres PID (TEGO TEŻ BRAKOWAŁO)
    czyscStareProbki(m_seriesP);
    czyscStareProbki(m_seriesI);
    czyscStareProbki(m_seriesD);

    //PID
    m_seriesP->append(t, m_logika.getP());
    m_seriesI->append(t, m_logika.getI());
    m_seriesD->append(t, m_logika.getD());

}

void MainWindow::on_pushResetPID_clicked(){
    m_logika.getRegulator().resetuj();
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

        //qDebug() << "Odebrano dane w MainWindow! A[0]:" << (a.empty() ? 0 : a[0]);

        // 4. Wysyłamy do logiki
        m_logika.nowyModelARX(a, b, op, szum);

        ui->statusbar->showMessage("Zaktualizowano parametry ARX.", 3000);
    }
    else
    {
        //qDebug() << "Anulowano okno (nie kliknięto Zapisz)";
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
    ui->spinInterwal->setValue(m_logika.getGenerator().getInterwal());
    m_timerSymulacji->setInterval(m_logika.getGenerator().getInterwal());
    odswiezGUI();
    on_pushResetSym_clicked();
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
}
