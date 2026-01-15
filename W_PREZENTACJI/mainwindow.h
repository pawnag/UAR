#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtCharts>
#include <vector>
#include <utility> // dla std::pair
#include "W_USLUG/KlasaUslugowa.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushStart_clicked();
    void on_pushStop_clicked();
    void on_pushResetSym_clicked();
    void on_pushConfigARX_clicked();

    void aktualizujSymulacje();
    void aktualizujParametryGeneratora();
    void aktualizujParametryPID();

    void odbierzParametryARX(std::vector<double> a, std::vector<double> b,
                             int k, double szum,
                             double umin, double umax, double ymin, double ymax);

    void zapiszKonfiguracje();
    void wczytajKonfiguracje();

    void on_pushSaveConfig_clicked();
    void on_pushLoadConfig_clicked();

    void on_spinAmplituda_valueChanged(double arg1);

    void on_comboTypSygnalu_currentIndexChanged(int index);

    void on_spinOkres_valueChanged(double arg1);

    void on_spinSkladowaStala_valueChanged(double arg1);

    void on_spinWypelnienie_valueChanged(double arg1);

    void on_spinPidKp_valueChanged(double arg1);

    void on_spinPidTi_valueChanged(double arg1);

    void on_spinPidTd_valueChanged(double arg1);

    void on_comboMetCalk_currentIndexChanged(int index);

    void on_pushResetPID_clicked();

    void on_spinInterwal_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QTimer *m_timerSymulacji;
    KlasaUslugowa m_logika;

    QChart *m_chartOutput;
    QLineSeries *m_seriesZadana;
    QLineSeries *m_seriesWyjscie;

    QChart *m_chartError;
    QLineSeries *m_seriesUchyb;

    QChart *m_chartControl;
    QLineSeries *m_seriesSterowanie;

    QChart *m_chartPID;
    QLineSeries *m_seriesP;
    QLineSeries *m_seriesI;
    QLineSeries *m_seriesD;

    std::pair<QChart*, QChartView*> stworzWykres(QString tytul, QString osY);
    QLineSeries* dodajSerie(QChart* chart, QString nazwa, QColor kolor);
    void zarzadzajWykresem(QChart* chart, double t);
    void odswiezGUI();
};

#endif // MAINWINDOW_H
