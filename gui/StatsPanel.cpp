#include "StatsPanel.h"
#include <QFont>
#include <QPen>
#include <QColor>
#include <QPainter>

StatsPanel::StatsPanel(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Fitness Stats");
    setFixedSize(420, 280);

    setupUI();
    setupChart();
}

StatsPanel::~StatsPanel() = default;

void StatsPanel::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(8);

    headerLayout = new QHBoxLayout();
    titleLabel = new QLabel("Average Fitness per Generation", this);
    QFont font = titleLabel->font();
    font.setBold(true);
    font.setPointSize(11);
    titleLabel->setFont(font);

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    closeButton = new QPushButton("Close", this);
    headerLayout->addWidget(closeButton);
    connect(closeButton, &QPushButton::clicked, this, &StatsPanel::close);

    mainLayout->addLayout(headerLayout);
}

void StatsPanel::setupChart()
{
    fitnessChart = new QChart();
    avgFitnessSeries = new QLineSeries();
    avgFitnessSeries->setName("Avg Fitness");
    avgFitnessSeries->setPen(QPen(QColor(40, 180, 80), 2));

    fitnessChart->addSeries(avgFitnessSeries);
    fitnessChart->setTitle("Fitness Trend");
    fitnessChart->legend()->setVisible(true);
    fitnessChart->legend()->setAlignment(Qt::AlignBottom);

    axisX = new QValueAxis();
    axisX->setTitleText("Generation");
    axisX->setLabelFormat("%d");
    axisX->setRange(0, 10);

    axisY = new QValueAxis();
    axisY->setTitleText("Average Fitness");
    axisY->setLabelFormat("%.2f");
    axisY->setRange(0, 1);

    fitnessChart->addAxis(axisX, Qt::AlignBottom);
    fitnessChart->addAxis(axisY, Qt::AlignLeft);
    avgFitnessSeries->attachAxis(axisX);
    avgFitnessSeries->attachAxis(axisY);

    chartView = new QChartView(fitnessChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(chartView);
}

void StatsPanel::updateAverageFitness(int generation, double avgFitness)
{
    fitnessData.emplace_back(generation, avgFitness);
    avgFitnessSeries->append(generation, avgFitness);

    if (generation > axisX->max())
        axisX->setMax(generation + 1);

    if (avgFitness > axisY->max())
        axisY->setMax(avgFitness * 1.1);
    if (avgFitness < axisY->min())
        axisY->setMin(avgFitness * 0.9);

    fitnessChart->update();
}