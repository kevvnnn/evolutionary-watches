#ifndef STATSPANEL_H
#define STATSPANEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QChartView>
#include <vector>

class StatsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit StatsPanel(QWidget *parent = nullptr);
    ~StatsPanel() override;

    void updateAverageFitness(int generation, double avgFitness);

private:
    void setupUI();
    void setupChart();

    QVBoxLayout *mainLayout;
    QHBoxLayout *headerLayout;
    QLabel *titleLabel;
    QPushButton *closeButton;

    QChart *fitnessChart;
    QChartView *chartView;
    QLineSeries *avgFitnessSeries;
    QValueAxis *axisX;
    QValueAxis *axisY;

    std::vector<std::pair<int, double>> fitnessData;
};

#endif