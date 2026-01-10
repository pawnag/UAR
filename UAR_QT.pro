QT       += core gui
QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Dodajemy foldery do ścieżek, żeby #include "modelarx.h" działało wszędzie
INCLUDEPATH += W_DANYCH W_USLUG W_PREZENTACJI W_TESTY

CONFIG += console
CONFIG += c++17

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    W_DANYCH/GeneratorWartosciZadanej.cpp \
    W_DANYCH/ModelARX.cpp \
    W_DANYCH/RegulatorPID.cpp \
    W_PREZENTACJI/mainwindow.cpp \
    W_TESTY/TestyDodatkowe.cpp \
    W_TESTY/Testy_UAR.cpp \
    W_USLUG/KlasaUslugowa.cpp \
    W_USLUG/ProstyUAR.cpp \
    W_USLUG/Symulacja.cpp \
    main.cpp
    # USUNIĘTO: mainwindow.cpp (bo jest już wyżej w W_PREZENTACJI)

HEADERS += \
    W_DANYCH/GeneratorWartosciZadanej.h \
    W_DANYCH/ModelARX.h \
    W_DANYCH/RegulatorPID.h \
    W_PREZENTACJI/mainwindow.h \
    W_TESTY/TestyDodatkowe.h \
    W_USLUG/KlasaUslugowa.h \
    W_USLUG/ProstyUAR.h \
    W_USLUG/Symulacja.h
    # USUNIĘTO: mainwindow.h (dubel)
    # USUNIĘTO: Testy_UAR.h (pisałeś, że go nie masz)

FORMS += \
    W_PREZENTACJI/mainwindow.ui
    # USUNIĘTO: mainwindow.ui (dubel)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
msvc: QMAKE_CXXFLAGS += /FS
