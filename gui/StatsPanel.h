#ifndef STATSPANEL_H
#define STATSPANEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <vector>
#include <utility>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QChartView>

// For Clicking in graph
#include <QPoint>
#include <QMouseEvent>

class StatsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit StatsPanel(QWidget *parent = nullptr);
    ~StatsPanel() override;

    void updateAverageFitness(int generation, double avgFitness);
    void setFirstGenerationWatchFitness(double fitness);

private:
    void setupUI();
    void setupChart();

    QVBoxLayout *mainLayout;
    QHBoxLayout *headerLayout;
    QLabel *titleLabel;
    QPushButton *closeButton;
    QChart*          fitnessChart;
    QChartView*      chartView;
    QLineSeries*     avgFitnessSeries;
    QValueAxis*      axisX;
    QValueAxis*      axisY;

    std::vector<std::pair<int, double>> fitnessData;
    // for showing info
    void showGenerationInfo(int generation);

protected:
    void mousePressEvent(QMouseEvent* event) override;
};

#endif