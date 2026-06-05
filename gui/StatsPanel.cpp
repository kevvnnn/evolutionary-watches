#include "StatsPanel.h"
#include <QFont>
#include <QPen>
#include <QColor>
#include <QPainter>

StatsPanel::StatsPanel(QWidget *parent)
    : QWidget(parent)
{

    // setWindowTitle("Average Fitness");
    // setFixedSize(400, 260); // More compact size
    setupUI();
    setupChart();

    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
}

StatsPanel::~StatsPanel() = default;

void StatsPanel::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10); // Clean, balanced margins
    mainLayout->setSpacing(8);

    // Header Bar (matches your app's clean look)
    headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(8);

    titleLabel = new QLabel("Average Fitness per Generation", this);
    QFont font = titleLabel->font();
    font.setBold(true);
    font.setPointSize(10);
    titleLabel->setFont(font);
    titleLabel->setStyleSheet("color: #222;"); // Dark text for readability

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch(); // Push close button to right

    closeButton = new QPushButton("Close", this);
    closeButton->setStyleSheet(R"(
        QPushButton {
            background-color: #e9ecef;
            border: 1px solid #ced4da;
            border-radius: 3px;
            padding: 4px 12px;
            font-size: 9pt;
            color: #212529;
        }
        QPushButton:hover {
            background-color: #dee2e6;
        }
        QPushButton:pressed {
            background-color: #ced4da;
        }
    )");
    headerLayout->addWidget(closeButton);
    connect(closeButton, &QPushButton::clicked, this, &StatsPanel::close);

    mainLayout->addLayout(headerLayout);
}

void StatsPanel::setupChart()
{
    fitnessChart = new QChart();
    fitnessChart->setTitle("Fitness Trend");
    fitnessChart->setTitleFont(QFont("Arial", 9));
    fitnessChart->legend()->setVisible(true);
    fitnessChart->legend()->setAlignment(Qt::AlignBottom);
    fitnessChart->legend()->setFont(QFont("Arial", 8));
    fitnessChart->setBackgroundBrush(QColor("#ffffff")); // Pure white background
    fitnessChart->setPlotAreaBackgroundBrush(QColor("#ffffff"));
    fitnessChart->setMargins(QMargins(5, 5, 5, 5)); // Tight chart margins

    avgFitnessSeries = new QLineSeries();
    avgFitnessSeries->setName("Avg Fitness");
    avgFitnessSeries->setPen(QPen(QColor(0, 120, 212), 2)); // Professional blue (matches your app's button blue)

    fitnessChart->addSeries(avgFitnessSeries);

    // X Axis (Generation)
    axisX = new QValueAxis();
    axisX->setTitleText("Generation");
    axisX->setTitleFont(QFont("Arial", 7));
    axisX->setLabelFormat("%d");
    axisX->setRange(0, 10);
    axisX->setLabelsFont(QFont("Arial", 7));
    axisX->setGridLineColor(QColor("#e9ecef")); // Light gray grid lines
    axisX->setLinePenColor(QColor("#adb5bd"));

    // Y Axis (Average Fitness)
    axisY = new QValueAxis();
    axisY->setTitleText("Average Fitness");
    axisY->setTitleFont(QFont("Arial", 7));
    axisY->setLabelFormat("%.2f");
    axisY->setRange(0, 1.0);
    axisY->setLabelsFont(QFont("Arial", 7));
    axisY->setGridLineColor(QColor("#e9ecef")); // Light gray grid lines
    axisY->setLinePenColor(QColor("#adb5bd"));

    fitnessChart->addAxis(axisX, Qt::AlignBottom);
    fitnessChart->addAxis(axisY, Qt::AlignLeft);
    avgFitnessSeries->attachAxis(axisX);
    avgFitnessSeries->attachAxis(axisY);

    chartView = new QChartView(fitnessChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedSize(360, 160); // Remove any default border
    mainLayout->addWidget(chartView);
}

void StatsPanel::updateAverageFitness(int generation, double avgFitness)
{
    fitnessData.emplace_back(generation, avgFitness);
    avgFitnessSeries->append(generation, avgFitness);

    // Auto-scale axes
    if (generation > axisX->max())
        axisX->setMax(generation + 1);

    if (avgFitness > axisY->max())
        axisY->setMax(avgFitness * 1.1);
    if (avgFitness < axisY->min())
        axisY->setMin(avgFitness * 0.9);

    fitnessChart->update();
}