#ifndef AXISSETUP_H
#define AXISSETUP_H

#include <QtCharts>

class AxisSetup {
public:
    AxisSetup(QChart *chartData, QChart *chartPackets, QLineSeries *seriesIn, QLineSeries *seriesOut,
              QLineSeries *seriesInPackets, QLineSeries *seriesOutPackets);

    QDateTimeAxis *axisXData;
    QDateTimeAxis *axisXPackets;
    QValueAxis *axisYData;
    QValueAxis *axisYPackets;

private:
    void setupAxes(QChart *chartData, QChart *chartPackets, QLineSeries *seriesIn, QLineSeries *seriesOut,
                   QLineSeries *seriesInPackets, QLineSeries *seriesOutPackets);

    void setupAxisAppearance();
};

#endif // AXISSETUP_H