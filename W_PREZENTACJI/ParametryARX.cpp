#include "ParametryARX.h"
#include "ui_ParametryARX.h"
#include <QMessageBox> // <--- WAŻNE: Dodaj ten nagłówek

ParametryARX::ParametryARX(QWidget *parent) :
    QDialog(parent), ui(new Ui::ParametryARX)
{
    ui->setupUi(this);
    setWindowTitle("Konfiguracja Modelu ARX");
}

ParametryARX::~ParametryARX() { delete ui; }

// --- NOWA METODA WALIDUJĄCA ---
void ParametryARX::accept()
{
    // 1. Pobieramy wstępnie dane używając Twoich funkcji pomocniczych
    std::vector<double> tempA = stringToVector(ui->editA->text());
    std::vector<double> tempB = stringToVector(ui->editB->text());

    // 2. SPRAWDZENIE WIELOMIANU A
    if (tempA.size() < 3) {
        QMessageBox::warning(this, "Błąd parametrów",
                             "Wielomian A musi posiadać minimum 3 współczynniki!");
        ui->editA->setFocus(); // Ustaw kursor w polu błędu
        ui->editA->selectAll();
        return; // PRZERYWAMY: Nie zamykamy okna!
    }

    // 3. SPRAWDZENIE WIELOMIANU B
    if (tempB.size() < 3) {
        QMessageBox::warning(this, "Błąd parametrów",
                             "Wielomian B musi posiadać minimum 3 współczynniki!");
        ui->editB->setFocus();
        ui->editB->selectAll();
        return; // PRZERYWAMY
    }

    // 4. Jeśli wszystko OK, wywołujemy oryginalną metodę, która zamyka okno
    QDialog::accept();
}

// ZAKTUALIZOWANA FUNKCJA
void ParametryARX::ustawAktualne(const std::vector<double>& a,
                                 const std::vector<double>& b,
                                 int opoznienie,
                                 double zaklocenie)
{
    // 1. Konwersja wektorów na tekst
    QString strA = vectorToString(a);
    QString strB = vectorToString(b);

    // 2. Wypełnienie pól edycyjnych (żeby użytkownik miał co edytować)
    ui->editA->setText(strA);
    ui->editB->setText(strB);
    ui->spinOpoznienie->setValue(opoznienie);

    // Jeśli w UI masz spinBox od zakłóceń, ustaw go też:
    if(ui->spinZaklocenie) {
        ui->spinZaklocenie->setValue(zaklocenie);
    }

    // 3. AKTUALIZACJA ETYKIETY PODSUMOWANIA (To, o co chodziło)
    QString info = QString("A: [%1]\n"
                           "B: [%2]\n"
                           "k: %3, Szum: %4")
                       .arg(strA)
                       .arg(strB)
                       .arg(opoznienie)
                       .arg(zaklocenie);

    // Upewnij się, że nazwa labela w .ui to labelCurrentSummary
    ui->labelCurrentSummary->setText(info);
}

// Gettery
std::vector<double> ParametryARX::getA() const { return stringToVector(ui->editA->text()); }

std::vector<double> ParametryARX::getB() const { return stringToVector(ui->editB->text()); }

int ParametryARX::getOpoznienie() const
{
    return ui->spinOpoznienie->value();
}

// Funkcje pomocnicze
std::vector<double> ParametryARX::stringToVector(const QString& str) const
{
    std::vector<double> vec;
    // Qt::SkipEmptyParts jest ważne, żeby podwójne spacje nie robiły błędów
    QStringList list = str.split(' ', Qt::SkipEmptyParts);
    for(const QString& s : list) {
        bool ok;
        // Zamiana kropki na przecinek jeśli ktoś wpisze złą notację (opcjonalne, ale przydatne)
        QString tempS = s;
        tempS.replace(",", ".");

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
