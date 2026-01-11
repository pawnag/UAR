/********************************************************************************
** Form generated from reading UI file 'ParametryARX.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARAMETRYARX_H
#define UI_PARAMETRYARX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ParametryARX
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupCurrentValues;
    QVBoxLayout *verticalLayout_Current;
    QLabel *labelCurrentSummary;
    QGroupBox *groupEdit;
    QFormLayout *formLayout;
    QLabel *labelA;
    QLineEdit *editA;
    QLabel *labelB;
    QLineEdit *editB;
    QLabel *labelD;
    QSpinBox *spinOpoznienie;
    QLabel *labelZaklocenie;
    QDoubleSpinBox *spinZaklocenie;
    QGroupBox *groupOgraniczenia;
    QGridLayout *gridLayout;
    QCheckBox *checkOgraniczenia;
    QLabel *labelU;
    QDoubleSpinBox *spinMinU;
    QDoubleSpinBox *spinMaxU;
    QLabel *labelY;
    QDoubleSpinBox *spinMinY;
    QDoubleSpinBox *spinMaxY;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ParametryARX)
    {
        if (ParametryARX->objectName().isEmpty())
            ParametryARX->setObjectName("ParametryARX");
        ParametryARX->resize(420, 530);
        verticalLayout = new QVBoxLayout(ParametryARX);
        verticalLayout->setObjectName("verticalLayout");
        groupCurrentValues = new QGroupBox(ParametryARX);
        groupCurrentValues->setObjectName("groupCurrentValues");
        verticalLayout_Current = new QVBoxLayout(groupCurrentValues);
        verticalLayout_Current->setObjectName("verticalLayout_Current");
        labelCurrentSummary = new QLabel(groupCurrentValues);
        labelCurrentSummary->setObjectName("labelCurrentSummary");
        labelCurrentSummary->setWordWrap(true);

        verticalLayout_Current->addWidget(labelCurrentSummary);


        verticalLayout->addWidget(groupCurrentValues);

        groupEdit = new QGroupBox(ParametryARX);
        groupEdit->setObjectName("groupEdit");
        formLayout = new QFormLayout(groupEdit);
        formLayout->setObjectName("formLayout");
        formLayout->setVerticalSpacing(15);
        labelA = new QLabel(groupEdit);
        labelA->setObjectName("labelA");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelA);

        editA = new QLineEdit(groupEdit);
        editA->setObjectName("editA");

        formLayout->setWidget(0, QFormLayout::FieldRole, editA);

        labelB = new QLabel(groupEdit);
        labelB->setObjectName("labelB");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelB);

        editB = new QLineEdit(groupEdit);
        editB->setObjectName("editB");

        formLayout->setWidget(1, QFormLayout::FieldRole, editB);

        labelD = new QLabel(groupEdit);
        labelD->setObjectName("labelD");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelD);

        spinOpoznienie = new QSpinBox(groupEdit);
        spinOpoznienie->setObjectName("spinOpoznienie");
        spinOpoznienie->setMinimum(1);
        spinOpoznienie->setMaximum(100);

        formLayout->setWidget(2, QFormLayout::FieldRole, spinOpoznienie);

        labelZaklocenie = new QLabel(groupEdit);
        labelZaklocenie->setObjectName("labelZaklocenie");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelZaklocenie);

        spinZaklocenie = new QDoubleSpinBox(groupEdit);
        spinZaklocenie->setObjectName("spinZaklocenie");
        spinZaklocenie->setDecimals(2);
        spinZaklocenie->setSingleStep(0.010000000000000);

        formLayout->setWidget(3, QFormLayout::FieldRole, spinZaklocenie);


        verticalLayout->addWidget(groupEdit);

        groupOgraniczenia = new QGroupBox(ParametryARX);
        groupOgraniczenia->setObjectName("groupOgraniczenia");
        gridLayout = new QGridLayout(groupOgraniczenia);
        gridLayout->setObjectName("gridLayout");
        checkOgraniczenia = new QCheckBox(groupOgraniczenia);
        checkOgraniczenia->setObjectName("checkOgraniczenia");
        checkOgraniczenia->setChecked(true);

        gridLayout->addWidget(checkOgraniczenia, 0, 0, 1, 3);

        labelU = new QLabel(groupOgraniczenia);
        labelU->setObjectName("labelU");

        gridLayout->addWidget(labelU, 1, 0, 1, 1);

        spinMinU = new QDoubleSpinBox(groupOgraniczenia);
        spinMinU->setObjectName("spinMinU");
        spinMinU->setMinimum(-1000.000000000000000);
        spinMinU->setMaximum(1000.000000000000000);
        spinMinU->setValue(-10.000000000000000);

        gridLayout->addWidget(spinMinU, 1, 1, 1, 1);

        spinMaxU = new QDoubleSpinBox(groupOgraniczenia);
        spinMaxU->setObjectName("spinMaxU");
        spinMaxU->setMinimum(-1000.000000000000000);
        spinMaxU->setMaximum(1000.000000000000000);
        spinMaxU->setValue(10.000000000000000);

        gridLayout->addWidget(spinMaxU, 1, 2, 1, 1);

        labelY = new QLabel(groupOgraniczenia);
        labelY->setObjectName("labelY");

        gridLayout->addWidget(labelY, 2, 0, 1, 1);

        spinMinY = new QDoubleSpinBox(groupOgraniczenia);
        spinMinY->setObjectName("spinMinY");
        spinMinY->setMinimum(-1000.000000000000000);
        spinMinY->setMaximum(1000.000000000000000);
        spinMinY->setValue(-10.000000000000000);

        gridLayout->addWidget(spinMinY, 2, 1, 1, 1);

        spinMaxY = new QDoubleSpinBox(groupOgraniczenia);
        spinMaxY->setObjectName("spinMaxY");
        spinMaxY->setMinimum(-1000.000000000000000);
        spinMaxY->setMaximum(1000.000000000000000);
        spinMaxY->setValue(10.000000000000000);

        gridLayout->addWidget(spinMaxY, 2, 2, 1, 1);


        verticalLayout->addWidget(groupOgraniczenia);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(ParametryARX);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ParametryARX);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, ParametryARX, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, ParametryARX, qOverload<>(&QDialog::reject));
        QObject::connect(checkOgraniczenia, &QCheckBox::toggled, spinMinU, &QDoubleSpinBox::setEnabled);
        QObject::connect(checkOgraniczenia, &QCheckBox::toggled, spinMaxU, &QDoubleSpinBox::setEnabled);
        QObject::connect(checkOgraniczenia, &QCheckBox::toggled, spinMinY, &QDoubleSpinBox::setEnabled);
        QObject::connect(checkOgraniczenia, &QCheckBox::toggled, spinMaxY, &QDoubleSpinBox::setEnabled);

        QMetaObject::connectSlotsByName(ParametryARX);
    } // setupUi

    void retranslateUi(QDialog *ParametryARX)
    {
        ParametryARX->setWindowTitle(QCoreApplication::translate("ParametryARX", "Konfiguracja Modelu ARX", nullptr));
        groupCurrentValues->setTitle(QCoreApplication::translate("ParametryARX", "Aktualnie aktywne parametry obiektu:", nullptr));
        labelCurrentSummary->setText(QCoreApplication::translate("ParametryARX", "A: [1.0, -0.4]\n"
"B: [0.6]\n"
"k: 1, Zak\305\202\303\263cenia: 0.00", nullptr));
        groupEdit->setTitle(QCoreApplication::translate("ParametryARX", "Edycja parametr\303\263w:", nullptr));
        labelA->setText(QCoreApplication::translate("ParametryARX", "Wielomian A (np. 1 -0.4):", nullptr));
        editA->setPlaceholderText(QCoreApplication::translate("ParametryARX", "Wpisz wsp\303\263\305\202czynniki po spacji", nullptr));
        labelB->setText(QCoreApplication::translate("ParametryARX", "Wielomian B (np. 0.6 0.2):", nullptr));
        editB->setPlaceholderText(QCoreApplication::translate("ParametryARX", "Wpisz wsp\303\263\305\202czynniki po spacji", nullptr));
        labelD->setText(QCoreApplication::translate("ParametryARX", "Op\303\263\305\272nienie (k):", nullptr));
        labelZaklocenie->setText(QCoreApplication::translate("ParametryARX", "Odchylenie szumu:", nullptr));
        groupOgraniczenia->setTitle(QCoreApplication::translate("ParametryARX", "Ograniczenia Sygna\305\202\303\263w:", nullptr));
        checkOgraniczenia->setText(QCoreApplication::translate("ParametryARX", "W\305\202\304\205cz ograniczenia", nullptr));
        labelU->setText(QCoreApplication::translate("ParametryARX", "Sterowanie (u):", nullptr));
        spinMinU->setPrefix(QCoreApplication::translate("ParametryARX", "Min: ", nullptr));
        spinMaxU->setPrefix(QCoreApplication::translate("ParametryARX", "Max: ", nullptr));
        labelY->setText(QCoreApplication::translate("ParametryARX", "Wyj\305\233cie (y):", nullptr));
        spinMinY->setPrefix(QCoreApplication::translate("ParametryARX", "Min: ", nullptr));
        spinMaxY->setPrefix(QCoreApplication::translate("ParametryARX", "Max: ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ParametryARX: public Ui_ParametryARX {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAMETRYARX_H
