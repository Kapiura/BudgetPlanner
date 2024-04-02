#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <QtCharts>

class Graph : public QObject
{
    Q_OBJECT
  public:
    explicit Graph(QObject *parent = nullptr);

  signals:
};

#endif // GRAPH_H
