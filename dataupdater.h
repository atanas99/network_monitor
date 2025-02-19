#ifndef DATAUPDATER_H
#define DATAUPDATER_H

#include <QtCharts>
#include <QLabel>
#include <QQueue>

class DataUpdater {
public:
    DataUpdater(QLineSeries *seriesIn, QLineSeries *seriesOut, QLineSeries *seriesInPackets,
                QLineSeries *seriesOutPackets,
                QDateTimeAxis *axisXData, QDateTimeAxis *axisXPackets, QValueAxis *axisYData, QValueAxis *axisYPackets,
                QLabel *labelIn, QLabel *labelOut, QLabel *labelInPackets, QLabel *labelOutPackets);

    void updateNetworkInfo(const QList<QList<qint64>> &networkData);

private:
    QLineSeries *seriesIn;
    QLineSeries *seriesOut;
    QLineSeries *seriesInPackets;
    QLineSeries *seriesOutPackets;
    QDateTimeAxis *axisXData;
    QDateTimeAxis *axisXPackets;
    QValueAxis *axisYData;
    QValueAxis *axisYPackets;
    QLabel *labelIn;
    QLabel *labelOut;
    QLabel *labelInPackets;
    QLabel *labelOutPackets;

    qint64 previousInBytes = 0;
    qint64 previousOutBytes = 0;
    qint64 previousInPackets = 0;
    qint64 previousOutPackets = 0;

    QQueue<QPair<qint64, qreal>> inPacketsQueue;
    QQueue<QPair<qint64, qreal>> outPacketsQueue;
    QQueue<QPair<qint64, qreal>> inPacketsCountQueue;
    QQueue<QPair<qint64, qreal>> outPacketsCountQueue;
};

#endif // DATAUPDATER_H