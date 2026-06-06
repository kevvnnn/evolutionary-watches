#ifndef STATSPANEL_H
#define STATSPANEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <vector>
#include <utility>

// ✅ Correct Qt6 Charts includes (NO NAMESPACE MACROS!)
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QChartView>

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

    // ✅ Qt6 — NO QtCharts:: NAMESPACE!
    QChart*          fitnessChart;
    QChartView*      chartView;
    QLineSeries*     avgFitnessSeries;
    QValueAxis*      axisX;
    QValueAxis*      axisY;

    std::vector<std::pair<int, double>> fitnessData;
};

#endif