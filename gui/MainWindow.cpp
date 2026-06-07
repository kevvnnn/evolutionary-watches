#include "MainWindow.h"
#include "WatchCanvas.h"
#include "ControlPanel.h"
#include "StatsPanel.h"
#include "GeneticAlgorithm.h"
#include "../genome/Watch.h"
#include "../fileio/EvolutionHistory.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <functional>
#include <QToolTip>
void setStatsPanelEvolutionHistory(const WatchGA::FileIO::EvolutionHistory* hist);

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
    setStatsPanelEvolutionHistory(&m_evolutionHistory);
    statsPanel->setFixedSize(450, 260);
    rightLayout->addWidget(statsPanel);

    rightLayout->addStretch();
    mainLayout->addWidget(rightContainer);

    // =========================================================
    // INITIALIZE REAL GENETIC ALGORITHM
    // =========================================================
    m_currentGeneration = 0;

    // =========================================================
    // STEP BUTTON — EVOLVE + SHOW BEST WATCH (REAL SYSTEM)
    // =========================================================
    connect(controlPanel, &WatchGA::GUI::ControlPanel::stepClicked, this, [this]() {
        if (m_currentGeneration == 0) m_ga.reset();

        qDebug() << "Step: Evolving Generation" << m_currentGeneration + 1;
        int nextGen = m_currentGeneration + 1;

        QString suffix;
        if (nextGen % 10 == 1 && nextGen % 100 != 11) suffix = "st";
        else if (nextGen % 10 == 2 && nextGen % 100 != 12) suffix = "nd";
        else if (nextGen % 10 == 3 && nextGen % 100 != 13) suffix = "rd";
        else suffix = "th";

        QPoint topCenter = mapToGlobal(QPoint(width() / 2 - 100, 40));
        QToolTip::showText(topCenter, QString("Generating %1%2 generation...").arg(nextGen).arg(suffix), nullptr,QRect(), 1500);

        m_ga.runGeneration();
        WatchGA::FileIO::EvolutionHistory::GenerationRecord record;
        record.generationNumber = m_currentGeneration + 1;
        record.bestFitness    = m_ga.getBestFitness();
        record.averageFitness = m_ga.getAverageFitness();
        record.worstFitness   = m_ga.getWorstFitness();
        record.bestWatch      = m_ga.getBestWatch();

        m_evolutionHistory.addRecord(record);

        auto bestWatch = m_ga.getBestWatch();

        if (bestWatch) {
            watchCanvas->setWatch(bestWatch.get());
        }

        m_currentGeneration++;
        statsPanel->updateAverageFitness(m_currentGeneration, m_ga.getAverageFitness());

        // // NEW: COSMETIC ONLY - Update canvas with current generation
        // watchCanvas->setGeneration(m_currentGeneration);
    });

    m_ga.setStatsCallback([this](int gen, double avg) {
        statsPanel->updateAverageFitness(gen, avg);
    });
}