#include "StatsPanel.h"
#include <QFont>
#include <QPen>
#include <QColor>
#include <QPainter>

// namespace WatchGA{
// namespace GUI{


StatsPanel::StatsPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupChart();
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
}

StatsPanel::~StatsPanel()
{}

void StatsPanel::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(8);

    headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(8);

    titleLabel = new QLabel("Average Fitness per Generation", this);
    QFont font = titleLabel->font();
    font.setBold(true);
    font.setPointSize(10);
    titleLabel->setFont(font);
    titleLabel->setStyleSheet("color: #222;");

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

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
        QPushButton:hover { background-color: #dee2e6; }
        QPushButton:pressed { background-color: #ced4da; }
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
    fitnessChart->setBackgroundBrush(QColor("#ffffff"));

    avgFitnessSeries = new QLineSeries();
    avgFitnessSeries->setName("Avg Fitness");
    avgFitnessSeries->setPen(QPen(QColor(0, 120, 212), 2));
    fitnessChart->addSeries(avgFitnessSeries);

    axisX = new QValueAxis();
    axisX->setTitleText("Generation");
    axisX->setLabelFormat("%d");
    axisX->setRange(0, 10);
    axisX->setGridLineColor(QColor("#e9ecef"));

    axisY = new QValueAxis();
    axisY->setTitleText("Average Fitness");
    axisY->setLabelFormat("%.2f");
    axisY->setRange(0, 1.0);
    axisY->setGridLineColor(QColor("#e9ecef"));

    fitnessChart->addAxis(axisX, Qt::AlignBottom);
    fitnessChart->addAxis(axisY, Qt::AlignLeft);
    avgFitnessSeries->attachAxis(axisX);
    avgFitnessSeries->attachAxis(axisY);

    chartView = new QChartView(fitnessChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(chartView);
}

void StatsPanel::setFirstGenerationWatchFitness(double fitness)
{
    avgFitnessSeries->clear();
    fitnessData.clear();
    updateAverageFitness(0, fitness);
}

void StatsPanel::updateAverageFitness(int generation, double avgFitness)
{
    fitnessData.emplace_back(generation, avgFitness);
    avgFitnessSeries->append(generation, avgFitness);

    if (generation > axisX->max())
        axisX->setMax(generation + 1);

    if (avgFitness > axisY->max())
        axisY->setMax(avgFitness * 1.1);

    fitnessChart->update();
    chartView->repaint();
}
// } //namespace GUI
// } // namespace WatchGA