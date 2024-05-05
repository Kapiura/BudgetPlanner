#include "graph.h"
#include <QChartView>
#include <QDebug>
#include <QPieSeries>
#include <QtCharts>

Graph::Graph(QObject *parent)
    : QObject{parent}
{
    qDebug() << "Graph object has been created";
}

Graph::~Graph()
{
    qDebug() << "Graph object has been delted";
}

bool Graph::updatePieSeries()
{
    qDebug() << "Creating pie sereis\n";
    series->append("", 2);
}

bool Graph::addSeries()
{
    qDebug() << "Adding series from pie series\n";
}

bool Graph::setView()
{
    qDebug() << "Set view of series\n";
}
