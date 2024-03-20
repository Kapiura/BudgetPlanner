#ifndef RAPORTHANDLER_H
#define RAPORTHANDLER_H

#include <QObject>

class RaportHandler : public QObject
{
    Q_OBJECT
public:
    explicit RaportHandler(QObject *parent = nullptr);

signals:
};

#endif // RAPORTHANDLER_H
