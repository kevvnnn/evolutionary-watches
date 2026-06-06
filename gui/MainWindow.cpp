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

    watchCanvas = new WatchCanvas;
    watchCanvas->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    leftLayout->addWidget(watchCanvas);

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
        int newGen = m_currentGeneration + 1;
        qDebug() << "Step to generation:" << newGen;

        // graph update
        statsPanel->updateAverageFitness(newGen, 0.2 + (newGen * 0.1));

        // watch update 
        WatchGA::Genome::Watch* newWatch = new WatchGA::Genome::Watch();
        newWatch->randomize();
        watchCanvas->setWatch(newWatch);

        m_currentGeneration++;
    });

    // GA Callback
    WatchGA::Algorithm::GeneticAlgorithm ga;
    ga.setStatsCallback([this](int generation, double avgFitness) {
        statsPanel->updateAverageFitness(generation, avgFitness);
    });

    rightLayout->addStretch();
    mainLayout->addWidget(rightContainer);

    // Initial watch
    WatchGA::Genome::Watch* watch = new WatchGA::Genome::Watch();
    watch->randomize();
    watchCanvas->setWatch(watch);

    double realWatchFitness = watch->getFitnessScore();
    statsPanel->setFirstGenerationWatchFitness(realWatchFitness);
}