#include "../include/axissetup.h"

AxisSetup::AxisSetup(QChart *chartData, QChart *chartPackets, QLineSeries *seriesIn, QLineSeries *seriesOut,
                     QLineSeries *seriesInPackets, QLineSeries *seriesOutPackets) {
    setupAxes(chartData, chartPackets, seriesIn, seriesOut, seriesInPackets, seriesOutPackets);
    setupAxisAppearance();
}

void AxisSetup::setupAxes(QChart *chartData, QChart *chartPackets, QLineSeries *seriesIn, QLineSeries *seriesOut,
                          QLineSeries *seriesInPackets, QLineSeries *seriesOutPackets) {
    axisXData = new QDateTimeAxis();
    axisXPackets = new QDateTimeAxis();
    axisXData->setFormat("hh:mm:ss");
    axisXPackets->setFormat("hh:mm:ss");
    axisXData->setTitleText("Time");
    axisXPackets->setTitleText("Time");
    axisXData->setTickCount(11);
    axisXPackets->setTickCount(11);
    axisXData->setGridLineVisible(false);
    axisXPackets->setGridLineVisible(false);

    axisYData = new QValueAxis();
    axisYPackets = new QValueAxis();
    axisYData->setTitleText("Data (MB)");
    axisYPackets->setTitleText("Packets");
    axisYData->setTickCount(9);
    axisYPackets->setTickCount(9);
    axisYData->setGridLineVisible(false);
    axisYPackets->setGridLineVisible(false);

    chartData->addAxis(axisXData, Qt::AlignBottom);
    chartPackets->addAxis(axisXPackets, Qt::AlignBottom);
    chartData->addAxis(axisYData, Qt::AlignLeft);
    chartPackets->addAxis(axisYPackets, Qt::AlignLeft);

    seriesIn->attachAxis(axisXData);
    seriesOut->attachAxis(axisXData);
    seriesInPackets->attachAxis(axisXPackets);
    seriesOutPackets->attachAxis(axisXPackets);
    seriesIn->attachAxis(axisYData);
    seriesOut->attachAxis(axisYData);
    seriesInPackets->attachAxis(axisYPackets);
    seriesOutPackets->attachAxis(axisYPackets);
}

void AxisSetup::setupAxisAppearance() {
    QFont axisFont;
    axisFont.setPointSize(8);

    axisXData->setLabelsBrush(QBrush(Qt::white));
    axisXPackets->setLabelsBrush(QBrush(Qt::white));
    axisYData->setLabelsBrush(QBrush(Qt::white));
    axisYPackets->setLabelsBrush(QBrush(Qt::white));
    axisXData->setTitleBrush(QBrush(Qt::white));
    axisXPackets->setTitleBrush(QBrush(Qt::white));
    axisYData->setTitleBrush(QBrush(Qt::white));
    axisYPackets->setTitleBrush(QBrush(Qt::white));

    axisXData->setLabelsFont(axisFont);
    axisXPackets->setLabelsFont(axisFont);
}