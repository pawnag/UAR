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

    // Metody do pobierania danych
    std::vector<double> getA() const;
    std::vector<double> getB() const;
    int getOpoznienie() const;

    // ZMIANA: Dodajemy 'double zaklocenie'
    void ustawAktualne(const std::vector<double>& a,
                       const std::vector<double>& b,
                       int opoznienie,
                       double zaklocenie);

public slots:
    // --- TUTAJ DODAJEMY NADPISANIE METODY ACCEPT ---
    void accept() override;

private:
    Ui::ParametryARX *ui;

    std::vector<double> stringToVector(const QString& str) const;
    QString vectorToString(const std::vector<double>& vec) const;
};

#endif // PARAMETRYARX_H
