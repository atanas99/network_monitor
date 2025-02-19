#ifndef NETWORKCHART_H
#define NETWORKCHART_H

#include <QWidget>
#include <QtCharts>
#include <QQueue>
#include <QLabel>
#include <QCheckBox>

class ChartSetup;
class AxisSetup;
class UIComponents;
class DataUpdater;

class NetworkChart : public QWidget {
Q_OBJECT

public:
    explicit NetworkChart(QWidget *parent = nullptr);
    void updateNetworkInfo(const QList<QList<qint64>> &networkData);
    void toggleSeriesVisibility();

private:
    ChartSetup *chartSetup;
    AxisSetup *axisSetup;
    UIComponents *uiComponents;
    DataUpdater *dataUpdater;
};

#endif // NETWORKCHART_H