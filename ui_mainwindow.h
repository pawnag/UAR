/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_Main;
    QWidget *widgetWykresy;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_Controls;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox_Sim;
    QGridLayout *gridLayout_Sim;
    QSpinBox *spinInterwal;
    QPushButton *pushStop;
    QPushButton *pushStart;
    QLabel *labelInterwal;
    QPushButton *pushResetSym;
    QLabel *label;
    QSpinBox *spinOknoObserwacji;
    QGroupBox *groupBox_Gen;
    QFormLayout *formLayout_Gen;
    QLabel *labelTypSyg;
    QComboBox *comboTypSygnalu;
    QLabel *labelAmplituda;
    QDoubleSpinBox *spinAmplituda;
    QLabel *labelOkres;
    QDoubleSpinBox *spinOkres;
    QLabel *labelSkladowaStala;
    QDoubleSpinBox *spinSkladowaStala;
    QLabel *labelWypelnienie;
    QDoubleSpinBox *spinWypelnienie;
    QGroupBox *groupBox_PID;
    QFormLayout *formLayout_PID;
    QLabel *labelKp;
    QDoubleSpinBox *spinPidKp;
    QLabel *labelTi;
    QDoubleSpinBox *spinPidTi;
    QLabel *labelTd;
    QDoubleSpinBox *spinPidTd;
    QLabel *labelMetCalk;
    QComboBox *comboMetCalk;
    QPushButton *pushResetPID;
    QGroupBox *groupBox_ARX;
    QVBoxLayout *verticalLayout_ARX;
    QPushButton *pushConfigARX;
    QGroupBox *groupBox_File;
    QHBoxLayout *horizontalLayout_File;
    QPushButton *pushSaveConfig;
    QPushButton *pushLoadConfig;
    QSpacerItem *verticalSpacer;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 800);
        MainWindow->setMinimumSize(QSize(1200, 800));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout_Main = new QHBoxLayout(centralwidget);
        horizontalLayout_Main->setObjectName("horizontalLayout_Main");
        widgetWykresy = new QWidget(centralwidget);
        widgetWykresy->setObjectName("widgetWykresy");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widgetWykresy->sizePolicy().hasHeightForWidth());
        widgetWykresy->setSizePolicy(sizePolicy);
        widgetWykresy->setStyleSheet(QString::fromUtf8("background-color: rgb(45, 45, 45);"));

        horizontalLayout_Main->addWidget(widgetWykresy);

        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setMinimumSize(QSize(280, 0));
        scrollArea->setMaximumSize(QSize(320, 16777215));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 278, 764));
        verticalLayout_Controls = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_Controls->setObjectName("verticalLayout_Controls");
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_Controls->addItem(verticalSpacer_2);

        groupBox_Sim = new QGroupBox(scrollAreaWidgetContents);
        groupBox_Sim->setObjectName("groupBox_Sim");
        gridLayout_Sim = new QGridLayout(groupBox_Sim);
        gridLayout_Sim->setObjectName("gridLayout_Sim");
        spinInterwal = new QSpinBox(groupBox_Sim);
        spinInterwal->setObjectName("spinInterwal");
        spinInterwal->setMinimum(10);
        spinInterwal->setMaximum(1000);
        spinInterwal->setValue(200);

        gridLayout_Sim->addWidget(spinInterwal, 2, 1, 1, 1);

        pushStop = new QPushButton(groupBox_Sim);
        pushStop->setObjectName("pushStop");

        gridLayout_Sim->addWidget(pushStop, 0, 1, 1, 1);

        pushStart = new QPushButton(groupBox_Sim);
        pushStart->setObjectName("pushStart");

        gridLayout_Sim->addWidget(pushStart, 0, 0, 1, 1);

        labelInterwal = new QLabel(groupBox_Sim);
        labelInterwal->setObjectName("labelInterwal");

        gridLayout_Sim->addWidget(labelInterwal, 2, 0, 1, 1);

        pushResetSym = new QPushButton(groupBox_Sim);
        pushResetSym->setObjectName("pushResetSym");

        gridLayout_Sim->addWidget(pushResetSym, 1, 0, 1, 2);

        label = new QLabel(groupBox_Sim);
        label->setObjectName("label");

        gridLayout_Sim->addWidget(label, 3, 0, 1, 1);

        spinOknoObserwacji = new QSpinBox(groupBox_Sim);
        spinOknoObserwacji->setObjectName("spinOknoObserwacji");
        spinOknoObserwacji->setMinimum(10);
        spinOknoObserwacji->setMaximum(100);

        gridLayout_Sim->addWidget(spinOknoObserwacji, 3, 1, 1, 1);


        verticalLayout_Controls->addWidget(groupBox_Sim);

        groupBox_Gen = new QGroupBox(scrollAreaWidgetContents);
        groupBox_Gen->setObjectName("groupBox_Gen");
        formLayout_Gen = new QFormLayout(groupBox_Gen);
        formLayout_Gen->setObjectName("formLayout_Gen");
        labelTypSyg = new QLabel(groupBox_Gen);
        labelTypSyg->setObjectName("labelTypSyg");

        formLayout_Gen->setWidget(0, QFormLayout::LabelRole, labelTypSyg);

        comboTypSygnalu = new QComboBox(groupBox_Gen);
        comboTypSygnalu->addItem(QString());
        comboTypSygnalu->addItem(QString());
        comboTypSygnalu->addItem(QString());
        comboTypSygnalu->setObjectName("comboTypSygnalu");

        formLayout_Gen->setWidget(0, QFormLayout::FieldRole, comboTypSygnalu);

        labelAmplituda = new QLabel(groupBox_Gen);
        labelAmplituda->setObjectName("labelAmplituda");

        formLayout_Gen->setWidget(1, QFormLayout::LabelRole, labelAmplituda);

        spinAmplituda = new QDoubleSpinBox(groupBox_Gen);
        spinAmplituda->setObjectName("spinAmplituda");
        spinAmplituda->setMaximum(1000.000000000000000);
        spinAmplituda->setSingleStep(0.100000000000000);

        formLayout_Gen->setWidget(1, QFormLayout::FieldRole, spinAmplituda);

        labelOkres = new QLabel(groupBox_Gen);
        labelOkres->setObjectName("labelOkres");

        formLayout_Gen->setWidget(2, QFormLayout::LabelRole, labelOkres);

        spinOkres = new QDoubleSpinBox(groupBox_Gen);
        spinOkres->setObjectName("spinOkres");
        spinOkres->setMinimum(0.100000000000000);
        spinOkres->setMaximum(10000.000000000000000);
        spinOkres->setValue(10.000000000000000);

        formLayout_Gen->setWidget(2, QFormLayout::FieldRole, spinOkres);

        labelSkladowaStala = new QLabel(groupBox_Gen);
        labelSkladowaStala->setObjectName("labelSkladowaStala");

        formLayout_Gen->setWidget(3, QFormLayout::LabelRole, labelSkladowaStala);

        spinSkladowaStala = new QDoubleSpinBox(groupBox_Gen);
        spinSkladowaStala->setObjectName("spinSkladowaStala");
        spinSkladowaStala->setMinimum(-1000.000000000000000);
        spinSkladowaStala->setMaximum(1000.000000000000000);

        formLayout_Gen->setWidget(3, QFormLayout::FieldRole, spinSkladowaStala);

        labelWypelnienie = new QLabel(groupBox_Gen);
        labelWypelnienie->setObjectName("labelWypelnienie");

        formLayout_Gen->setWidget(4, QFormLayout::LabelRole, labelWypelnienie);

        spinWypelnienie = new QDoubleSpinBox(groupBox_Gen);
        spinWypelnienie->setObjectName("spinWypelnienie");
        spinWypelnienie->setMaximum(1.000000000000000);
        spinWypelnienie->setSingleStep(0.100000000000000);
        spinWypelnienie->setValue(0.500000000000000);

        formLayout_Gen->setWidget(4, QFormLayout::FieldRole, spinWypelnienie);


        verticalLayout_Controls->addWidget(groupBox_Gen);

        groupBox_PID = new QGroupBox(scrollAreaWidgetContents);
        groupBox_PID->setObjectName("groupBox_PID");
        formLayout_PID = new QFormLayout(groupBox_PID);
        formLayout_PID->setObjectName("formLayout_PID");
        labelKp = new QLabel(groupBox_PID);
        labelKp->setObjectName("labelKp");

        formLayout_PID->setWidget(0, QFormLayout::LabelRole, labelKp);

        spinPidKp = new QDoubleSpinBox(groupBox_PID);
        spinPidKp->setObjectName("spinPidKp");
        spinPidKp->setMaximum(1000.000000000000000);
        spinPidKp->setSingleStep(0.100000000000000);
        spinPidKp->setValue(0.000000000000000);

        formLayout_PID->setWidget(0, QFormLayout::FieldRole, spinPidKp);

        labelTi = new QLabel(groupBox_PID);
        labelTi->setObjectName("labelTi");

        formLayout_PID->setWidget(1, QFormLayout::LabelRole, labelTi);

        spinPidTi = new QDoubleSpinBox(groupBox_PID);
        spinPidTi->setObjectName("spinPidTi");
        spinPidTi->setMaximum(10000.000000000000000);

        formLayout_PID->setWidget(1, QFormLayout::FieldRole, spinPidTi);

        labelTd = new QLabel(groupBox_PID);
        labelTd->setObjectName("labelTd");

        formLayout_PID->setWidget(2, QFormLayout::LabelRole, labelTd);

        spinPidTd = new QDoubleSpinBox(groupBox_PID);
        spinPidTd->setObjectName("spinPidTd");
        spinPidTd->setMaximum(10000.000000000000000);

        formLayout_PID->setWidget(2, QFormLayout::FieldRole, spinPidTd);

        labelMetCalk = new QLabel(groupBox_PID);
        labelMetCalk->setObjectName("labelMetCalk");

        formLayout_PID->setWidget(3, QFormLayout::LabelRole, labelMetCalk);

        comboMetCalk = new QComboBox(groupBox_PID);
        comboMetCalk->addItem(QString());
        comboMetCalk->addItem(QString());
        comboMetCalk->setObjectName("comboMetCalk");
        comboMetCalk->setEnabled(true);

        formLayout_PID->setWidget(3, QFormLayout::FieldRole, comboMetCalk);

        pushResetPID = new QPushButton(groupBox_PID);
        pushResetPID->setObjectName("pushResetPID");

        formLayout_PID->setWidget(4, QFormLayout::SpanningRole, pushResetPID);


        verticalLayout_Controls->addWidget(groupBox_PID);

        groupBox_ARX = new QGroupBox(scrollAreaWidgetContents);
        groupBox_ARX->setObjectName("groupBox_ARX");
        verticalLayout_ARX = new QVBoxLayout(groupBox_ARX);
        verticalLayout_ARX->setObjectName("verticalLayout_ARX");
        pushConfigARX = new QPushButton(groupBox_ARX);
        pushConfigARX->setObjectName("pushConfigARX");

        verticalLayout_ARX->addWidget(pushConfigARX);


        verticalLayout_Controls->addWidget(groupBox_ARX);

        groupBox_File = new QGroupBox(scrollAreaWidgetContents);
        groupBox_File->setObjectName("groupBox_File");
        horizontalLayout_File = new QHBoxLayout(groupBox_File);
        horizontalLayout_File->setObjectName("horizontalLayout_File");
        pushSaveConfig = new QPushButton(groupBox_File);
        pushSaveConfig->setObjectName("pushSaveConfig");

        horizontalLayout_File->addWidget(pushSaveConfig);

        pushLoadConfig = new QPushButton(groupBox_File);
        pushLoadConfig->setObjectName("pushLoadConfig");

        horizontalLayout_File->addWidget(pushLoadConfig);


        verticalLayout_Controls->addWidget(groupBox_File);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_Controls->addItem(verticalSpacer);

        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout_Main->addWidget(scrollArea);

        horizontalLayout_Main->setStretch(0, 1);
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Symulator Uk\305\202adu Automatycznej Regulacji", nullptr));
        groupBox_Sim->setTitle(QCoreApplication::translate("MainWindow", "Symulacja", nullptr));
        pushStop->setText(QCoreApplication::translate("MainWindow", "Stop", nullptr));
        pushStart->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        labelInterwal->setText(QCoreApplication::translate("MainWindow", "Interwa\305\202 [ms]:", nullptr));
        pushResetSym->setText(QCoreApplication::translate("MainWindow", "Pe\305\202ny Reset Symulacji", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Okno obserwacji [s]:", nullptr));
        groupBox_Gen->setTitle(QCoreApplication::translate("MainWindow", "Generator", nullptr));
        labelTypSyg->setText(QCoreApplication::translate("MainWindow", "Typ sygna\305\202u:", nullptr));
        comboTypSygnalu->setItemText(0, QCoreApplication::translate("MainWindow", "Sta\305\202a", nullptr));
        comboTypSygnalu->setItemText(1, QCoreApplication::translate("MainWindow", "Prostok\304\205t", nullptr));
        comboTypSygnalu->setItemText(2, QCoreApplication::translate("MainWindow", "Sinus", nullptr));

        labelAmplituda->setText(QCoreApplication::translate("MainWindow", "Amplituda:", nullptr));
        labelOkres->setText(QCoreApplication::translate("MainWindow", "Okres [s]:", nullptr));
        labelSkladowaStala->setText(QCoreApplication::translate("MainWindow", "Sk\305\202adowa sta\305\202a:", nullptr));
        labelWypelnienie->setText(QCoreApplication::translate("MainWindow", "Wype\305\202nienie [0-1]:", nullptr));
        groupBox_PID->setTitle(QCoreApplication::translate("MainWindow", "Regulator PID", nullptr));
        labelKp->setText(QCoreApplication::translate("MainWindow", "Wzmocnienie (Kp):", nullptr));
        labelTi->setText(QCoreApplication::translate("MainWindow", "Ca\305\202kowanie (Ti):", nullptr));
        labelTd->setText(QCoreApplication::translate("MainWindow", "R\303\263\305\274niczkowanie (Td):", nullptr));
        labelMetCalk->setText(QCoreApplication::translate("MainWindow", "Metoda ca\305\202kowania:", nullptr));
        comboMetCalk->setItemText(0, QCoreApplication::translate("MainWindow", "Sta\305\202a w sumie", nullptr));
        comboMetCalk->setItemText(1, QCoreApplication::translate("MainWindow", "Sta\305\202a przed sum\304\205", nullptr));

        pushResetPID->setText(QCoreApplication::translate("MainWindow", "Resetuj PID (ca\305\202k\304\231)", nullptr));
        groupBox_ARX->setTitle(QCoreApplication::translate("MainWindow", "Obiekt ARX", nullptr));
        pushConfigARX->setText(QCoreApplication::translate("MainWindow", "Konfiguracja parametr\303\263w ARX", nullptr));
        groupBox_File->setTitle(QCoreApplication::translate("MainWindow", "Konfiguracja JSON", nullptr));
        pushSaveConfig->setText(QCoreApplication::translate("MainWindow", "Zapisz", nullptr));
        pushLoadConfig->setText(QCoreApplication::translate("MainWindow", "Wczytaj", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
