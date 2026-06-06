#include "MainWindow.h"
#include "WatchCanvas.h"
#include "ControlPanel.h"
#include "StatsPanel.h"
#include "GeneticAlgorithm.h"
#include "../genome/Watch.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <functional>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("WatchGA - Component Inspector");
    setFixedSize(1166, 760);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(30);

    // LEFT: Watch Canvas
    QWidget* leftContainer = new QWidget;
    QHBoxLayout* leftLayout = new QHBoxLayout(leftContainer);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setAlignment(Qt::AlignCenter);
    leftLayout->addStretch();

    WatchCanvas* canvas = new WatchCanvas;
    canvas->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    leftLayout->addWidget(canvas);

    leftLayout->addStretch();
    mainLayout->addWidget(leftContainer);

    // RIGHT: Panels
    QWidget* rightContainer = new QWidget;
    QVBoxLayout* rightLayout = new QVBoxLayout(rightContainer);
    rightLayout->setSpacing(15);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    WatchGA::GUI::ControlPanel* controlPanel = new WatchGA::GUI::ControlPanel;
    rightLayout->addWidget(controlPanel);

    statsPanel = new StatsPanel(rightContainer);
    statsPanel->setFixedSize(450, 260);
    rightLayout->addWidget(statsPanel);

    m_currentGeneration = 0;
    m_evolutionHistory.setExperimentName("WatchGA Test");

    connect(controlPanel, &WatchGA::GUI::ControlPanel::stepClicked, this, [this]() {
        qDebug() << "MainWindow handling step...";

        // Generate ONE NEW GENERATION
        m_evolutionHistory.generateDummyData(m_currentGeneration + 1);

        // Get the new record
        const auto& record = m_evolutionHistory.getRecord(m_currentGeneration);

        // Update the graph
        statsPanel->updateAverageFitness(record.generationNumber, record.averageFitness);

        m_currentGeneration++;
    });

    // GA Callback
    WatchGA::Algorithm::GeneticAlgorithm ga;
    ga.setStatsCallback([this](int generation, double avgFitness) {
        statsPanel->updateAverageFitness(generation, avgFitness);
    });

    rightLayout->addStretch();
    mainLayout->addWidget(rightContainer);

    // =========================================================
    // CREATE WATCH + PASS REAL FITNESS TO STATS PANEL
    // =========================================================
    WatchGA::Genome::Watch* watch = new WatchGA::Genome::Watch();
    watch->randomize();

    // Send Watch to canvas
    canvas->setWatch(watch);

    // Send REAL WATCH FITNESS to StatsPanel (Generation 0)
    double realWatchFitness = watch->getFitnessScore();
    statsPanel->setFirstGenerationWatchFitness(realWatchFitness);

    //dummy value to see if the graph can actually show lines
    // statsPanel->updateAverageFitness(0, 0.2);
    // statsPanel->updateAverageFitness(1, 0.5);
    // statsPanel->updateAverageFitness(2, 0.8);
}