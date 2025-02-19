#include "../include/uicomponents.h"
#include <QChartView>
#include <QVBoxLayout>
#include <QHBoxLayout>

UIComponents::UIComponents(QWidget *parent, QChart *chartData, QChart *chartPackets) {
    setupLabels();
    setupCheckBoxes();
    setupLayout(parent, chartData, chartPackets);
}

void UIComponents::setupLabels() {
    labelIn = new QLabel();
    labelOut = new QLabel();
    labelInPackets = new QLabel();
    labelOutPackets = new QLabel();
    labelIn->setStyleSheet("QLabel { color : white; }");
    labelOut->setStyleSheet("QLabel { color : white; }");
    labelInPackets->setStyleSheet("QLabel { color : white; }");
    labelOutPackets->setStyleSheet("QLabel { color : white; }");
}

void UIComponents::setupCheckBoxes() {
    checkBoxIn = new QCheckBox("Received Data");
    checkBoxOut = new QCheckBox("Sent Data");
    checkBoxInPackets = new QCheckBox("Received Packets");
    checkBoxOutPackets = new QCheckBox("Sent Packets");
    checkBoxIn->setChecked(true);
    checkBoxOut->setChecked(true);
    checkBoxInPackets->setChecked(true);
    checkBoxOutPackets->setChecked(true);
}

void UIComponents::setupLayout(QWidget *parent, QChart *chartData, QChart *chartPackets) {
    chartViewData = new QChartView(chartData);
    chartViewPackets = new QChartView(chartPackets);
    chartViewData->setRenderHint(QPainter::Antialiasing);
    chartViewPackets->setRenderHint(QPainter::Antialiasing);
    chartViewData->setFixedSize(600, 300);
    chartViewPackets->setFixedSize(600, 300);

    auto *checkBoxLayout = new QHBoxLayout();
    checkBoxLayout->addWidget(checkBoxIn);
    checkBoxLayout->addWidget(checkBoxOut);
    checkBoxLayout->addWidget(checkBoxInPackets);
    checkBoxLayout->addWidget(checkBoxOutPackets);

    auto *labelLayout = new QHBoxLayout();
    labelLayout->addWidget(labelIn);
    labelLayout->addWidget(labelOut);
    labelLayout->addWidget(labelInPackets);
    labelLayout->addWidget(labelOutPackets);

    auto *chartsLayout = new QHBoxLayout();
    chartsLayout->addWidget(chartViewData);
    chartsLayout->addWidget(chartViewPackets);

    mainLayout = new QVBoxLayout(parent);
    mainLayout->addLayout(checkBoxLayout);
    mainLayout->addLayout(labelLayout);
    mainLayout->addLayout(chartsLayout);
}