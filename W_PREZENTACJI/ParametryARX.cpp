// #include "ParametryARX.h"
// #include "ui_ParametryARX.h"

// ParametryARX::ParametryARX(QWidget *parent) :
//     QDialog(parent), ui(new Ui::ParametryARX)
// {
//     ui->setupUi(this);
//     setWindowTitle("Konfiguracja Modelu ARX");
// }

// ParametryARX::~ParametryARX() { delete ui; }

// void ParametryARX::ustawAktualne(const std::vector<double>& a, const std::vector<double>& b, int opoznienie)
// {
//     ui->editA->setText(vectorToString(a));
//     ui->editB->setText(vectorToString(b));
//     ui->spinOpoznienie->setValue(opoznienie);
// }

// std::vector<double> ParametryARX::getA() const { return stringToVector(ui->editA->text()); }
// std::vector<double> ParametryARX::getB() const { return stringToVector(ui->editB->text()); }
// int ParametryARX::getOpoznienie() const { return ui->spinOpoznienie->value(); }

// std::vector<double> ParametryARX::stringToVector(const QString& str) const
// {
//     std::vector<double> vec;
//     QStringList list = str.split(' ', Qt::SkipEmptyParts);
//     for(const QString& s : list) {
//         bool ok;
//         double val = s.toDouble(&ok);
//         if(ok) vec.push_back(val);
//     }
//     return vec;
// }

// QString ParametryARX::vectorToString(const std::vector<double>& vec) const
// {
//     QStringList list;
//     for(double d : vec) list << QString::number(d);
//     return list.join(' ');
// }
