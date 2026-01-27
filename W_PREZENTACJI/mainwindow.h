#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include "W_USLUG/KlasaUslugowa.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, KlasaUslugowa *usluga = nullptr);
    ~MainWindow();

private slots:
    void on_pushStart_clicked();
    void on_pushStop_clicked();
    void on_pushResetSym_clicked();

    void aktualizujWykresy(double czas, double zadana, double wyjscie, double ster, double uchyb);

    void aktualizujParametryGeneratora();
    void aktualizujParametryPID();
    void on_spinInterwal_editingFinished();

    void on_pushConfigARX_clicked();
    void odbierzParametryARX(std::vector<double> a, std::vector<double> b, int k, double szum, double umin, double umax, double ymin, double ymax);
    void on_pushSaveConfig_clicked();
    void on_pushLoadConfig_clicked();
    void on_pushResetPID_clicked();

    void on_spinOknoObserwacji_editingFinished();

    void on_spinAmplituda_editingFinished();

    void on_spinOkres_editingFinished();

    void on_spinSkladowaStala_editingFinished();

    void on_spinWypelnienie_editingFinished();

    void on_spinPidKp_editingFinished();

    void on_spinPidTi_editingFinished();

    void on_spinPidTd_editingFinished();

    void on_comboTypSygnalu_currentIndexChanged(int index);

    void on_comboMetCalk_currentIndexChanged(int index);

    void resetSymulacji();

    void on_checkBox_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    KlasaUslugowa *m_usluga;
    double m_oknoCzasowe;

    QChart *m_chartOutput, *m_chartError, *m_chartControl, *m_chartPID;
    QLineSeries *m_seriesZadana, *m_seriesWyjscie;
    QLineSeries *m_seriesUchyb, *m_seriesSterowanie;
    QLineSeries *m_seriesP, *m_seriesI, *m_seriesD;

    std::pair<QChart*, QChartView*> stworzWykres(QString tytul, QString osY);
    QLineSeries* dodajSerie(QChart* chart, QString nazwa, QColor kolor);
    void zarzadzajWykresem(QChart* chart, double t);
    void odswiezGUI();
};

#endif // MAINWINDOW_H
