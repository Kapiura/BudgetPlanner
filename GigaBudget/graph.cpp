// graph.cpp

#include "graph.h"

Graph::Graph(QObject *parent)
    : QObject(parent), frame(nullptr)
{
    qDebug() << "graph summoned\n";
    series = new QPieSeries();
    chart = new QChart();
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
}

Graph::~Graph()
{
    qDebug() << "graph deleted\n";
    delete chartView;
    delete chart;
    delete series;
}

void Graph::updateGraph(const QMap<QString, qreal> &data, QFrame *frame, const QString &title)
{
    if (frame->layout()) {

        QLayout *existingLayout = frame->layout();
        delete existingLayout;
    }


    this->frame = frame;
    QVBoxLayout *layout = new QVBoxLayout(frame);
    layout->addWidget(chartView);


    series->clear();


    for (auto it = data.begin(); it != data.end(); ++it) {
        series->append(it.key(), it.value());
    }
    this->colorSlices();



    chart->setTitle(title);


    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chartView->setRenderHint(QPainter::Antialiasing);


    chart->addSeries(series);
}



void Graph::updateGraphWithData(const QMap<QString, qreal> &data, const QString &title)
{

    series->clear();


    for (auto it = data.begin(); it != data.end(); ++it) {
        series->append(it.key(), it.value());
    }
this->colorSlices();

    chart->setTitle(title);


    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chartView->setRenderHint(QPainter::Antialiasing);


    // chart->addSeries(series);
}

void Graph::clearGraph()
{

    series->clear();


    chart->setTitle("");


    chart->legend()->setVisible(false);
}

void Graph::colorSlices()
{
    if(created == false)
    {
        QPieSlice* slice;
        foreach(slice,series->slices())
        {
            slice->setBrush(QColor(rand()%256,rand()%256,rand()%256));
        }
        created = true;
    }

}

