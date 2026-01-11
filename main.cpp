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
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
#endif
}
