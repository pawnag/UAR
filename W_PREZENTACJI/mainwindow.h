#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtCharts>
#include "W_USLUG/KlasaUslugowa.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFileDialog>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushStart_clicked();
    void on_pushStop_clicked();
    void on_pushResetSym_clicked();

    void aktualizujSymulacje();
    void aktualizujParametryGeneratora();
    void aktualizujParametryPID();

    void on_pushConfigARX_clicked();

    void on_pushSaveConfig_clicked();
    void on_pushLoadConfig_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *m_timerSymulacji;

    KlasaUslugowa m_logika;

    // --- Wykres 1: Regulacja ---
    QLineSeries *m_seriesZadana;
    QLineSeries *m_seriesWyjscie;
    QChart *m_chartOutput;
    QChartView *m_chartViewOutput;

    // --- Wykres 2: Uchyb ---
    QLineSeries *m_seriesUchyb;
    QChart *m_chartError;
    QChartView *m_chartViewError;

    // --- Wykres 3: Sterowanie ---
    QLineSeries *m_seriesSterowanie;
    QChart *m_chartControl;
    QChartView *m_chartViewControl;

    // --- Wykres 4: Składowe PID ---
    QLineSeries *m_seriesP;
    QLineSeries *m_seriesI;
    QLineSeries *m_seriesD;
    QChart *m_chartPID;
    QChartView *m_chartViewPID;

    // Metody pomocnicze
    void setupChart(QLayout* layout, QChart*& chart, QChartView*& view);
    void stylizujWykres(QChart* chart, bool pokazLegende);

    //JSON
    void zapiszKonfiguracje();
    void wczytajKonfiguracje();

    ModelARX m_model;
    RegulatorPID m_regulator;
    GeneratorWartosciZadanej m_generator;
    Symulacja m_symulacja;


};

#endif // MAINWINDOW_H
