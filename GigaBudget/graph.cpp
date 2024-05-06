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
    // Sprawdź, czy ramka już zawiera układ
    if (frame->layout()) {
        // Jeśli tak, usuń istniejący układ
        QLayout *existingLayout = frame->layout();
        delete existingLayout;
    }

    // Ustawienie ramki
    this->frame = frame;
    QVBoxLayout *layout = new QVBoxLayout(frame);
    layout->addWidget(chartView);

    // Usunięcie istniejących serii z wykresu
    series->clear();

    // Dodanie nowej serii danych na podstawie danych z QMap
    for (auto it = data.begin(); it != data.end(); ++it) {
        series->append(it.key(), it.value());
    }

    // Ustawienie tytułu
    chart->setTitle(title);

    // Dodanie legendy
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Dodanie nowej serii do wykresu
    chart->addSeries(series);
}



void Graph::updateGraphWithData(const QMap<QString, qreal> &data, const QString &title)
{
    // Usunięcie istniejących slices
    series->clear();

    // Dodanie nowych slices na podstawie danych z QMap
    for (auto it = data.begin(); it != data.end(); ++it) {
        series->append(it.key(), it.value());
    }

    // Ustawienie tytułu
    chart->setTitle(title);

    // Dodanie legendy
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Dodanie serii do wykresu
    // chart->addSeries(series);
}

void Graph::clearGraph()
{
    // Usunięcie wszystkich istniejących slice'ów
    series->clear();

    // Usunięcie tytułu
    chart->setTitle("");

    // Ukrycie legendy
    chart->legend()->setVisible(false);
}


