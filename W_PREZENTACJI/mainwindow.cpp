#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ParametryARX.h"
#include <QSignalBlocker>
#include <QFileDialog>
#include <QLineEdit>
#include <QDebug>
#include <QJsonDocument>
#include <QFile>

static constexpr double DOMYSLNE_OKNO_CZASOWE = 10.0;
static constexpr double MARGINES_Y = 0.1;

MainWindow::MainWindow(QWidget *parent, KlasaUslugowa *usluga)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_usluga(usluga)
    , m_oknoCzasowe(DOMYSLNE_OKNO_CZASOWE)
{
    ui->setupUi(this);

    if (!m_usluga) {
        m_usluga = new KlasaUslugowa(this);
    }

    QVBoxLayout* layoutMain = new QVBoxLayout(ui->widgetWykresy);
    layoutMain->setContentsMargins(0, 0, 0, 0);

    auto paraGlowna = stworzWykres("Wartość zadana i regulowana", "Odpowiedź układu");
    m_chartOutput = paraGlowna.first;
    layoutMain->addWidget(paraGlowna.second, 1);

    if (!m_chartOutput->axes(Qt::Horizontal).isEmpty()) {
        auto axisX = static_cast<QValueAxis*>(m_chartOutput->axes(Qt::Horizontal).first());
        axisX->setTickCount(11);
    }

    m_seriesZadana = dodajSerie(m_chartOutput, "Wartość zadana (w)", Qt::red);
    m_seriesWyjscie = dodajSerie(m_chartOutput, "Wartość regulowana (y)", QColor(0, 150, 255));

    QHBoxLayout* layoutDolny = new QHBoxLayout();
    layoutMain->addLayout(layoutDolny, 1);

    auto paraUchyb = stworzWykres("Uchyb regulacji", "Uchyb");
    m_chartError = paraUchyb.first;
    layoutDolny->addWidget(paraUchyb.second);
    m_seriesUchyb = dodajSerie(m_chartError, "Uchyb", Qt::green);

    auto paraSter = stworzWykres("Sygnał sterujący", "Sterowanie");
    m_chartControl = paraSter.first;
    layoutDolny->addWidget(paraSter.second);
    m_seriesSterowanie = dodajSerie(m_chartControl, "Sterowanie", Qt::magenta);

    auto paraPID = stworzWykres("Składowe sterowania", "Wartość PID");
    m_chartPID = paraPID.first;
    layoutDolny->addWidget(paraPID.second);
    m_seriesP = dodajSerie(m_chartPID, "P", Qt::cyan);
    m_seriesI = dodajSerie(m_chartPID, "I", Qt::yellow);
    m_seriesD = dodajSerie(m_chartPID, "D", QColor(255, 100, 255));

    connect(m_usluga, &KlasaUslugowa::noweDane, this, &MainWindow::aktualizujWykresy);

    ui->spinOknoObserwacji->setValue(m_oknoCzasowe);
    odswiezGUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::pair<QChart*, QChartView*> MainWindow::stworzWykres(QString tytul, QString osY) {
    QChart* chart = new QChart();
    chart->setTitle(tytul);

    chart->setBackgroundBrush(QBrush(QColor(30, 30, 30)));
    chart->setTitleBrush(Qt::white);
    chart->legend()->setAlignment(Qt::AlignTop);
    chart->legend()->setLabelBrush(Qt::white);

    QPen axisPen(Qt::white);
    axisPen.setWidth(1);

    // Oś X
    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("Czas [s]");
    axisX->setLabelsBrush(Qt::white);
    axisX->setTitleBrush(Qt::white);
    axisX->setGridLineColor(QColor(80, 80, 80));
    axisX->setLinePen(axisPen);
    axisX->setRange(0, m_oknoCzasowe); // Startowy zakres
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

void MainWindow::zarzadzajWykresem(QChart* chart, double t) {
    if (!chart) return;
    if (chart->axes().isEmpty()) return;
    // Oś X
    double minX = 0.0;
    double maxX = 10.0; // Wartość tymczasowa
    if (t > m_oknoCzasowe) { // Efekt płynnego wykresu
        minX = t - m_oknoCzasowe;
        maxX = t;
    } else {
        minX = 0.0;
        maxX = m_oknoCzasowe;
    }
    // Ustawiamy zakres na osi poziomej
    chart->axes(Qt::Horizontal).first()->setRange(minX, maxX);

    // Oś Y
    double yMin = 1000.0;
    double yMax = -1000.0;
    bool saDane = false;
    double czasDoUsuniecia = t - 200.0; // Bufor potrzebny żeby dało się zmieniać okno czasowe

    for (auto series : chart->series()) {
        auto linia = static_cast<QLineSeries*>(series);

        // Usuwamy stare punkty
        if (linia->count() > 0) {
            if (linia->at(0).x() < czasDoUsuniecia) {
                linia->remove(0);
            }
        }
        // Szukamy po punktach które są widoczne
        for (auto p : linia->points()) {
            if (p.x() >= minX) {
                if (p.y() < yMin) {
                    yMin = p.y();
                }
                if (p.y() > yMax) {
                    yMax = p.y();
                }
                saDane = true;
            }
        }
    }
    if (saDane) {
        double roznica = yMax - yMin;
        if (roznica < 0.1) { roznica = 1.0; }
        double margines = roznica * 0.1; // 10% maginesu góra dół
        chart->axes(Qt::Vertical).first()->setRange(yMin - margines, yMax + margines);
    }
}

void MainWindow::aktualizujWykresy(double czas, double zadana, double wyjscie, double sterowanie, double uchyb) {

    double valP = m_usluga->getPidLastP();
    double valI = m_usluga->getPidLastI();
    double valD = m_usluga->getPidLastD();

    // Dodajemy punkty
    m_seriesZadana->append(czas, zadana);
    m_seriesWyjscie->append(czas, wyjscie);
    m_seriesUchyb->append(czas, uchyb);
    m_seriesSterowanie->append(czas, sterowanie);

    m_seriesP->append(czas, valP);
    m_seriesI->append(czas, valI);
    m_seriesD->append(czas, valD);

    // Aktualizacja legendy (wartości bieżące)
    m_seriesZadana->setName(QString("Zadana: %1").arg(zadana, 0, 'f', 2));
    m_seriesWyjscie->setName(QString("Wyjście: %1").arg(wyjscie, 0, 'f', 2));
    m_seriesUchyb->setName(QString("Uchyb: %1").arg(uchyb, 0, 'f', 4));
    m_seriesSterowanie->setName(QString("Ster: %1").arg(sterowanie, 0, 'f', 2));

    // Odświeżenie widoków
    zarzadzajWykresem(m_chartOutput, czas);
    zarzadzajWykresem(m_chartError, czas);
    zarzadzajWykresem(m_chartControl, czas);
    zarzadzajWykresem(m_chartPID, czas);
}

void MainWindow::aktualizujParametryGeneratora() {
    auto typ = static_cast<int>(ui->comboTypSygnalu->currentIndex());
    bool isConstant = (typ == 0);

    ui->spinOkres->setEnabled(!isConstant);
    ui->spinAmplituda->setEnabled(!isConstant);

    ui->spinWypelnienie->setEnabled(typ == 1);

    m_usluga->ustawGenerator(
        ui->spinAmplituda->value(),
        ui->spinOkres->value(),
        ui->spinInterwal->value(),
        typ,
        ui->spinSkladowaStala->value(),
        ui->spinWypelnienie->value()
        );
}

void MainWindow::aktualizujParametryPID() {
    m_usluga->ustawPID(
        ui->spinPidKp->value(),
        ui->spinPidTi->value(),
        ui->spinPidTd->value(),
        ui->comboMetCalk->currentIndex()
        );
}

void MainWindow::on_pushStart_clicked() { m_usluga->start(); }
void MainWindow::on_pushStop_clicked() { m_usluga->stop(); }

void MainWindow::on_pushResetSym_clicked() {
    m_usluga->reset();

    auto wyczyscWykres = [](QChart* c, double okno) {
        for(auto s : c->series()) static_cast<QLineSeries*>(s)->clear();
        if(!c->axes(Qt::Horizontal).isEmpty())
            c->axes(Qt::Horizontal).first()->setRange(0, okno);
    };

    wyczyscWykres(m_chartOutput, m_oknoCzasowe);
    wyczyscWykres(m_chartError, m_oknoCzasowe);
    wyczyscWykres(m_chartControl, m_oknoCzasowe);
    wyczyscWykres(m_chartPID, m_oknoCzasowe);
}

void MainWindow::on_pushResetPID_clicked() {
    // Implementacja resetu samego PID w usłudze (opcjonalna)
    // m_usluga->resetPID();
}

void MainWindow::on_pushConfigARX_clicked() {
    ParametryARX okno(this);
    std::vector<double> A, B;
    int opoznienie;
    double szum, umin, umax, ymin, ymax;

    m_usluga->pobierzModel(A, B, opoznienie, szum, umin, umax, ymin, ymax);
    okno.ustawDane(A, B, opoznienie, szum, umin, umax, ymin, ymax);
    connect(&okno, &ParametryARX::zglosNoweParametry, this, &MainWindow::odbierzParametryARX);
    okno.exec();
}

void MainWindow::odbierzParametryARX(std::vector<double> a, std::vector<double> b, int k, double szum, double umin, double umax, double ymin, double ymax) {
    m_usluga->ustawModel(a, b, k, szum, umin, umax, ymin, ymax);
}

void MainWindow::on_pushSaveConfig_clicked() {
    QString f = QFileDialog::getSaveFileName(this, "Zapisz konfigurację", "", "JSON (*.json)");
    if(!f.isEmpty()) {

        QFile file(f);
        if (!file.open(QIODevice::WriteOnly)) return;

        QJsonDocument doc(m_usluga->toJson());
        file.write(doc.toJson());
        file.close();
    }
}


void MainWindow::on_pushLoadConfig_clicked() {
    QString f = QFileDialog::getOpenFileName(this, "Wczytaj konfigurację", "", "JSON (*.json)");
    if(!f.isEmpty()) {

        QFile file(f);
        if (!file.open(QIODevice::ReadOnly)) return;

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (!doc.isObject()) return;

        m_usluga->fromJson(doc.object());
        odswiezGUI();
    }
}



void MainWindow::odswiezGUI() {
    const QList<QWidget*> widgets = {
        ui->spinAmplituda, ui->spinOkres, ui->spinInterwal, ui->spinSkladowaStala,
        ui->spinWypelnienie, ui->spinPidKp, ui->spinPidTi, ui->spinPidTd,
        ui->comboTypSygnalu, ui->comboMetCalk
    };

    for(auto w : widgets) w->blockSignals(true);

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

    int typ = ui->comboTypSygnalu->currentIndex();
    bool isConstant = (typ == 0); // Zakładam stały
    ui->spinOkres->setEnabled(!isConstant);
    ui->spinAmplituda->setEnabled(!isConstant);
    ui->spinWypelnienie->setEnabled(typ == 1); // Zakładam Prostokąt

    for(auto w : widgets) w->blockSignals(false);
}

void MainWindow::on_spinAmplituda_editingFinished() { aktualizujParametryGeneratora(); }
void MainWindow::on_spinOkres_editingFinished() { aktualizujParametryGeneratora(); }
void MainWindow::on_spinSkladowaStala_editingFinished() { aktualizujParametryGeneratora(); }
void MainWindow::on_spinWypelnienie_editingFinished() { aktualizujParametryGeneratora(); }
void MainWindow::on_spinInterwal_editingFinished() {
    m_usluga->setInterwal(ui->spinInterwal->value());
    aktualizujParametryGeneratora();
}
void MainWindow::on_comboTypSygnalu_currentIndexChanged(int index) { aktualizujParametryGeneratora(); }

void MainWindow::on_spinPidKp_editingFinished() { aktualizujParametryPID(); }
void MainWindow::on_spinPidTi_editingFinished() { aktualizujParametryPID(); }
void MainWindow::on_spinPidTd_editingFinished() { aktualizujParametryPID(); }
void MainWindow::on_comboMetCalk_currentIndexChanged(int index) { aktualizujParametryPID(); }

void MainWindow::on_spinOknoObserwacji_editingFinished()
{
    double noweOkno = ui->spinOknoObserwacji->value();
    if (noweOkno < 1.0) noweOkno = 1.0;
    m_oknoCzasowe = noweOkno;

    if (!m_chartOutput->axes(Qt::Horizontal).isEmpty()) {
        auto axisX = static_cast<QValueAxis*>(m_chartOutput->axes(Qt::Horizontal).first());
        axisX->setRange(0, m_oknoCzasowe);
        if (m_oknoCzasowe <= 20.0) {
            axisX->setTickCount(static_cast<int>(m_oknoCzasowe) + 1);
        } else {
            axisX->setTickCount(11);
        }
    }

    double t = 0.0;
}
