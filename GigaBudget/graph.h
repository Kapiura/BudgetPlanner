#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
// #include <QtCharts>

class Graph : public QObject
{
    Q_OBJECT
  public:
    explicit Graph(QObject *parent = nullptr);
    ~Graph();

  signals:
};

#endif // GRAPH_H
