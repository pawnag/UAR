#include "mainwindow.h"
#include "ParametryARX.h"
#include "ui_mainwindow.h"
#include "W_DANYCH/GeneratorWartosciZadanej.h"
#include <QSignalBlocker>

// Stałe konfiguracyjne
static constexpr double OKNO_CZASOWE_S = 10.0;
static constexpr double MARGINES_Y = 0.1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_timerSymulacji(new QTimer(this))
{
    // Magiczna linika: tutaj Qt parsuje plik .ui i podłącza sloty "on_..."
    ui->setupUi(this);

    // --- KONFIGURACJA WYKRESÓW ---
    QVBoxLayout* layoutMain = new QVBoxLayout(ui->widgetWykresy);
    layoutMain->setContentsMargins(0, 0, 0, 0);

    // 1. Wykres główny
    auto paraGlowna = stworzWykres("Wartość zadana i regulowana", "Odpowiedź układu");
    m_chartOutput = paraGlowna.first;
    layoutMain->addWidget(paraGlowna.second, 1);

    m_seriesZadana = dodajSerie(m_chartOutput, "Wartość zadana (w)", Qt::red);
    m_seriesWyjscie = dodajSerie(m_chartOutput, "Wartość regulowana (y)", QColor(0, 150, 255));

    // 2. Wykresy dolne
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

    // Timer nadal musi być ręcznie, bo nie jest widgetem z UI
    m_timerSymulacji->setInterval(ui->spinInterwal->value());
    connect(m_timerSymulacji, &QTimer::timeout, this, &MainWindow::aktualizujSymulacje);

    // ZMIANA: Usunięto ręczne connecty dla przycisków (Start/Stop/Reset).
    // Qt podłączy je samo, bo metody nazywają się on_pushStart_clicked itd.

    // Init
    aktualizujParametryGeneratora();
    aktualizujParametryPID();
}

MainWindow::~MainWindow() { delete ui; }

// ==========================================
// LOGIKA WYKRESÓW
// ==========================================

std::pair<QChart*, QChartView*> MainWindow::stworzWykres(QString tytul, QString osY) {
    QChart* chart = new QChart();
    chart->setTitle(tytul);
    chart->setBackgroundBrush(QBrush(QColor(30, 30, 30)));
    chart->setTitleBrush(Qt::white);
    chart->legend()->setAlignment(Qt::AlignTop);
    chart->legend()->setLabelBrush(Qt::white);
    chart->createDefaultAxes();

    QChartView* view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);

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
    chart->setAxisX(chart->axes(Qt::Horizontal).first(), s);
    chart->setAxisY(chart->axes(Qt::Vertical).first(), s);
    return s;
}

void MainWindow::zarzadzajWykresem(QChart* chart, double t) {
    if (!chart) return;

    auto axisX = static_cast<QValueAxis*>(chart->axes(Qt::Horizontal).first());
    if (t > OKNO_CZASOWE_S)
        axisX->setRange(t - OKNO_CZASOWE_S, t);
    else
        axisX->setRange(0, OKNO_CZASOWE_S);

    double minVal = 1e9, maxVal = -1e9;
    double limitCzasu = t - OKNO_CZASOWE_S;
    bool hasData = false;

    for (auto series : chart->series()) {
        auto line = static_cast<QLineSeries*>(series);
        if (line->count() > 0 && line->at(0).x() < limitCzasu)
            line->remove(0);

        for(const auto& p : line->points()) {
            if(p.x() >= limitCzasu) {
                if (p.y() < minVal) minVal = p.y();
                if (p.y() > maxVal) maxVal = p.y();
                hasData = true;
            }
        }
    }

    if (!hasData) { minVal = -1.0; maxVal = 1.0; }
    double diff = maxVal - minVal;
    if (diff < 0.001) diff = 1.0;
    auto axisY = static_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    axisY->setRange(minVal - diff * MARGINES_Y, maxVal + diff * MARGINES_Y);
}

void MainWindow::aktualizujSymulacje()
{
    m_logika.wykonajKrokSymulacji();

    double t = m_logika.getCzas();

    auto updateS = [&](QLineSeries* s, double val, QString prefix, int prec) {
        s->append(t, val);
        s->setName(QString("%1: %2").arg(prefix).arg(val, 0, 'f', prec));
    };

    updateS(m_seriesZadana, m_logika.getWartoscZadana(), "Zadana", 2);
    updateS(m_seriesWyjscie, m_logika.getWartoscWyjscie(), "Wyjście", 2);
    updateS(m_seriesUchyb, m_logika.getUchyb(), "Uchyb", 3);
    updateS(m_seriesSterowanie, m_logika.getSterowanie(), "Sterowanie", 2);

    // Opcjonalne pobieranie PID (jeśli dodano metody do KlasyUslugowej)
    // auto pid = m_logika.pobierzRegulator();
    // updateS(m_seriesP, pid.getLastP(), "P", 2);
    // updateS(m_seriesI, pid.getLastI(), "I", 2);
    // updateS(m_seriesD, pid.getLastD(), "D", 2);

    zarzadzajWykresem(m_chartOutput, t);
    zarzadzajWykresem(m_chartError, t);
    zarzadzajWykresem(m_chartControl, t);
    zarzadzajWykresem(m_chartPID, t);
}

// ==========================================
// SLOTY AUTO-CONNECT (Główne przyciski)
// Qt połączy je samo dzięki nazwom funkcji!
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

    auto resetX = [](QChart* c) { c->axes(Qt::Horizontal).first()->setRange(0, OKNO_CZASOWE_S); };
    resetX(m_chartOutput); resetX(m_chartError);
    resetX(m_chartControl); resetX(m_chartPID);

    odswiezGUI();
    ui->statusbar->showMessage("Symulacja ZRESETOWANA.");
}

// ==========================================
// METODY POMOCNICZE (SETTERY)
// ==========================================

void MainWindow::aktualizujParametryGeneratora() {
    auto typ = static_cast<GeneratorWartosciZadanej::TypSygnalu>(ui->comboTypSygnalu->currentIndex());

    bool fixed = (typ == GeneratorWartosciZadanej::SYGNAL_STALY);
    ui->spinOkres->setEnabled(!fixed);
    ui->spinAmplituda->setEnabled(!fixed);
    ui->spinWypelnienie->setEnabled(typ == GeneratorWartosciZadanej::SYGNAL_PROSTOKATNY);

    m_logika.setGenerator(
        ui->spinAmplituda->value(),
        ui->spinOkres->value(),
        ui->spinInterwal->value(),
        typ,
        ui->spinSkladowaStala->value(),
        ui->spinWypelnienie->value()
        );
}

void MainWindow::aktualizujParametryPID() {
    m_logika.setRegulator(
        ui->spinPidKp->value(),
        ui->spinPidTi->value(),
        ui->spinPidTd->value()
        );
    m_logika.setPidMetodaCalkowania(ui->comboMetCalk->currentIndex());
}

void MainWindow::on_pushConfigARX_clicked() {
    ParametryARX dialog(this);

    // 1. Pobieramy kopię modelu z logiki
    auto model = m_logika.pobierzModel();

    // 2. Przekazujemy cały obiekt do okna (CZYSTO!)
    dialog.ustawAktualne(model);

    // 3. Odbieramy sygnał (tutaj nadal odbieramy rozbite wartości,
    // co jest OK, bo setter w KlasaUslugowa wymaga rozbicia)
    connect(&dialog, &ParametryARX::zglosNoweParametry,
            this, [this](std::vector<double> a, std::vector<double> b,
                   int k, double szum, double umin, double umax, double ymin, double ymax)
            {
                m_logika.setModelARX(a, b, k, szum, umin, umax, ymin, ymax);
                ui->statusbar->showMessage("Zaktualizowano parametry ARX.");
            });

    dialog.exec();
}

void MainWindow::odswiezGUI() {
    // 1. Blokada sygnałów (żeby nie wyzwalać slotów on_... przy ustawianiu)
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

    // 2. Obsługa Generatora (tak jak miałeś)
    auto gen = m_logika.pobierzGenerator();
    ui->comboTypSygnalu->setCurrentIndex((int)gen.getTypSygnalu());
    ui->spinAmplituda->setValue(gen.getAmplituda());
    ui->spinOkres->setValue(gen.getOkresRzeczywisty());
    ui->spinInterwal->setValue(gen.getInterwal());
    ui->spinSkladowaStala->setValue(gen.getSkladowaStala());
    ui->spinWypelnienie->setValue(gen.getWypelnienie());

    // 3. Obsługa PID (TERAZ TAK SAMO JAK GENERATOR)
    auto pid = m_logika.pobierzRegulator(); // Pobieramy kopię obiektu
    ui->spinPidKp->setValue(pid.getWzmocnienie());
    ui->spinPidTi->setValue(pid.getStalaCalk());
    ui->spinPidTd->setValue(pid.getStalaRozn());
    ui->comboMetCalk->setCurrentIndex((int)pid.getLiczCalk());

    // 4. Odświeżenie stanu kontrolek (enabled/disabled)
    aktualizujParametryGeneratora();
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
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
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

    m_logika.fromJson(doc.object());
    m_timerSymulacji->setInterval(m_logika.pobierzGenerator().getInterwal());

    odswiezGUI();
    on_pushResetSym_clicked();
}

// ==========================================
// SLOTY AUTO-CONNECT (Parametry)
// ==========================================

void MainWindow::on_spinAmplituda_valueChanged(double arg1) {
    Q_UNUSED(arg1);
    aktualizujParametryGeneratora();
}

void MainWindow::on_comboTypSygnalu_currentIndexChanged(int index) {
    Q_UNUSED(index);
    aktualizujParametryGeneratora();
}

void MainWindow::on_spinOkres_valueChanged(double arg1) {
    Q_UNUSED(arg1);
    aktualizujParametryGeneratora();
}

void MainWindow::on_spinSkladowaStala_valueChanged(double arg1) {
    Q_UNUSED(arg1);
    aktualizujParametryGeneratora();
}

void MainWindow::on_spinWypelnienie_valueChanged(double arg1) {
    Q_UNUSED(arg1);
    aktualizujParametryGeneratora();
}

void MainWindow::on_spinInterwal_valueChanged(int arg1) {
    m_timerSymulacji->setInterval(arg1);
    aktualizujParametryGeneratora();
}

void MainWindow::on_spinPidKp_valueChanged(double arg1) {
    Q_UNUSED(arg1);
    aktualizujParametryPID();
}

void MainWindow::on_spinPidTi_valueChanged(double arg1) {
    Q_UNUSED(arg1);
    aktualizujParametryPID();
}

void MainWindow::on_spinPidTd_valueChanged(double arg1) {
    Q_UNUSED(arg1);
    aktualizujParametryPID();
}

void MainWindow::on_comboMetCalk_currentIndexChanged(int index) {
    Q_UNUSED(index);
    aktualizujParametryPID();
}

void MainWindow::on_pushResetPID_clicked() {
    m_logika.resetPID();
    ui->statusbar->showMessage("PID zresetowany.");
}
