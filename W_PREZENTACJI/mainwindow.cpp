#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ParametryARX.h"
#include <QSignalBlocker>
#include <QFileDialog>
#include <QFileDialog>
#include <QLineEdit>

// Stałe konfiguracyjne
static constexpr double OKNO_CZASOWE_S = 10.0;
static constexpr double MARGINES_Y = 0.1;

MainWindow::MainWindow(QWidget *parent, KlasaUslugowa *usluga)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_usluga(usluga)
    , m_oknoCzasowe(10.0) // Domyślnie 10 sekund
{
    ui->setupUi(this);

    // Zabezpieczenie (gdyby uruchomiono okno bez wstrzykniętej usługi)
    if (!m_usluga) {
        m_usluga = new KlasaUslugowa(nullptr);
    }

    // ---------------------------------------------------------
    // 1. KONFIGURACJA UI I WYKRESÓW
    // ---------------------------------------------------------

    QVBoxLayout* layoutMain = new QVBoxLayout(ui->widgetWykresy);
    layoutMain->setContentsMargins(0, 0, 0, 0);

    // --- Wykres 1: Górny (Główny) ---
    auto paraGlowna = stworzWykres("Wartość zadana i regulowana", "Odpowiedź układu");
    m_chartOutput = paraGlowna.first;
    layoutMain->addWidget(paraGlowna.second, 1); // 50% wysokości

    // --- ZMIANA: Ustawienie podziałki co 1s TYLKO dla górnego wykresu ---
    if (!m_chartOutput->axes(Qt::Horizontal).isEmpty()) {
        auto axisX = static_cast<QValueAxis*>(m_chartOutput->axes(Qt::Horizontal).first());
        axisX->setTickCount(11); // 11 kresek na 10s = co 1 sekundę
    }

    m_seriesZadana = dodajSerie(m_chartOutput, "Wartość zadana (w)", Qt::red);
    m_seriesWyjscie = dodajSerie(m_chartOutput, "Wartość regulowana (y)", QColor(0, 150, 255));

    // Layout dla dolnych wykresów
    QHBoxLayout* layoutDolny = new QHBoxLayout();
    layoutMain->addLayout(layoutDolny, 1); // 50% wysokości

    // --- Wykres 2: Uchyb ---
    auto paraUchyb = stworzWykres("Uchyb regulacji", "Uchyb");
    m_chartError = paraUchyb.first;
    layoutDolny->addWidget(paraUchyb.second);
    m_seriesUchyb = dodajSerie(m_chartError, "Uchyb", Qt::green);

    // --- Wykres 3: Sterowanie ---
    auto paraSter = stworzWykres("Sygnał sterujący", "Sterowanie");
    m_chartControl = paraSter.first;
    layoutDolny->addWidget(paraSter.second);
    m_seriesSterowanie = dodajSerie(m_chartControl, "Sterowanie", Qt::magenta);

    //Wykres 4: PID
    auto paraPID = stworzWykres("Składowe sterowania", "Wartość PID");
    m_chartPID = paraPID.first;
    layoutDolny->addWidget(paraPID.second);
    m_seriesP = dodajSerie(m_chartPID, "P", Qt::cyan);
    m_seriesI = dodajSerie(m_chartPID, "I", Qt::yellow);
    m_seriesD = dodajSerie(m_chartPID, "D", QColor(255, 100, 255));

    connect(m_usluga, &KlasaUslugowa::noweDaneDostepne, this, &MainWindow::aktualizujWykresy);

    odswiezGUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ---------------------------------------------------------
// KONFIGURACJA WYKRESÓW
// ---------------------------------------------------------

std::pair<QChart*, QChartView*> MainWindow::stworzWykres(QString tytul, QString osY) {
    QChart* chart = new QChart();
    chart->setTitle(tytul);

    // Styl
    chart->setBackgroundBrush(QBrush(QColor(30, 30, 30)));
    chart->setTitleBrush(Qt::white);
    chart->legend()->setAlignment(Qt::AlignTop);
    chart->legend()->setLabelBrush(Qt::white);

    // Pędzel do osi (Biały)
    QPen axisPen(Qt::white);
    axisPen.setWidth(1);

    // Oś X
    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("Czas [s]");
    axisX->setLabelsBrush(Qt::white);
    axisX->setTitleBrush(Qt::white);
    axisX->setGridLineColor(QColor(80, 80, 80));
    axisX->setLinePen(axisPen);

    // Ustawiamy zakres początkowy od razu (0-10s)
    axisX->setRange(0, OKNO_CZASOWE_S);

    chart->addAxis(axisX, Qt::AlignBottom);

    // Oś Y
    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText(osY);
    axisY->setLabelsBrush(Qt::white);
    axisY->setTitleBrush(Qt::white);
    axisY->setGridLineColor(QColor(80, 80, 80));
    axisY->setLinePen(axisPen);
    chart->addAxis(axisY, Qt::AlignLeft);

    QChartView* view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);

    return {chart, view};
}

QLineSeries* MainWindow::dodajSerie(QChart* chart, QString nazwa, QColor kolor) {
    QLineSeries* s = new QLineSeries();
    s->setName(nazwa);
    s->setPen(QPen(kolor, 2));

    // Punkty widoczne tylko na głównym wykresie
    if (chart == m_chartOutput) {
        s->setPointsVisible(true);
        s->setMarkerSize(3);
    } else {
        s->setPointsVisible(false);
    }
    s->setPointLabelsVisible(false);

    chart->addSeries(s);

    if (!chart->axes(Qt::Horizontal).isEmpty())
        s->attachAxis(chart->axes(Qt::Horizontal).first());

    if (!chart->axes(Qt::Vertical).isEmpty())
        s->attachAxis(chart->axes(Qt::Vertical).first());

    return s;
}
// W pliku: W_PREZENTACJI/mainwindow.cpp

// W pliku: W_PREZENTACJI/mainwindow.cpp
// W pliku: W_PREZENTACJI/mainwindow.cpp

// W pliku: W_PREZENTACJI/mainwindow.cpp

void MainWindow::zarzadzajWykresem(QChart* chart, double t) {
    if (!chart) return;

    auto axisX = static_cast<QValueAxis*>(chart->axes(Qt::Horizontal).first());

    // --- LOGIKA OSI CZASU ---
    // Cel: Stała szerokość okna od samego początku.
    // Np. ustawiasz 20s -> oś jest 0-20s, wykres dochodzi do 5s, reszta pusta.

    double minX, maxX;

    if (t <= m_oknoCzasowe) {
        // Wariant 1: Czas nie przekroczył jeszcze zadanego okna.
        // Ustawiamy sztywno od 0 do m_oknoCzasowe.
        // Dzięki temu widać "wolne miejsce" po prawej stronie.
        minX = 0.0;
        maxX = m_oknoCzasowe;
    } else {
        // Wariant 2: Czas przekroczył okno (np. t=25s, okno=20s).
        // Przesuwamy widok (scrolling): 5s - 25s.
        minX = t - m_oknoCzasowe;
        maxX = t;
    }

    axisX->setRange(minX, maxX);

    // --- SKALOWANIE Y I CZYSZCZENIE (Bez zmian) ---

    // Usuwamy tylko dane, które wyszły daleko poza ekran (bufor bezpieczeństwa)
    double limitBezpieczenstwa = 200.0;
    double limitUsuwania = t - limitBezpieczenstwa;

    double minVal = 1e9, maxVal = -1e9;
    bool hasData = false;

    // Szukamy min/max tylko w widocznym fragmencie, żeby wykres Y dobrze się skalował
    // Uwaga: 'limitWidocznosci' to początek osi X
    double limitWidocznosci = minX;

    for (auto series : chart->series()) {
        auto line = static_cast<QLineSeries*>(series);

        // 1. Usuwanie bardzo starych danych
        if (line->count() > 0 && line->at(0).x() < limitUsuwania) {
            line->remove(0);
        }

        // 2. Skalowanie Y
        for(const auto& p : line->points()) {
            if(p.x() >= limitWidocznosci) {
                if (p.y() < minVal) minVal = p.y();
                if (p.y() > maxVal) maxVal = p.y();
                hasData = true;
            }
        }
    }

    if (hasData) {
        double diff = maxVal - minVal;
        if (diff < 0.1) diff = 1.0;
        double margines = diff * 0.1;

        auto axisY = static_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
        axisY->setRange(minVal - margines, maxVal + margines);
    }
}
// ---------------------------------------------------------
// SLOTY LOGIKI (Użycie Fasady - typy proste)
// ---------------------------------------------------------
void MainWindow::aktualizujWykresy() {
    // 1. Pobieramy wszystkie aktualne wartości z usługi
    double t = m_usluga->getCzas();
    double w = m_usluga->getWartoscZadana();
    double y = m_usluga->getWartoscWyjscie();
    double e = m_usluga->getUchyb();
    double u = m_usluga->getSterowanie();

    double valP = m_usluga->getPidLastP();
    double valI = m_usluga->getPidLastI();
    double valD = m_usluga->getPidLastD();

    // 2. Dodajemy punkty do wykresów
    m_seriesZadana->append(t, w);
    m_seriesWyjscie->append(t, y);
    m_seriesUchyb->append(t, e);
    m_seriesSterowanie->append(t, u);

    m_seriesP->append(t, valP);
    m_seriesI->append(t, valI);
    m_seriesD->append(t, valD);

    // 3. --- AKTUALIZACJA LEGENDY (DYNAMICZNE NAPISY) ---
    // Formatujemy liczbę do 2 lub 4 miejsc po przecinku
    m_seriesZadana->setName(QString("Wartość zadana (w): %1").arg(w, 0, 'f', 2));
    m_seriesWyjscie->setName(QString("Wartość regulowana (y): %1").arg(y, 0, 'f', 2));

    m_seriesUchyb->setName(QString("Uchyb: %1").arg(e, 0, 'f', 4)); // Uchyb warto widzieć dokładniej
    m_seriesSterowanie->setName(QString("Sterowanie: %1").arg(u, 0, 'f', 2));

    m_seriesP->setName(QString("P: %1").arg(valP, 0, 'f', 2));
    m_seriesI->setName(QString("I: %1").arg(valI, 0, 'f', 2));
    m_seriesD->setName(QString("D: %1").arg(valD, 0, 'f', 2));

    // 4. Przesuwanie i skalowanie osi
    zarzadzajWykresem(m_chartOutput, t);
    zarzadzajWykresem(m_chartError, t);
    zarzadzajWykresem(m_chartControl, t);
    zarzadzajWykresem(m_chartPID, t);
}

void MainWindow::aktualizujParametryGeneratora() {
    auto typ = static_cast<GeneratorWartosciZadanej::TypSygnalu>(ui->comboTypSygnalu->currentIndex());
    bool constant = (typ == GeneratorWartosciZadanej::SYGNAL_STALY);

    ui->spinOkres->setEnabled(!constant);
    ui->spinAmplituda->setEnabled(!constant);
    ui->spinWypelnienie->setEnabled(typ == GeneratorWartosciZadanej::SYGNAL_PROSTOKATNY);

    m_usluga->setGenerator(
        ui->spinAmplituda->value(),
        ui->spinOkres->value(),
        ui->spinInterwal->value(),
        typ,
        ui->spinSkladowaStala->value(),
        ui->spinWypelnienie->value()
        );
}

void MainWindow::aktualizujParametryPID() {
    m_usluga->setRegulator(
        ui->spinPidKp->value(),
        ui->spinPidTi->value(),
        ui->spinPidTd->value()
        );
    m_usluga->setPidMetodaCalkowania(ui->comboMetCalk->currentIndex());
}

void MainWindow::on_pushStart_clicked() { m_usluga->start(); }
void MainWindow::on_pushStop_clicked() { m_usluga->stop(); }

void MainWindow::on_pushResetSym_clicked() {
    m_usluga->reset();

    auto cleanChart = [](QChart* c) {
        for(auto s : c->series()) static_cast<QLineSeries*>(s)->clear();
        if(!c->axes(Qt::Horizontal).isEmpty())
            c->axes(Qt::Horizontal).first()->setRange(0, OKNO_CZASOWE_S);
    };

    cleanChart(m_chartOutput);
    cleanChart(m_chartError);
    cleanChart(m_chartControl);
    cleanChart(m_chartPID);
}

void MainWindow::on_pushResetPID_clicked() {
    m_usluga->resetPID();
}

void MainWindow::on_pushConfigARX_clicked() {
    ParametryARX okno(this);
    // Wyjątek: ModelARX przesyłamy jako obiekt, bo to okno konfiguracyjne
    okno.ustawAktualne(m_usluga->pobierzModel());
    connect(&okno, &ParametryARX::zglosNoweParametry, this, &MainWindow::odbierzParametryARX);
    okno.exec();
}

void MainWindow::odbierzParametryARX(std::vector<double> a, std::vector<double> b, int k, double szum, double umin, double umax, double ymin, double ymax) {
    m_usluga->setModelARX(a, b, k, szum, umin, umax, ymin, ymax);
}

void MainWindow::on_pushSaveConfig_clicked() {
    QString f = QFileDialog::getSaveFileName(this, "Zapisz", "", "JSON (*.json)");
    if(f.isEmpty()) return;
    QFile file(f);
    if(file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(m_usluga->toJson()).toJson());
    }
}

void MainWindow::on_pushLoadConfig_clicked() {
    QString f = QFileDialog::getOpenFileName(this, "Wczytaj", "", "JSON (*.json)");
    if(f.isEmpty()) return;
    QFile file(f);
    if(file.open(QIODevice::ReadOnly)) {
        m_usluga->fromJson(QJsonDocument::fromJson(file.readAll()).object());
        odswiezGUI();
    }
}

void MainWindow::odswiezGUI() {
    QSignalBlocker b1(ui->spinAmplituda);
    QSignalBlocker b2(ui->spinOkres);
    QSignalBlocker b3(ui->spinInterwal);
    QSignalBlocker b4(ui->spinSkladowaStala);
    QSignalBlocker b5(ui->spinWypelnienie);
    QSignalBlocker b6(ui->spinPidKp);
    QSignalBlocker b7(ui->spinPidTi);
    QSignalBlocker b8(ui->spinPidTd);
    QSignalBlocker b9(ui->comboTypSygnalu);
    QSignalBlocker b10(ui->comboMetCalk);

    ui->comboTypSygnalu->setCurrentIndex(m_usluga->getGenTyp());
    ui->spinAmplituda->setValue(m_usluga->getGenAmplituda());
    ui->spinOkres->setValue(m_usluga->getGenOkres());
    ui->spinInterwal->setValue(m_usluga->getGenInterwal());
    ui->spinSkladowaStala->setValue(m_usluga->getGenSkladowa());
    ui->spinWypelnienie->setValue(m_usluga->getGenWypelnienie());

    ui->spinPidKp->setValue(m_usluga->getPidKp());
    ui->spinPidTi->setValue(m_usluga->getPidTi());
    ui->spinPidTd->setValue(m_usluga->getPidTd());
    ui->comboMetCalk->setCurrentIndex(m_usluga->getPidMetodaCalkowania());

    aktualizujParametryGeneratora();
}

// GENERATOR
void MainWindow::on_spinAmplituda_editingFinished() {
    aktualizujParametryGeneratora();
}
void MainWindow::on_spinOkres_editingFinished() {
    aktualizujParametryGeneratora();
}
void MainWindow::on_spinSkladowaStala_editingFinished() {
    aktualizujParametryGeneratora();
}
void MainWindow::on_spinWypelnienie_editingFinished() {
    aktualizujParametryGeneratora();
}
void MainWindow::on_spinInterwal_editingFinished() {
    // Tutaj masz specyficzną logikę dla interwału
    m_usluga->setInterwal(ui->spinInterwal->value());
    aktualizujParametryGeneratora();
}
void MainWindow::on_comboTypSygnalu_currentIndexChanged(int index) {
    aktualizujParametryGeneratora();
}

// PID
void MainWindow::on_spinPidKp_editingFinished() {
    aktualizujParametryPID();
}
void MainWindow::on_spinPidTi_editingFinished() {
    aktualizujParametryPID();
}
void MainWindow::on_spinPidTd_editingFinished() {
    aktualizujParametryPID();
}
void MainWindow::on_comboMetCalk_currentIndexChanged(int index) {
    aktualizujParametryPID();
}

void MainWindow::on_spinOknoObserwacji_editingFinished()
{
    // 1. Pobierz nową wartość z GUI
    double noweOkno = ui->spinOknoObserwacji->value();

    // Zabezpieczenie przed dziwnymi wartościami (choć spinBox ma swoje limity)
    if (noweOkno < 1.0) noweOkno = 1.0;

    // 2. Zaktualizuj zmienną klasy
    m_oknoCzasowe = noweOkno;

    // 3. Opcjonalnie: Popraw siatkę (grid) na głównym wykresie
    // Żeby podziałka była ładna (np. co 1s dla małych czasów), dostosowujemy TickCount
    if (!m_chartOutput->axes(Qt::Horizontal).isEmpty()) {
        auto axisX = static_cast<QValueAxis*>(m_chartOutput->axes(Qt::Horizontal).first());

        if (m_oknoCzasowe <= 20.0) {
            // Dla krótkich czasów: podziałka co 1s (np. 10s -> 11 kresek)
            axisX->setTickCount(static_cast<int>(m_oknoCzasowe) + 1);
        } else {
            // Dla długich czasów: automat lub stała liczba, żeby nie zamazać osi
            axisX->setTickCount(11);
        }
    }

    // 4. Wymuś natychmiastowe przerysowanie wykresów z nową skalą
    double t = m_usluga->getCzas();
    zarzadzajWykresem(m_chartOutput, t);
    zarzadzajWykresem(m_chartError, t);
    zarzadzajWykresem(m_chartControl, t);
    zarzadzajWykresem(m_chartPID, t);
}

