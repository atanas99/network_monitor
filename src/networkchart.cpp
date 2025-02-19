#include "../include/networkchart.h"
#include "../include/chartsetup.h"
#include "../include/axissetup.h"
#include "../include/uicomponents.h"
#include "../include/dataupdater.h"

NetworkChart::NetworkChart(QWidget *parent) : QWidget(parent) {
    chartSetup = new ChartSetup();
    axisSetup = new AxisSetup(chartSetup->chartData, chartSetup->chartPackets,
                              chartSetup->seriesIn, chartSetup->seriesOut,
                              chartSetup->seriesInPackets, chartSetup->seriesOutPackets);
    uiComponents = new UIComponents(this, chartSetup->chartData, chartSetup->chartPackets);
    dataUpdater = new DataUpdater(chartSetup->seriesIn, chartSetup->seriesOut,
                                  chartSetup->seriesInPackets, chartSetup->seriesOutPackets,
                                  axisSetup->axisXData, axisSetup->axisXPackets,
                                  axisSetup->axisYData, axisSetup->axisYPackets,
                                  uiComponents->labelIn, uiComponents->labelOut,
                                  uiComponents->labelInPackets, uiComponents->labelOutPackets);

    connect(uiComponents->checkBoxIn, &QCheckBox::toggled, this, &NetworkChart::toggleSeriesVisibility);
    connect(uiComponents->checkBoxOut, &QCheckBox::toggled, this, &NetworkChart::toggleSeriesVisibility);
    connect(uiComponents->checkBoxInPackets, &QCheckBox::toggled, this, &NetworkChart::toggleSeriesVisibility);
    connect(uiComponents->checkBoxOutPackets, &QCheckBox::toggled, this, &NetworkChart::toggleSeriesVisibility);

    setLayout(uiComponents->mainLayout);
}

void NetworkChart::toggleSeriesVisibility() {
    chartSetup->seriesIn->setVisible(uiComponents->checkBoxIn->isChecked());
    chartSetup->seriesOut->setVisible(uiComponents->checkBoxOut->isChecked());
    chartSetup->seriesInPackets->setVisible(uiComponents->checkBoxInPackets->isChecked());
    chartSetup->seriesOutPackets->setVisible(uiComponents->checkBoxOutPackets->isChecked());

    if (uiComponents->checkBoxIn->isChecked() && !chartSetup->seriesIn->attachedAxes().contains(axisSetup->axisXData))
        chartSetup->seriesIn->attachAxis(axisSetup->axisXData);
    if (uiComponents->checkBoxOut->isChecked() && !chartSetup->seriesOut->attachedAxes().contains(axisSetup->axisXData))
        chartSetup->seriesOut->attachAxis(axisSetup->axisXData);
    if (uiComponents->checkBoxInPackets->isChecked() &&
        !chartSetup->seriesInPackets->attachedAxes().contains(axisSetup->axisXPackets))
        chartSetup->seriesInPackets->attachAxis(axisSetup->axisXPackets);
    if (uiComponents->checkBoxOutPackets->isChecked() &&
        !chartSetup->seriesOutPackets->attachedAxes().contains(axisSetup->axisXPackets))
        chartSetup->seriesOutPackets->attachAxis(axisSetup->axisXPackets);

    uiComponents->chartViewPackets->setVisible(
            uiComponents->checkBoxInPackets->isChecked() || uiComponents->checkBoxOutPackets->isChecked());
    uiComponents->chartViewData->setVisible(
            uiComponents->checkBoxIn->isChecked() || uiComponents->checkBoxOut->isChecked());

    if (uiComponents->chartViewData->isVisible() || uiComponents->chartViewPackets->isVisible()) {
        uiComponents->chartViewData->setFixedSize(600, 300);
        uiComponents->chartViewPackets->setFixedSize(600, 300);
    }
}

void NetworkChart::updateNetworkInfo(const QList<QList<qint64>> &networkData) {
    dataUpdater->updateNetworkInfo(networkData);
}