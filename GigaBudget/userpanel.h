#ifndef USERPANEL_H
#define USERPANEL_H

#include <QObject>

class UserPanel : public QObject
{
    Q_OBJECT
public:
    explicit UserPanel(QObject *parent = nullptr);

signals:
};

#endif // USERPANEL_H
