#ifndef STATSPANEL_H
#define STATSPANEL_H

// Include all necessary Qt libraries
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QChartView>
#include <QMouseEvent>
#include <QPoint>

// For storing fitness data points (generation number + fitness value)
#include <vector>
#include <utility>

// Shows a fitness graph that tracks average fitness over generations
class StatsPanel : public QWidget
{
    Q_OBJECT

public:
    // Constructor
    explicit StatsPanel(QWidget *parent = nullptr);

    // Destructor
    ~StatsPanel();

    // Set the fitness value for the very first generation
    void setFirstGenerationWatchFitness(double fitness);

public slots:
    // Add a new data point to the graph (update the line)
    void updateAverageFitness(int generation, double avgFitness);

    // Clear the entire graph when we press Reset in the main window
    void clearGraph();

protected:
    // Detect mouse clicks on the graph to show generation info
    void mousePressEvent(QMouseEvent* event) override;

private:
    // Set up the top bar and layout of the panel
    void setupUI();

    // Set up the chart, axes, and line style
    void setupChart();

    // Show a pop-up with info for a specific generation
    void showGenerationInfo(int generation);

    // Layouts
    QVBoxLayout *mainLayout;
    QHBoxLayout *headerLayout;

    // Title text at the top
    QLabel *titleLabel;

    // Chart components
    QChart *fitnessChart;
    QLineSeries *avgFitnessSeries;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QChartView *chartView;

    // Store all fitness data points (generation, fitness value)
    std::vector<std::pair<int, double>> fitnessData;
};

#endif // STATSPANEL_H