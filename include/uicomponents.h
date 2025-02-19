#ifndef UICOMPONENTS_H
#define UICOMPONENTS_H

#include <QLabel>
#include <QCheckBox>
#include <QChartView>
#include <QHBoxLayout>
#include <QVBoxLayout>

class UIComponents {
public:
    UIComponents(QWidget *parent, QChart *chartData, QChart *chartPackets);

    QLabel *labelIn;
    QLabel *labelOut;
    QLabel *labelInPackets;
    QLabel *labelOutPackets;
    QCheckBox *checkBoxIn;
    QCheckBox *checkBoxOut;
    QCheckBox *checkBoxInPackets;
    QCheckBox *checkBoxOutPackets;
    QChartView *chartViewData;
    QChartView *chartViewPackets;
    QVBoxLayout *mainLayout;

private:
    void setupLabels();

    void setupCheckBoxes();

    void setupLayout(QWidget *parent, QChart *chartData, QChart *chartPackets);
};

#endif // UICOMPONENTS_H