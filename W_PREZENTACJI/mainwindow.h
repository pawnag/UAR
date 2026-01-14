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
    // Obsługa przycisków
    void on_pushStart_clicked();
    void on_pushStop_clicked();
    void on_pushResetSym_clicked();
    void on_pushConfigARX_clicked();

    // Logika i aktualizacja
    void aktualizujSymulacje();
    void aktualizujParametryGeneratora();
    void aktualizujParametryPID();

    // JSON
    void zapiszKonfiguracje();
    void wczytajKonfiguracje();

    void on_pushSaveConfig_clicked();
    void on_pushLoadConfig_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *m_timerSymulacji;
    KlasaUslugowa m_logika;

    // --- WYKRESY (Zredukowana liczba wskaźników) ---
    // 1. Wykres Główny (Zadana + Wyjście)
    QChart *m_chartOutput;
    QLineSeries *m_seriesZadana;
    QLineSeries *m_seriesWyjscie;

    // 2. Wykres Uchybu
    QChart *m_chartError;
    QLineSeries *m_seriesUchyb;

    // 3. Wykres Sterowania
    QChart *m_chartControl;
    QLineSeries *m_seriesSterowanie;

    // 4. Wykres PID
    QChart *m_chartPID;
    QLineSeries *m_seriesP;
    QLineSeries *m_seriesI;
    QLineSeries *m_seriesD;

    // --- HELPERY (DRY & Composition) ---
    // Tworzy wykres i widok, zwraca parę wskaźników
    std::pair<QChart*, QChartView*> stworzWykres(QString tytul, QString osY);

    // Dodaje serię danych do wykresu i zwraca wskaźnik do niej
    QLineSeries* dodajSerie(QChart* chart, QString nazwa, QColor kolor);

    // Odpowiada za przesuwanie okna czasowego (scroll) i autoskalowanie Y
    void zarzadzajWykresem(QChart* chart, double t);

    // Pomocnicza do odświeżania kontrolek GUI po wczytaniu pliku/resecie
    void odswiezGUI();
};

#endif // MAINWINDOW_H
