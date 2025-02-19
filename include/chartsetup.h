#ifndef CHARTSETUP_H
#define CHARTSETUP_H

#include <QtCharts>

class ChartSetup {
public:
    ChartSetup();
    QChart *chartData;
    QChart *chartPackets;
    QLineSeries *seriesIn;
    QLineSeries *seriesOut;
    QLineSeries *seriesInPackets;
    QLineSeries *seriesOutPackets;

private:
    void setupSeries();
    void setupCharts();
    void setupChartAppearance();
};

#endif // CHARTSETUP_H