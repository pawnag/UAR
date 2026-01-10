#include "mainwindow.h"
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

    // 2. Proporcje (Stretch) - wszystkie równe
    ui->verticalLayout_Graphs->setStretch(0, 1);
    ui->verticalLayout_Graphs->setStretch(1, 1);
    ui->verticalLayout_Graphs->setStretch(2, 1);
    ui->verticalLayout_Graphs->setStretch(3, 1);

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

    // NAPRAWA BŁĘDU C2039: Używamy metody setPlotAreaBackgroundVisible
    chart->setPlotAreaBackgroundVisible(false);

    // 2. Usunięcie marginesów
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setMargins(QMargins(5, 5, 5, 5));
    chart->setBackgroundRoundness(0);
    chart->setTitle(""); // Brak tytułu

    // 3. Legenda
    if (pokazLegende) {
        chart->legend()->setVisible(true);
        // >>> ZMIANA: LEGENDA PO PRAWEJ STRONIE <<<
        chart->legend()->setAlignment(Qt::AlignRight);

        chart->legend()->setLabelBrush(QBrush(Qt::white));
        chart->legend()->setBackgroundVisible(false);

        QFont font = chart->legend()->font();
        font.setPointSize(8);
        chart->legend()->setFont(font);
    } else {
        chart->legend()->setVisible(false);
    }

    // 4. Osie
    auto axes = chart->axes();
    for (auto axis : axes) {
        axis->setLabelsBrush(QBrush(Qt::white));
        axis->setGridLineColor(QColor(60, 60, 60));
        axis->setTitleText("");
        QFont axisFont = axis->labelsFont();
        axisFont.setPointSize(8);
        axis->setLabelsFont(axisFont);
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
}

void MainWindow::on_pushConfigARX_clicked()
{

}

