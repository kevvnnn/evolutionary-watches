#ifndef STATSPANEL_H
#define STATSPANEL_H

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

/**
 * @brief Shows a real-time fitness graph during evolution
 *
 * Displays average fitness across generations.
 * Supports clicking to view detailed generation stats.
 */
class StatsPanel : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor - sets up graph and layout
     * @param parent Parent widget
     */
    explicit StatsPanel(QWidget *parent = nullptr);

    ~StatsPanel() override;

    /**
     * @brief Sets initial fitness for generation 0
     * @param fitness Starting fitness value
     */
    void setFirstGenerationWatchFitness(double fitness);

public slots:
    /**
     * @brief Adds a new data point to the fitness graph
     * @param generation Current generation number
     * @param avgFitness Average fitness of the population
     */
    void updateAverageFitness(int generation, double avgFitness);

    /**
     * @brief Clears all data and resets the graph
     */
    void clearGraph();

protected:
    /**
     * @brief Handles mouse clicks on the graph
     * @param event Mouse event
     */
    void mousePressEvent(QMouseEvent* event) override;

private:
    // Set up the top bar and layout of the panel
    void setupUI();

    /// Sets up chart, axes, and visual style
    void setupChart();

    /// Shows detailed stats for a clicked generation
    void showGenerationInfo(int generation);

    // Layouts
    QVBoxLayout *mainLayout;
    QHBoxLayout *headerLayout;

    // UI
    QLabel *titleLabel;

    // Chart
    QChart *fitnessChart;
    QLineSeries *avgFitnessSeries;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QChartView *chartView;

    // Store all fitness data points (generation, fitness value)
    std::vector<std::pair<int, double>> fitnessData;
};

#endif // STATSPANEL_H