#ifndef PARAMETRYARX_H
#define PARAMETRYARX_H

#include <QDialog>
#include <vector>

namespace Ui { class ParametryARX; }

class ParametryARX : public QDialog
{
    Q_OBJECT

public:
    explicit ParametryARX(QWidget *parent = nullptr);
    ~ParametryARX();

    void ustawDane(const std::vector<double>& a, const std::vector<double>& b,
                   int opoznienie, double szum,
                   double umin, double umax, double ymin, double ymax);

signals:
    void zglosNoweParametry(std::vector<double> a, std::vector<double> b,
                            int opoznienie, double szum,
                            double umin, double umax, double ymin, double ymax);

private slots:
    void on_pushZapisz_clicked();
    void on_pushAnuluj_clicked();

private:
    Ui::ParametryARX *ui;

    std::vector<double> stringToVector(const QString& str) const;
    QString vectorToString(const std::vector<double>& vec) const;
};

#endif // PARAMETRYARX_H
