#include "W_PREZENTACJI/mainwindow.h"
#include <QApplication>
#include <iostream>
//#define TRYB_TESTOWY // <--- Odkomentuj to tylko jak chcesz testy

extern void uruchomWszystkieTesty();

int main(int argc, char *argv[]) {
#ifdef TRYB_TESTOWY
    uruchomWszystkieTesty();
    return 0;
#else
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
#endif
}
