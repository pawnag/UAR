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

    // Metody do pobierania przetworzonych danych z pól tekstowych
    std::vector<double> getA() const;
    std::vector<double> getB() const;
    int getOpoznienie() const;

    // Metody do ustawienia aktualnych wartości w oknie (żeby nie było puste po otwarciu)
    void ustawAktualne(const std::vector<double>& a, const std::vector<double>& b, int opoznienie);

private:
    Ui::ParametryARX *ui;

    // Funkcja pomocnicza do zamiany QString "1 0.5" na std::vector
    std::vector<double> stringToVector(const QString& str) const;
    QString vectorToString(const std::vector<double>& vec) const;
};

#endif // PARAMETRYARX_H
