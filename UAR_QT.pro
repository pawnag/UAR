# --- CZYSTA I POPRAWNA KONFIGURACJA ---

# To jest linia, która działa w tamtym drugim projekcie:
QT       += core gui widgets charts

CONFIG   += c++17

# Usuwamy ręczne hacki (INCLUDEPATH do QtCharts), bo w działającym projekcie ich nie ma!
# System sam powinien to znaleźć, skoro działa w TestWykresow.

# Ścieżki do Twoich folderów z kodem
INCLUDEPATH += W_DANYCH W_USLUG W_PREZENTACJI W_TESTY

# Lista Twoich plików
SOURCES += \
    W_DANYCH/GeneratorWartosciZadanej.cpp \
    W_DANYCH/ModelARX.cpp \
    W_DANYCH/RegulatorPID.cpp \
    W_PREZENTACJI/ParametryARX.cpp \
    W_PREZENTACJI/mainwindow.cpp \
    W_TESTY/TestyDodatkowe.cpp \
    W_TESTY/Testy_UAR.cpp \
    W_USLUG/KlasaUslugowa.cpp \
    W_USLUG/ProstyUAR.cpp \
    W_USLUG/Symulacja.cpp \
    main.cpp

HEADERS += \
    W_DANYCH/GeneratorWartosciZadanej.h \
    W_DANYCH/ModelARX.h \
    W_DANYCH/RegulatorPID.h \
    W_PREZENTACJI/ParametryARX.h \
    W_PREZENTACJI/mainwindow.h \
    W_TESTY/TestyDodatkowe.h \
    W_USLUG/KlasaUslugowa.h \
    W_USLUG/ProstyUAR.h \
    W_USLUG/Symulacja.h

FORMS += \
    W_PREZENTACJI/ParametryARX.ui \
    W_PREZENTACJI/mainwindow.ui

# Standardowe reguły
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
