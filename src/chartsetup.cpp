#include "../include/chartsetup.h"
#include <QPen>

ChartSetup::ChartSetup() {
    setupCharts();
    setupSeries();
    setupChartAppearance();
}

void ChartSetup::setupCharts() {
    chartData = new QChart();
    chartPackets = new QChart();
}

void ChartSetup::setupSeries() {
    seriesIn = new QLineSeries();
    seriesOut = new QLineSeries();
    seriesInPackets = new QLineSeries();
    seriesOutPackets = new QLineSeries();

    QPen greenPen(Qt::darkGreen);
    seriesIn->setPen(greenPen);
    QPen bluePen(Qt::darkMagenta);
    seriesOut->setPen(bluePen);
    QPen redPen(Qt::darkRed);
    seriesInPackets->setPen(redPen);
    QPen yellowPen(Qt::darkCyan);
    seriesOutPackets->setPen(yellowPen);

    seriesIn->setName("<b>Received Data (MB)</b>");
    seriesOut->setName("<b>Sent Data (MB)</b>");
    seriesInPackets->setName("<b>Received Packets</b>");
    seriesOutPackets->setName("<b>Sent Packets</b>");

    chartData->addSeries(seriesIn);
    chartData->addSeries(seriesOut);
    chartPackets->addSeries(seriesInPackets);
    chartPackets->addSeries(seriesOutPackets);
}

void ChartSetup::setupChartAppearance() {
    QChart *charts[] = {chartData, chartPackets};
    for (QChart *chart: charts) {
        chart->setBackgroundBrush(QBrush(QColor(53, 53, 53)));
        chart->setTitleBrush(QBrush(Qt::white));
        chart->setPlotAreaBackgroundBrush(QBrush(QColor(35, 35, 35)));
        chart->setPlotAreaBackgroundVisible(true);
        chart->legend()->setLabelBrush(QBrush(Qt::white));
    }
}