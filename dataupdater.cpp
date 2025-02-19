#include "dataupdater.h"
#include <QDateTime>

DataUpdater::DataUpdater(
        QLineSeries *seriesIn, QLineSeries *seriesOut, QLineSeries *seriesInPackets,QLineSeries *seriesOutPackets,
        QDateTimeAxis *axisXData, QDateTimeAxis *axisXPackets, QValueAxis *axisYData,QValueAxis *axisYPackets,
        QLabel *labelIn, QLabel *labelOut, QLabel *labelInPackets, QLabel *labelOutPackets):

        seriesIn(seriesIn), seriesOut(seriesOut), seriesInPackets(seriesInPackets),
        seriesOutPackets(seriesOutPackets),
        axisXData(axisXData), axisXPackets(axisXPackets), axisYData(axisYData), axisYPackets(axisYPackets),
        labelIn(labelIn), labelOut(labelOut), labelInPackets(labelInPackets), labelOutPackets(labelOutPackets) {}

void DataUpdater::updateNetworkInfo(const QList<QList<qint64>> &networkData) {
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();

    qint64 totalInBytes = 0;
    qint64 totalOutBytes = 0;
    qint64 totalInPackets = 0;
    qint64 totalOutPackets = 0;

    for (const auto &rowData: networkData) {
        totalInBytes += rowData[4];
        totalOutBytes += rowData[6];
        totalInPackets += rowData[3];
        totalOutPackets += rowData[5];
    }

    qint64 diffInBytes = totalInBytes - previousInBytes;
    qint64 diffOutBytes = totalOutBytes - previousOutBytes;
    qint64 diffInPackets = totalInPackets - previousInPackets;
    qint64 diffOutPackets = totalOutPackets - previousOutPackets;

    previousInBytes = totalInBytes;
    previousOutBytes = totalOutBytes;
    previousInPackets = totalInPackets;
    previousOutPackets = totalOutPackets;

    qreal diffInMB = static_cast<qreal>(diffInBytes) / 1048576;
    qreal diffOutMB = static_cast<qreal>(diffOutBytes) / 1048576;

    inPacketsQueue.enqueue(qMakePair(currentTime, diffInMB));
    outPacketsQueue.enqueue(qMakePair(currentTime, diffOutMB));
    inPacketsCountQueue.enqueue(qMakePair(currentTime, static_cast<qreal>(diffInPackets)));
    outPacketsCountQueue.enqueue(qMakePair(currentTime, static_cast<qreal>(diffOutPackets)));

    while (!inPacketsQueue.isEmpty() && inPacketsQueue.head().first < currentTime - 10000) {
        inPacketsQueue.dequeue();
    }
    while (!outPacketsQueue.isEmpty() && outPacketsQueue.head().first < currentTime - 10000) {
        outPacketsQueue.dequeue();
    }
    while (!inPacketsCountQueue.isEmpty() && inPacketsCountQueue.head().first < currentTime - 10000) {
        inPacketsCountQueue.dequeue();
    }
    while (!outPacketsCountQueue.isEmpty() && outPacketsCountQueue.head().first < currentTime - 10000) {
        outPacketsCountQueue.dequeue();
    }

    seriesIn->clear();
    seriesOut->clear();
    seriesInPackets->clear();
    seriesOutPackets->clear();

    qreal totalReceivedMB = 0;
    qreal totalSentMB = 0;
    qreal totalReceivedPackets = 0;
    qreal totalSentPackets = 0;

    for (const auto &dataPoint: inPacketsQueue) {
        seriesIn->append(static_cast<qreal>(dataPoint.first), dataPoint.second);
        totalReceivedMB += dataPoint.second;
    }

    for (const auto &dataPoint: outPacketsQueue) {
        seriesOut->append(static_cast<qreal>(dataPoint.first), dataPoint.second);
        totalSentMB += dataPoint.second;
    }
    for (const auto &dataPoint: inPacketsCountQueue) {
        seriesInPackets->append(static_cast<qreal>(dataPoint.first), dataPoint.second);
        totalReceivedPackets += dataPoint.second;
    }
    for (const auto &dataPoint: outPacketsCountQueue) {
        seriesOutPackets->append(static_cast<qreal>(dataPoint.first), dataPoint.second);
        totalSentPackets += dataPoint.second;
    }

    labelIn->setText(QString("<b>Data received in the last 10 seconds:</b> %1 <b>MB</b>").arg(totalReceivedMB, 0, 'f', 2));
    labelOut->setText(QString("<b>Data sent in the last 10 seconds:</b> %1 <b>MB</b>").arg(totalSentMB, 0, 'f', 2));
    labelInPackets->setText(QString("<b>Packets received in the last 10 seconds:</b> %1").arg(totalReceivedPackets, 0, 'f', 0));
    labelOutPackets->setText(QString("<b>Packets sent in the last 10 seconds:</b> %1").arg(totalSentPackets, 0, 'f', 0));

    qreal maxInMB = 0;
    qreal maxOutMB = 0;
    qreal maxInPackets = 0;
    qreal maxOutPackets = 0;

    for (const auto &dataPoint: inPacketsQueue) {
        if (dataPoint.second > maxInMB) maxInMB = dataPoint.second;
    }
    for (const auto &dataPoint: outPacketsQueue) {
        if (dataPoint.second > maxOutMB) maxOutMB = dataPoint.second;
    }
    for (const auto &dataPoint: inPacketsCountQueue) {
        if (dataPoint.second > maxInPackets) maxInPackets = dataPoint.second;
    }
    for (const auto &dataPoint: outPacketsCountQueue) {
        if (dataPoint.second > maxOutPackets) maxOutPackets = dataPoint.second;
    }

    QDateTime currentDateTime = QDateTime::currentDateTime();
    axisXData->setRange(currentDateTime.addSecs(-10), currentDateTime);
    axisXPackets->setRange(currentDateTime.addSecs(-10), currentDateTime);
    axisYData->setRange(0, std::max(maxInMB, maxOutMB) * 1.1);
    axisYPackets->setRange(0, std::max(maxInPackets, maxOutPackets) * 1.1);
}