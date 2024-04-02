#include "graph.h"
#include <QDebug>

Graph::Graph(QObject *parent) : QObject{parent}
{
    qDebug() << "Graph object has been created";
}

Graph::~Graph()
{
    qDebug() << "Graph object has been delted";
}
