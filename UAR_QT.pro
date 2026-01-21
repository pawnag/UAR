QT       += core gui widgets charts

CONFIG   += c++17

INCLUDEPATH += W_DANYCH W_USLUG W_PREZENTACJI W_TESTY

SOURCES += \
    W_DANYCH/GeneratorWartosciZadanej.cpp \
    W_DANYCH/ModelARX.cpp \
    W_DANYCH/RegulatorPID.cpp \
    W_PREZENTACJI/ParametryARX.cpp \
    W_PREZENTACJI/mainwindow.cpp \
    W_TESTY/TestyDodatkowe.cpp \
    W_TESTY/Testy_UAR.cpp \
    W_USLUG/KlasaUslugowa.cpp \
    W_DANYCH/ProstyUAR.cpp \
    W_DANYCH/Symulacja.cpp \
    main.cpp

HEADERS += \
    W_DANYCH/GeneratorWartosciZadanej.h \
    W_DANYCH/ModelARX.h \
    W_DANYCH/RegulatorPID.h \
    W_PREZENTACJI/ParametryARX.h \
    W_PREZENTACJI/mainwindow.h \
    W_TESTY/Testy.h \
    W_TESTY/TestyDodatkowe.h \
    W_USLUG/KlasaUslugowa.h \
    W_DANYCH/ProstyUAR.h \
    W_DANYCH/Symulacja.h

FORMS += \
    W_PREZENTACJI/ParametryARX.ui \
    W_PREZENTACJI/mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
