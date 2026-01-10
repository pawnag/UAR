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
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
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
    QFormLayout *formLayout;
    QLabel *labelA;
    QLineEdit *editA;
    QLabel *labelB;
    QLineEdit *editB;
    QLabel *labelD;
    QSpinBox *spinOpoznienie;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ParametryARX)
    {
        if (ParametryARX->objectName().isEmpty())
            ParametryARX->setObjectName("ParametryARX");
        ParametryARX->resize(400, 250);
        ParametryARX->setStyleSheet(QString::fromUtf8("QDialog { background-color: #1e1e1e; color: white; }\n"
"QLabel { color: white; }\n"
"QLineEdit { background-color: #2d2d2d; color: white; border: 1px solid #555; padding: 5px; border-radius: 3px; }\n"
"QSpinBox { background-color: #2d2d2d; color: white; border: 1px solid #555; padding: 5px; }\n"
"QPushButton { background-color: #3d3d3d; color: white; border: 1px solid #555; padding: 5px 15px; border-radius: 3px; }\n"
"QPushButton:hover { background-color: #4d4d4d; }"));
        verticalLayout = new QVBoxLayout(ParametryARX);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        formLayout->setVerticalSpacing(15);
        labelA = new QLabel(ParametryARX);
        labelA->setObjectName("labelA");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelA);

        editA = new QLineEdit(ParametryARX);
        editA->setObjectName("editA");

        formLayout->setWidget(0, QFormLayout::FieldRole, editA);

        labelB = new QLabel(ParametryARX);
        labelB->setObjectName("labelB");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelB);

        editB = new QLineEdit(ParametryARX);
        editB->setObjectName("editB");

        formLayout->setWidget(1, QFormLayout::FieldRole, editB);

        labelD = new QLabel(ParametryARX);
        labelD->setObjectName("labelD");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelD);

        spinOpoznienie = new QSpinBox(ParametryARX);
        spinOpoznienie->setObjectName("spinOpoznienie");
        spinOpoznienie->setMinimum(1);
        spinOpoznienie->setMaximum(100);

        formLayout->setWidget(2, QFormLayout::FieldRole, spinOpoznienie);


        verticalLayout->addLayout(formLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(ParametryARX);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ParametryARX);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, ParametryARX, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, ParametryARX, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(ParametryARX);
    } // setupUi

    void retranslateUi(QDialog *ParametryARX)
    {
        ParametryARX->setWindowTitle(QCoreApplication::translate("ParametryARX", "Konfiguracja Modelu ARX", nullptr));
        labelA->setText(QCoreApplication::translate("ParametryARX", "Wielomian A (np. 1 -0.4):", nullptr));
        editA->setPlaceholderText(QCoreApplication::translate("ParametryARX", "Wpisz wsp\303\263\305\202czynniki po spacji", nullptr));
        labelB->setText(QCoreApplication::translate("ParametryARX", "Wielomian B (np. 0.6 0.2):", nullptr));
        editB->setPlaceholderText(QCoreApplication::translate("ParametryARX", "Wpisz wsp\303\263\305\202czynniki po spacji", nullptr));
        labelD->setText(QCoreApplication::translate("ParametryARX", "Op\303\263\305\272nienie (d):", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ParametryARX: public Ui_ParametryARX {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAMETRYARX_H
