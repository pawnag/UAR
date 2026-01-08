#ifndef TESTY_UAR_H
#define TESTY_UAR_H

#include <QObject>

class Testy_UAR : public QObject
{
    Q_OBJECT
public:
    explicit Testy_UAR(QObject *parent = nullptr);

signals:
};

#endif // TESTY_UAR_H
