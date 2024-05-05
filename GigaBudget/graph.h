#ifndef GRAPH_H
#define GRAPH_H

#include <QChartView>
#include <QDebug>
#include <QObject>
#include <QPieSeries>
#include <QtCharts>

class Graph : public QObject
{
private:
    QPieSeries *series = new QPieSeries();
    QChart *chart = new QChart();
    QChartView *chartView = new QChartView();

    Q_OBJECT
public:
    explicit Graph(QObject *parent = nullptr);
    ~Graph();

    bool updatePieSeries();
    bool addSeries();
    bool setView();

signals:
};

#endif // GRAPH_H
