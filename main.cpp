#include "W_PREZENTACJI/mainwindow.h"
#include <QApplication>

//#define TRYB_TESTOWY

extern void uruchomWszystkieTesty();

int main(int argc, char *argv[])
{
#ifdef TRYB_TESTOWY
    uruchomWszystkieTesty();
    return 0;
#else
    uruchomWszystkieTesty();
    QApplication a(argc, argv);

    // 1. Warstwa Usług (Logika) - tworzona na stosie, niezależna
    KlasaUslugowa usluga;

    // 2. Warstwa Prezentacji (GUI)
    // Wstrzykujemy wskaźnik do usługi.
    // NIE ustawiamy rodzica (parent), żeby zachować luźne powiązanie.
    MainWindow w(nullptr, &usluga);

    w.show();

    return a.exec();
#endif
}
