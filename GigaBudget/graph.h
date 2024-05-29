#ifndef GRAPH_H
#define GRAPH_H

#include <QChart>
#include <QChartView>
#include <QFrame>
#include <QObject>
#include <QPieSeries>
#include <QVBoxLayout>
#include <QtCharts>

class Graph : public QObject
{
    Q_OBJECT
  public:
    explicit Graph(QObject *parent = nullptr);

    ~Graph();

    void setFrame(QFrame *frame);
    QChartView *getView();

    void addSlice(const QString &label, qreal value);
    void addDataFromMap(const QMap<QString, qreal> &data);
    void clearGraph();
    void setTitle(const QString &title);
    void addLegend();
    void updateGraph(const QMap<QString, qreal> &data, QFrame *frame, const QString &title);
    void updateGraph(const QMap<QString, qreal> &data, const QString &title);
    void updateGraphWithData(const QMap<QString, qreal> &data, const QString &title);
    void colorSlices();

  private:
    QPieSeries *series;
    QChart *chart;
    QChartView *chartView;
    QFrame *frame;
    bool created;
    QColor generateColor(const QString &key);
    QColor generateColor(int index);
};

#endif // GRAPH_H
