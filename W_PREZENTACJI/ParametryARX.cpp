#include "ParametryARX.h"
#include "ui_ParametryARX.h"
#include <QMessageBox>
#include <QStringList>

ParametryARX::ParametryARX(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametryARX)
{
    ui->setupUi(this);
}

ParametryARX::~ParametryARX()
{
    delete ui;
}

void ParametryARX::ustawAktualne(ModelARX model)
{
    QString strA = vectorToString(model.getA());
    QString strB = vectorToString(model.getB());

    ui->editA->setText(strA);
    ui->editB->setText(strB);
    ui->spinOpoznienie->setValue(model.getOpoznienieTransportowe());

    if (ui->spinZaklocenie) {
        ui->spinZaklocenie->setValue(model.getOdchylenieStandardoweSzumu());
    }

    ui->spinMinU->setValue(model.getUMIN());
    ui->spinMaxU->setValue(model.getUMAX());
    ui->spinMinY->setValue(model.getYMIN());
    ui->spinMaxY->setValue(model.getYMAX());

    if (ui->labelCurrentSummary) {
        QString info = QString("Obecne parametry:\nA: [%1]\nB: [%2]\nk: %3, Szum: %4")
        .arg(strA)
            .arg(strB)
            .arg(model.getOpoznienieTransportowe())
            .arg(model.getOdchylenieStandardoweSzumu());
        ui->labelCurrentSummary->setText(info);
    }
}

void ParametryARX::on_pushZapisz_clicked()
{
    std::vector<double> tempA = stringToVector(ui->editA->text());
    std::vector<double> tempB = stringToVector(ui->editB->text());

    while (tempA.size() < 3) {
        tempA.push_back(0.0);
    }

    while (tempB.size() < 3) {
        tempB.push_back(0.0);
    }

    int opoznienie = ui->spinOpoznienie->value();
    double szum = (ui->spinZaklocenie) ? ui->spinZaklocenie->value() : 0.0;
    double uMin = ui->spinMinU->value();
    double uMax = ui->spinMaxU->value();
    double yMin = ui->spinMinY->value();
    double yMax = ui->spinMaxY->value();

    if (uMin >= uMax) {
        QMessageBox::warning(this, "Błąd", "Min U musi być mniejsze od Max U!");
        return;
    }

    emit zglosNoweParametry(tempA, tempB, opoznienie, szum, uMin, uMax, yMin, yMax);

    this->accept();
}

void ParametryARX::on_pushAnuluj_clicked()
{
    this->reject();
}

std::vector<double> ParametryARX::stringToVector(const QString& str) const
{
    std::vector<double> vec;
    QStringList list = str.split(' ', Qt::SkipEmptyParts);
    for(const QString& s : list) {
        QString tempS = s;
        tempS.replace(",", ".");
        bool ok;
        double val = tempS.toDouble(&ok);
        if(ok) vec.push_back(val);
    }
    return vec;
}

QString ParametryARX::vectorToString(const std::vector<double>& vec) const
{
    QStringList list;
    for(double d : vec) list << QString::number(d);
    return list.join(' ');
}
