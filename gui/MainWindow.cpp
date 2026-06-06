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
    setWindowTitle("WatchGA - Evolution Viewer");
    setFixedSize(1166, 760);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(30);

    // LEFT: Watch Canvas
    QWidget* leftContainer = new QWidget;
    QHBoxLayout* leftLayout = new QHBoxLayout(leftContainer);
    leftLayout->setAlignment(Qt::AlignCenter);

    watchCanvas = new WatchCanvas;
    watchCanvas->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    leftLayout->addWidget(watchCanvas);
    mainLayout->addWidget(leftContainer);

    // RIGHT: Panels
    QWidget* rightContainer = new QWidget;
    QVBoxLayout* rightLayout = new QVBoxLayout(rightContainer);

    WatchGA::GUI::ControlPanel* controlPanel = new WatchGA::GUI::ControlPanel;
    rightLayout->addWidget(controlPanel);

    statsPanel = new StatsPanel(rightContainer);
    statsPanel->setFixedSize(450, 260);
    rightLayout->addWidget(statsPanel);

    rightLayout->addStretch();
    mainLayout->addWidget(rightContainer);

    // =========================================================
    // ✅ INITIALIZE REAL GENETIC ALGORITHM
    // =========================================================
    m_currentGeneration = 0;
    m_ga.reset(); // Starts population + evaluates generation 0

    // Show BEST WATCH of GENERATION 0
    watchCanvas->setWatch(m_ga.getBestWatch().get());
    statsPanel->updateAverageFitness(0, m_ga.getAverageFitness());

    // =========================================================
    // ✅ STEP BUTTON — EVOLVE + SHOW BEST WATCH (REAL SYSTEM)
    // =========================================================
    connect(controlPanel, &WatchGA::GUI::ControlPanel::stepClicked, this, [this]() {
        qDebug() << "Step: Evolving Generation" << m_currentGeneration + 1;

        // 1. RUN 1 FULL EVOLUTIONARY GENERATION
        m_ga.runGeneration();

        // 2. GET THE BEST WATCH OF THE NEW GENERATION
        auto bestWatch = m_ga.getBestWatch();

        // 3. DISPLAY BEST WATCH ON CANVAS
        if (bestWatch) {
            watchCanvas->setWatch(bestWatch.get());
        }

        // 4. UPDATE GRAPH WITH REAL FITNESS
        m_currentGeneration++;
        statsPanel->updateAverageFitness(m_currentGeneration, m_ga.getAverageFitness());
    });

    // Connect GA callback to graph
    m_ga.setStatsCallback([this](int gen, double avg) {
        statsPanel->updateAverageFitness(gen, avg);
    });
}