#include "ParametryARX.h"
#include "ui_ParametryARX.h"
#include <QMessageBox>

ParametryARX::ParametryARX(QWidget *parent) :
    QDialog(parent), ui(new Ui::ParametryARX)
{
    ui->setupUi(this);
    setWindowTitle("Konfiguracja Modelu ARX");

    // Opcjonalnie: ukrycie znaku zapytania na pasku tytułu
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

ParametryARX::~ParametryARX() { delete ui; }

// --- METODA WALIDUJĄCA (Twoja implementacja) ---
// Jest wywoływana automatycznie przy wciśnięciu Enter lub przez on_pushZapisz_clicked
void ParametryARX::accept()
{
    // 1. Pobieramy dane z UI
    std::vector<double> tempA = stringToVector(ui->editA->text());
    std::vector<double> tempB = stringToVector(ui->editB->text());

    // 2. Diagnostyka (zobaczysz to w konsoli Qt Creator)
    qDebug() << "Próba zapisu. A:" << tempA.size() << "B:" << tempB.size();

    // 3. Walidacja
    if (tempA.size() < 3) {
        QMessageBox::warning(this, "Błąd", "Wielomian A za krótki!");
        return; // PRZERYWAMY, okno zostaje otwarte
    }
    if (tempB.size() < 3) {
        QMessageBox::warning(this, "Błąd", "Wielomian B za krótki!");
        return; // PRZERYWAMY
    }

    // 4. KLUCZOWE: Jeśli wszystko OK, mówimy Qt "Zaakceptowano"
    QDialog::accept();
}

// --- POBIERANIE DANYCH Z OKNA (Gettery) ---
std::vector<double> ParametryARX::getA() const { return stringToVector(ui->editA->text()); }

std::vector<double> ParametryARX::getB() const { return stringToVector(ui->editB->text()); }

int ParametryARX::getOpoznienie() const { return ui->spinOpoznienie->value(); }

double ParametryARX::getSzum() const
{
    // Zabezpieczenie, gdyby kontrolka nie istniała w UI
    if(ui->spinZaklocenie) return ui->spinZaklocenie->value();
    return 0.0;
}

// --- INICJALIZACJA OKNA ---
void ParametryARX::ustawAktualne(const std::vector<double>& a,
                                 const std::vector<double>& b,
                                 int opoznienie,
                                 double zaklocenie)
{
    QString strA = vectorToString(a);
    QString strB = vectorToString(b);

    ui->editA->setText(strA);
    ui->editB->setText(strB);
    ui->spinOpoznienie->setValue(opoznienie);

    if(ui->spinZaklocenie) {
        ui->spinZaklocenie->setValue(zaklocenie);
    }

    // Aktualizacja etykiety informacyjnej
    QString info = QString("A: [%1]\nB: [%2]\nk: %3, Szum: %4")
                       .arg(strA).arg(strB).arg(opoznienie).arg(zaklocenie);

    // Upewnij się, że masz labelCurrentSummary w pliku .ui
    if(ui->labelCurrentSummary) ui->labelCurrentSummary->setText(info);
}

// --- OBSŁUGA PRZYCISKÓW (To trzeba było uzupełnić) ---

void ParametryARX::on_pushZapisz_clicked()
{
    // Wywołujemy accept(), który uruchamia Twoją walidację napisaną wyżej.
    // Jeśli walidacja przejdzie, okno się zamknie i zwróci QDialog::Accepted.
    accept();
}

void ParametryARX::on_pushAnuluj_clicked()
{
    // Zamykamy okno i zwracamy QDialog::Rejected.
    // Zmiany zostaną zignorowane w MainWindow.
    reject();
}

// --- FUNKCJE POMOCNICZE ---
std::vector<double> ParametryARX::stringToVector(const QString& str) const
{
    std::vector<double> vec;
    QStringList list = str.split(' ', Qt::SkipEmptyParts);
    for(const QString& s : list) {
        QString tempS = s;
        tempS.replace(",", "."); // Zamiana przecinka na kropkę
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
