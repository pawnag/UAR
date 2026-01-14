#include "mainwindow.h"
#include "ParametryARX.h"
#include "ui_mainwindow.h"
#include "W_DANYCH/GeneratorWartosciZadanej.h"
#include <QSignalBlocker>

// Stałe konfiguracyjne (KISS)
static constexpr double OKNO_CZASOWE_S = 10.0;
static constexpr double MARGINES_Y = 0.1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_timerSymulacji(new QTimer(this))
{
    ui->setupUi(this);

    // --- KONFIGURACJA WYKRESÓW (DRY) ---
    QVBoxLayout* layoutMain = new QVBoxLayout(ui->widgetWykresy);
    layoutMain->setContentsMargins(0, 0, 0, 0);

    // 1. Wykres główny (Góra)
    auto paraGlowna = stworzWykres("Wartość zadana i regulowana", "Odpowiedź układu");
    m_chartOutput = paraGlowna.first;
    layoutMain->addWidget(paraGlowna.second, 1);

    m_seriesZadana = dodajSerie(m_chartOutput, "Wartość zadana (w)", Qt::red);
    m_seriesWyjscie = dodajSerie(m_chartOutput, "Wartość regulowana (y)", QColor(0, 150, 255));

    // 2. Wykresy dolne (Kontener)
    QHBoxLayout* layoutDolny = new QHBoxLayout();
    layoutMain->addLayout(layoutDolny, 1);

    auto setupMalyWykres = [&](QString tytul, QString osY, QColor kol, QLineSeries** ptr) {
        auto para = stworzWykres(tytul, osY);
        *ptr = dodajSerie(para.first, tytul, kol);
        layoutDolny->addWidget(para.second);
        return para.first;
    };

    m_chartError = setupMalyWykres("Uchyb regulacji", "Uchyb", Qt::green, &m_seriesUchyb);
    m_chartControl = setupMalyWykres("Sygnał sterujący", "Sterowanie", Qt::magenta, &m_seriesSterowanie);

    // 3. Wykres PID
    auto paraPID = stworzWykres("Składowe sterowania", "Wartość PID");
    m_chartPID = paraPID.first;
    m_seriesP = dodajSerie(m_chartPID, "P", Qt::cyan);
    m_seriesI = dodajSerie(m_chartPID, "I", Qt::yellow);
    m_seriesD = dodajSerie(m_chartPID, "D", QColor(255, 100, 255));
    layoutDolny->addWidget(paraPID.second);

    // --- SYGNAŁY ---
    m_timerSymulacji->setInterval(ui->spinInterwal->value());
    connect(m_timerSymulacji, &QTimer::timeout, this, &MainWindow::aktualizujSymulacje);

    connect(ui->pushStart, &QPushButton::clicked, this, &MainWindow::on_pushStart_clicked);
    connect(ui->pushStop, &QPushButton::clicked, this, &MainWindow::on_pushStop_clicked);
    connect(ui->pushResetSym, &QPushButton::clicked, this, &MainWindow::on_pushResetSym_clicked);
    connect(ui->pushResetPID, &QPushButton::clicked, this, [this](){ m_logika.getRegulator().resetuj(); });

    // Generator & PID (grupowe podpięcie slotów)
    const auto updateGen = qOverload<double>(&QDoubleSpinBox::valueChanged);
    connect(ui->comboTypSygnalu, &QComboBox::currentIndexChanged, this, &MainWindow::aktualizujParametryGeneratora);
    connect(ui->spinAmplituda, updateGen, this, &MainWindow::aktualizujParametryGeneratora);
    connect(ui->spinOkres, updateGen, this, &MainWindow::aktualizujParametryGeneratora);
    connect(ui->spinSkladowaStala, updateGen, this, &MainWindow::aktualizujParametryGeneratora);
    connect(ui->spinWypelnienie, updateGen, this, &MainWindow::aktualizujParametryGeneratora);

    connect(ui->spinInterwal, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int val){
        m_timerSymulacji->setInterval(val);
        aktualizujParametryGeneratora();
    });

    const auto updatePID = qOverload<double>(&QDoubleSpinBox::valueChanged);
    connect(ui->spinPidKp, updatePID, this, &MainWindow::aktualizujParametryPID);
    connect(ui->spinPidTi, updatePID, this, &MainWindow::aktualizujParametryPID);
    connect(ui->spinPidTd, updatePID, this, &MainWindow::aktualizujParametryPID);
    connect(ui->comboMetCalk, &QComboBox::currentIndexChanged, this, &MainWindow::aktualizujParametryPID);

    // Init
    aktualizujParametryGeneratora();
    aktualizujParametryPID();
}

MainWindow::~MainWindow() { delete ui; }

// ==========================================
// LOGIKA WYKRESÓW (DRY & KISS)
// ==========================================

std::pair<QChart*, QChartView*> MainWindow::stworzWykres(QString tytul, QString osY) {
    QChart* chart = new QChart();
    chart->setTitle(tytul);
    chart->setBackgroundBrush(QBrush(QColor(30, 30, 30)));
    chart->setTitleBrush(Qt::white);
    chart->legend()->setAlignment(Qt::AlignTop);
    chart->legend()->setLabelBrush(Qt::white);
    chart->createDefaultAxes(); // Placeholder

    QChartView* view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);

    // Hack: Wymuszenie osi przed dodaniem serii, by móc je stylować od razu
    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("Czas [s]");
    axisX->setLabelsBrush(Qt::white);
    axisX->setTitleBrush(Qt::white);
    chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText(osY);
    axisY->setLabelsBrush(Qt::white);
    axisY->setTitleBrush(Qt::white);
    chart->addAxis(axisY, Qt::AlignLeft);

    return {chart, view};
}

QLineSeries* MainWindow::dodajSerie(QChart* chart, QString nazwa, QColor kolor) {
    QLineSeries* s = new QLineSeries();
    s->setName(nazwa);
    s->setPen(QPen(kolor, 2));
    chart->addSeries(s);
    // Podpięcie pod osie utworzone w stworzWykres
    chart->setAxisX(chart->axes(Qt::Horizontal).first(), s);
    chart->setAxisY(chart->axes(Qt::Vertical).first(), s);
    return s;
}

void MainWindow::zarzadzajWykresem(QChart* chart, double t) {
    if (!chart) return;

    // 1. Skalowanie X (Scroll)
    auto axisX = static_cast<QValueAxis*>(chart->axes(Qt::Horizontal).first());
    if (t > OKNO_CZASOWE_S)
        axisX->setRange(t - OKNO_CZASOWE_S, t);
    else
        axisX->setRange(0, OKNO_CZASOWE_S);

    // 2. Czyszczenie i Autoskalowanie Y (tylko widoczne punkty)
    double minVal = 1e9, maxVal = -1e9;
    double limitCzasu = t - OKNO_CZASOWE_S;
    bool hasData = false;

    for (auto series : chart->series()) {
        auto line = static_cast<QLineSeries*>(series);
        if (line->count() > 0 && line->at(0).x() < limitCzasu)
            line->remove(0); // Usuń stare

        // Szukaj min/max w widocznym oknie
        for(const auto& p : line->points()) {
            if(p.x() >= limitCzasu) {
                if (p.y() < minVal) minVal = p.y();
                if (p.y() > maxVal) maxVal = p.y();
                hasData = true;
            }
        }
    }

    if (!hasData) { minVal = -1.0; maxVal = 1.0; }

    // Margines Y
    double diff = maxVal - minVal;
    if (diff < 0.001) diff = 1.0;
    auto axisY = static_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    axisY->setRange(minVal - diff * MARGINES_Y, maxVal + diff * MARGINES_Y);
}

void MainWindow::aktualizujSymulacje()
{
    m_logika.wykonajKrokSymulacji();

    double t = m_logika.getCzas();

    // Helper lambda do aktualizacji serii i legendy
    auto updateS = [&](QLineSeries* s, double val, QString prefix, int prec) {
        s->append(t, val);
        s->setName(QString("%1: %2").arg(prefix).arg(val, 0, 'f', prec));
    };

    updateS(m_seriesZadana, m_logika.getWartoscZadana(), "Zadana", 2);
    updateS(m_seriesWyjscie, m_logika.getWartoscWyjscie(), "Wyjście", 2);
    updateS(m_seriesUchyb, m_logika.getUchyb(), "Uchyb", 3);
    updateS(m_seriesSterowanie, m_logika.getSterowanie(), "Sterowanie", 2);

    updateS(m_seriesP, m_logika.getP(), "P", 2);
    updateS(m_seriesI, m_logika.getI(), "I", 2);
    updateS(m_seriesD, m_logika.getD(), "D", 2);

    // Zarządzanie osiami (Scroll + AutoScale)
    zarzadzajWykresem(m_chartOutput, t);
    zarzadzajWykresem(m_chartError, t);
    zarzadzajWykresem(m_chartControl, t);
    zarzadzajWykresem(m_chartPID, t);
}

// ==========================================
// SLOTY UI
// ==========================================

void MainWindow::on_pushStart_clicked() {
    m_timerSymulacji->start();
    m_logika.start();
    ui->statusbar->showMessage("Symulacja TRWA...");
}

void MainWindow::on_pushStop_clicked() {
    m_timerSymulacji->stop();
    m_logika.stop();
    ui->statusbar->showMessage("Symulacja ZATRZYMANA.");
}

void MainWindow::on_pushResetSym_clicked() {
    on_pushStop_clicked();
    m_logika.reset();

    auto clearS = [](QLineSeries* s){ s->clear(); };
    clearS(m_seriesZadana); clearS(m_seriesWyjscie);
    clearS(m_seriesUchyb); clearS(m_seriesSterowanie);
    clearS(m_seriesP); clearS(m_seriesI); clearS(m_seriesD);

    // Reset osi X
    auto resetX = [](QChart* c) { c->axes(Qt::Horizontal).first()->setRange(0, OKNO_CZASOWE_S); };
    resetX(m_chartOutput); resetX(m_chartError);
    resetX(m_chartControl); resetX(m_chartPID);

    odswiezGUI();
    ui->statusbar->showMessage("Symulacja ZRESETOWANA.");
}

void MainWindow::aktualizujParametryGeneratora() {
    auto typ = static_cast<GeneratorWartosciZadanej::TypSygnalu>(ui->comboTypSygnalu->currentIndex());
    bool fixed = (typ == GeneratorWartosciZadanej::SYGNAL_STALY);

    ui->spinOkres->setEnabled(!fixed);
    ui->spinAmplituda->setEnabled(!fixed); // Zgodnie z logiką, stała nie ma amplitudy w sensie generatora fal
    ui->spinWypelnienie->setEnabled(typ == GeneratorWartosciZadanej::SYGNAL_PROSTOKATNY);

    m_logika.nowyGenerator(ui->spinAmplituda->value(), ui->spinOkres->value(),
                           ui->spinInterwal->value(), typ,
                           ui->spinSkladowaStala->value(), ui->spinWypelnienie->value());
}

void MainWindow::aktualizujParametryPID() {
    m_logika.nowyRegulator(ui->spinPidKp->value(), ui->spinPidTi->value(), ui->spinPidTd->value());
    // Jeśli potrzeba wysłać metodę całkowania:
    m_logika.getRegulator().setLiczCalk((RegulatorPID::LiczCalk)ui->comboMetCalk->currentIndex());
}

void MainWindow::on_pushConfigARX_clicked() {
    ParametryARX dialog(this);
    dialog.ustawAktualne(m_logika.getWektorA(), m_logika.getWektorB(),
                         m_logika.getOpoznienie(), m_logika.getSzum(),
                         m_logika.getModelUMIN(), m_logika.getModelUMAX(),
                         m_logika.getModelYMIN(), m_logika.getModelYMAX());

    if (dialog.exec() == QDialog::Accepted) {
        m_logika.nowyModelARX(dialog.getA(), dialog.getB(), dialog.getOpoznienie(),
                              dialog.getSzum(), dialog.getUMIN(), dialog.getUMAX(),
                              dialog.getYMIN(), dialog.getYMAX());
        ui->statusbar->showMessage("ARX Zaktualizowany.");
    }
}

void MainWindow::odswiezGUI() {
    // === 1. BLOKADA SYGNAŁÓW ===
    const QSignalBlocker bGen1(ui->comboTypSygnalu);
    const QSignalBlocker bGen2(ui->spinAmplituda);
    const QSignalBlocker bGen3(ui->spinOkres);
    const QSignalBlocker bGen4(ui->spinInterwal);
    const QSignalBlocker bGen5(ui->spinSkladowaStala);
    const QSignalBlocker bGen6(ui->spinWypelnienie);

    const QSignalBlocker bPid1(ui->spinPidKp);
    const QSignalBlocker bPid2(ui->spinPidTi);
    const QSignalBlocker bPid3(ui->spinPidTd);
    const QSignalBlocker bPid4(ui->comboMetCalk);

    // === 2. USTAWIENIE WARTOŚCI (Z LOGIKI DO GUI) ===
    const auto& gen = m_logika.getGenerator();
    ui->comboTypSygnalu->setCurrentIndex((int)gen.getTypSygnalu());
    ui->spinAmplituda->setValue(gen.getAmplituda());
    ui->spinOkres->setValue(gen.getOkresRzeczywisty());
    ui->spinInterwal->setValue(gen.getInterwal());
    ui->spinSkladowaStala->setValue(gen.getSkladowaStala());
    ui->spinWypelnienie->setValue(gen.getWypelnienie());

    const auto& pid = m_logika.getRegulator();
    ui->spinPidKp->setValue(pid.getWzmocnienie());
    ui->spinPidTi->setValue(pid.getStalaCalk());
    ui->spinPidTd->setValue(pid.getStalaRozn());
    ui->comboMetCalk->setCurrentIndex((int)pid.getLiczCalk());

    // === 3. NAPRAWA BLOKAD (Zarządzanie stanem UI) ===
    // Skoro zablokowaliśmy sygnały, musimy ręcznie odświeżyć
    // stan aktywności pól (Enabled/Disabled), bo slot się nie wykonał.

    bool fixed = (gen.getTypSygnalu() == GeneratorWartosciZadanej::SYGNAL_STALY);
    bool isRect = (gen.getTypSygnalu() == GeneratorWartosciZadanej::SYGNAL_PROSTOKATNY);

    ui->spinOkres->setEnabled(!fixed);
    ui->spinAmplituda->setEnabled(!fixed);
    ui->spinWypelnienie->setEnabled(isRect);
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

void MainWindow::wczytajKonfiguracje() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Wczytaj konfigurację"), "", tr("Pliki JSON (*.json)"));
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return;
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return;

    // 1. Wczytaj dane do logiki (tu dane są poprawne)
    QJsonObject root = doc.object();
    m_logika.fromJson(root);

    // 2. Zaktualizuj timer (bezpośrednio z logiki, nie przez GUI!)
    m_timerSymulacji->setInterval(m_logika.getGenerator().getInterwal());

    // 3. Odśwież GUI (To ustawi spinInterwal i inne kontrolki BEZ wyzwalania sygnałów)
    odswiezGUI();

    // 4. Reset symulacji
    on_pushResetSym_clicked();
}

