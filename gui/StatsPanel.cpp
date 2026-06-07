#include "StatsPanel.h"
#include "../fileio/EvolutionHistory.h"
#include <QFont>
#include <QPen>
#include <QColor>
#include <QPainter>
#include <QMessageBox>

// pointer to history (we'll set from MainWindow)
static const WatchGA::FileIO::EvolutionHistory* s_evolutionHistory = nullptr;

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

void StatsPanel::mousePressEvent(QMouseEvent* event)
{
    QPointF clickPos = chartView->chart()->mapToValue(event->pos(), avgFitnessSeries);
    int gen = qRound(clickPos.x());

    if (gen >= 0)
        showGenerationInfo(gen);
}

// ✅ SHOW REAL GENERATION HISTORY
void StatsPanel::showGenerationInfo(int generation)
{
    if (!s_evolutionHistory) {
        QMessageBox::information(this, "Info", "History not available");
        return;
    }

    try {
        const auto& rec = s_evolutionHistory->getRecord(generation);
        QString msg = QString(
            "Generation %1\n"
            "Best:  %2\n"
            "Avg:   %3\n"
            "Worst: %4"
        ).arg(generation)
         .arg(rec.bestFitness, 0, 'f', 2)
         .arg(rec.averageFitness, 0, 'f', 2)
         .arg(rec.worstFitness, 0, 'f', 2);

        QMessageBox::information(this, "Generation Data", msg);
    } catch (...) {
        QMessageBox::warning(this, "Error", "Invalid generation");
    }
}

// functionality to set history from MainWindow
void setStatsPanelEvolutionHistory(const WatchGA::FileIO::EvolutionHistory* hist)
{
    s_evolutionHistory = hist;
}