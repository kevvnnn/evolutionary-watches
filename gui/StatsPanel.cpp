#include "StatsPanel.h"
#include "../fileio/EvolutionHistory.h"
#include <QFont>
#include <QPen>
#include <QColor>
#include <QPainter>
#include <QMessageBox>

// Points to the evolution history data from the main window
static const WatchGA::FileIO::EvolutionHistory* s_evolutionHistory = nullptr;

// Constructor - sets up the UI and chart when the panel is created
StatsPanel::StatsPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupChart();
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
}

// Destructor
StatsPanel::~StatsPanel()
{}

// Set up the top title bar and overall layout
void StatsPanel::setupUI()
{
    // Main layout that holds everything vertically
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(8);

    // Top bar layout for the title
    headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(8);

    // Title label at the top of the panel
    // titleLabel = new QLabel("Average Fitness per Generation", this);
    // QFont font = titleLabel->font();
    // font.setBold(true);
    // font.setPointSize(10);
    // titleLabel->setFont(font);
    // titleLabel->setStyleSheet("color: #222;");

    // Add title to the top bar
    // headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    // Add top bar to main layout
    mainLayout->addLayout(headerLayout);
}

// Set up the fitness trend chart, axes, and visual style
void StatsPanel::setupChart()
{
    // Create the chart object
    fitnessChart = new QChart();
    // fitnessChart->setTitle("Fitness Trend");
    fitnessChart->setTitleFont(QFont("Arial", 9));
    fitnessChart->legend()->setVisible(false);
    // fitnessChart->legend()->setAlignment(Qt::AlignBottom);
    fitnessChart->setBackgroundBrush(QColor("#ffffff"));

    // Create the blue line that shows fitness over time
    avgFitnessSeries = new QLineSeries();
    // avgFitnessSeries->setName("Avg Fitness");
    avgFitnessSeries->setPen(QPen(QColor(0, 120, 212), 2));
    fitnessChart->addSeries(avgFitnessSeries);

    // X axis = generation number
    axisX = new QValueAxis();
    axisX->setTitleText("Generation");
    axisX->setLabelFormat("%d");
    axisX->setRange(0, 10);
    axisX->setGridLineColor(QColor("#e9ecef"));

    // Y axis = fitness value (0 to 1)
    axisY = new QValueAxis();
    axisY->setTitleText("Average Fitness");
    axisY->setLabelFormat("%.2f");
    axisY->setRange(0, 1.0);
    axisY->setGridLineColor(QColor("#e9ecef"));

    // Attach axes to the chart
    fitnessChart->addAxis(axisX, Qt::AlignBottom);
    fitnessChart->addAxis(axisY, Qt::AlignLeft);
    avgFitnessSeries->attachAxis(axisX);
    avgFitnessSeries->attachAxis(axisY);

    // Create the view that displays the chart
    chartView = new QChartView(fitnessChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(chartView);
}

// Clear the graph and set the very first fitness value
void StatsPanel::setFirstGenerationWatchFitness(double fitness)
{
    avgFitnessSeries->clear();
    fitnessData.clear();
    updateAverageFitness(0, fitness);
}

// Add a new point to the graph and update the display
void StatsPanel::updateAverageFitness(int generation, double avgFitness)
{
    // Save the data point
    fitnessData.emplace_back(generation, avgFitness);

    // Add it to the line on the chart
    avgFitnessSeries->append(generation, avgFitness);

    // Expand the X axis if we go beyond the current range
    if (generation > axisX->max())
        axisX->setMax(generation + 1);

    // Expand the Y axis if fitness goes above the current maximum
    if (avgFitness > axisY->max())
        axisY->setMax(avgFitness * 1.1);

    // Refresh the chart to show changes
    fitnessChart->update();
    chartView->repaint();
}

// When the user clicks the graph, find which generation they clicked
void StatsPanel::mousePressEvent(QMouseEvent* event)
{
    QPointF clickPos = chartView->chart()->mapToValue(event->pos(), avgFitnessSeries);
    int gen = qRound(clickPos.x());

    if (gen >= 0)
        showGenerationInfo(gen);
}

// Show a pop-up with best, average, and worst fitness for a generation
void StatsPanel::showGenerationInfo(int generation)
{
    // If no history exists, show a message
    if (!s_evolutionHistory) {
        QMessageBox::information(this, "Info", "History not available");
        return;
    }

    try {
        // Get the saved data for this generation
        const auto& rec = s_evolutionHistory->getRecord(generation);

        // Make a message to display
        QString msg = QString(
            "Generation %1\n"
            "Best:  %2\n"
            "Avg:   %3\n"
            "Worst: %4"
        ).arg(generation)
         .arg(rec.bestFitness, 0, 'f', 2)
         .arg(rec.averageFitness, 0, 'f', 2)
         .arg(rec.worstFitness, 0, 'f', 2);

        // Show the message box
        QMessageBox::information(this, "Generation Data", msg);
    } catch (...) {
        QMessageBox::warning(this, "Error", "Invalid generation");
    }
}

// Lets the main window connect the evolution history to this panel
void setStatsPanelEvolutionHistory(const WatchGA::FileIO::EvolutionHistory* hist)
{
    s_evolutionHistory = hist;
}

// This function clears the entire graph when Reset is pressed
void StatsPanel::clearGraph()
{
    // Remove all points from the line
    avgFitnessSeries->clear();

    // Clear the stored data list
    fitnessData.clear();

    // Put the axes back to their original default ranges
    axisX->setMax(10);
    axisY->setMax(1.0);

    // Refresh the chart so it's completely empty
    fitnessChart->update();
    chartView->repaint();
}