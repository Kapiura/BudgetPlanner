#ifndef RAPORTHANLDER_H
#define RAPORTHANLDER_H

#include <QObject>

class RaportHanlder : public QObject
{
    Q_OBJECT
public:
    explicit RaportHanlder(QObject *parent = nullptr);

signals:
};

#endif // RAPORTHANLDER_H
