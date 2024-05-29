#include "graph.h"

Graph::Graph(QObject *parent) : QObject(parent), frame(nullptr)
{
    qDebug() << "graph summoned\n";
    series = new QPieSeries();
    chart = new QChart();
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chart->setBackgroundBrush(QBrush(QColor("#f0f0f0")));
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
    if (frame->layout())
    {
        QLayout *existingLayout = frame->layout();
        delete existingLayout;
    }

    this->frame = frame;
    QVBoxLayout *layout = new QVBoxLayout(frame);
    layout->addWidget(chartView);

    series->clear();
    int colorIndex = 0;

    for (auto it = data.begin(); it != data.end(); ++it)
    {
        QPieSlice *slice = series->append(it.key(), it.value());
        // Kolorowanie segmentu
        QColor color = this->generateColor(colorIndex);
        slice->setColor(color);
        colorIndex++;
    }

    chart->setTitle(title);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chartView->setRenderHint(QPainter::Antialiasing);

    chart->addSeries(series);
}

void Graph::updateGraphWithData(const QMap<QString, qreal> &data, const QString &title)
{
    series->clear();
    int colorIndex = 0;

    for (auto it = data.begin(); it != data.end(); ++it)
    {
        QPieSlice *slice = series->append(it.key(), it.value());
        // Kolorowanie segmentu
        QColor color = this->generateColor(colorIndex);
        slice->setColor(color);
        colorIndex++;
    }

    chart->setTitle(title);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chartView->setRenderHint(QPainter::Antialiasing);
}

void Graph::clearGraph()
{
    series->clear();
    chart->setTitle("");
    chart->legend()->setVisible(false);
}

QColor Graph::generateColor(int index)
{
    static const QList<QColor> colors = {
        QColor(255, 179, 186), // pastelowy róż
        QColor(255, 223, 186), // pastelowy pomarańcz
        QColor(255, 255, 186), // pastelowy żółty
        QColor(186, 255, 201), // pastelowy zielony
        QColor(186, 255, 255), // pastelowy cyjan
        QColor(186, 223, 255), // pastelowy niebieski
        QColor(186, 186, 255), // pastelowy fiolet
        QColor(223, 186, 255), // pastelowy purpurowy
        QColor(255, 186, 255), // pastelowy różowy
        QColor(255, 186, 223)  // pastelowy koral
    };

    return colors[index % colors.size()];
}
