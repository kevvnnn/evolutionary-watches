#include "MainWindow.h"
#include "WatchCanvas.h"
#include "ControlPanel.h"
#include "StatsPanel.h"
#include "../algorithm/GeneticAlgorithm.h"
#include "../genome/Watch.h"
#include "../fileio/EvolutionHistory.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <functional>
#include <QToolTip> // for step button pop up
#include <memory>
#include "../algorithm/CrossoverStrategy.h"
#include "../algorithm/MutationStrategy.h"
#include "../algorithm/SelectionStrategy.h"

// Access the same s_config as ControlPanel
namespace WatchGA {namespace GUI {extern FileIO::ConfigManager s_config;}}
void setStatsPanelEvolutionHistory(const WatchGA::FileIO::EvolutionHistory* hist);

// Selection Strategy Factory
std::unique_ptr<WatchGA::Core::ISelectionStrategy> createSelectionStrategy(const std::string& name)
{
    if (name == "Tournament") {
        return std::make_unique<WatchGA::Algorithm::TournamentSelection>(3);
    }
    if (name == "Roulette Wheel") {
        return std::make_unique<WatchGA::Algorithm::RouletteWheelSelection>();
    }
    // Default
    return std::make_unique<WatchGA::Algorithm::TournamentSelection>(3);
}

// Crossover Strategy Factory
std::unique_ptr<WatchGA::Core::ICrossoverStrategy> createCrossoverStrategy(const std::string& name)
{
    if (name == "One Point") {
        return std::make_unique<WatchGA::Algorithm::OnePointCrossover>();
    }
    if (name == "Uniform") {
        return std::make_unique<WatchGA::Algorithm::UniformCrossover>();
    }
    // Default
    return std::make_unique<WatchGA::Algorithm::OnePointCrossover>();
}

// Mutation Strategy Factory
std::unique_ptr<WatchGA::Core::IMutationStrategy> createMutationStrategy(const std::string& name)
{
    if (name == "Swap") {
        return std::make_unique<WatchGA::Algorithm::SwapMutation>();
    }
    if (name == "Parameter") {
        return std::make_unique<WatchGA::Algorithm::ParameterMutation>(0.1);
    }
    if (name == "AddRemove") {
        return std::make_unique<WatchGA::Algorithm::AddRemoveMutation>(50);
    }
    // Default
    return std::make_unique<WatchGA::Algorithm::SwapMutation>();
}

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
    // m_ga.reset(); // Starts population + evaluates generation 0

    // // Show BEST WATCH of GENERATION 0
    // watchCanvas->setWatch(m_ga.getBestWatch().get());
    // statsPanel->updateAverageFitness(0, m_ga.getAverageFitness());

    // =========================================================
    // STEP BUTTON — EVOLVE + SHOW BEST WATCH (REAL SYSTEM)
    // =========================================================
    connect(controlPanel, &WatchGA::GUI::ControlPanel::stepClicked, this, [this]() {
        // Start Population if at gen 0, DO THE SAME FOR RUN 
        if (m_currentGeneration == 0) {
            initializeGeneticAlgorithmFirstRun();
        } 

        qDebug() << "Step: Evolving Generation" << m_currentGeneration + 1;
        int nextGen = m_currentGeneration + 1;

        // Set up a tooltip popup
        QString suffix;
        if (nextGen % 10 == 1 && nextGen % 100 != 11) suffix = "st";
        else if (nextGen % 10 == 2 && nextGen % 100 != 12) suffix = "nd";
        else if (nextGen % 10 == 3 && nextGen % 100 != 13) suffix = "rd";
        else suffix = "th";

        // Show the popup
        QPoint topCenter = mapToGlobal(QPoint(width() / 2 - 100, 40));
        QToolTip::showText(topCenter, QString("Generating %1%2 generation...").arg(nextGen).arg(suffix), nullptr,QRect(), 1500);

        

        // 1. RUN 1 FULL EVOLUTIONARY GENERATION
        m_ga.runGeneration();
        WatchGA::FileIO::EvolutionHistory::GenerationRecord record; //following the struct
        record.generationNumber = m_currentGeneration + 1;
        record.bestFitness    = m_ga.getBestFitness();
        record.averageFitness = m_ga.getAverageFitness();
        record.worstFitness   = m_ga.getWorstFitness();
        record.bestWatch      = m_ga.getBestWatch();

        m_evolutionHistory.addRecord(record);

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

    connect(controlPanel, &WatchGA::GUI::ControlPanel::resetClicked, this, [this](){
        qDebug() << "Current Population: " << WatchGA::GUI::s_config.getInt("populationSize", 100);
    });

    // Connect GA callback to graph
    m_ga.setStatsCallback([this](int gen, double avg) {
        statsPanel->updateAverageFitness(gen, avg);
    });
}
void MainWindow::initializeGeneticAlgorithmFirstRun(){
    using namespace WatchGA::GUI;
    using namespace WatchGA::Algorithm;
    // Get latest values from the shared config
    int popSize = s_config.getInt("populationSize", 100);
    double mutRate = s_config.getDouble("mutationRate", 0.1);
    double crossRate = s_config.getDouble("crossoverRate", 0.8);
    int elitism = s_config.getInt("elitismCount", 2);
    std::string selection = s_config.getString("selectionStrategy", "Tournament");
    std::string crossover = s_config.getString("crossoverStrategy", "One Point");
    std::string mutation = s_config.getString("mutationStrategy", "Swap");
    // Apply the configs to the GA
    m_ga.setPopulationSize(popSize);
    m_ga.setMutationRate(mutRate);
    m_ga.setCrossoverRate(crossRate);
    m_ga.setElitismCount(elitism);
    m_ga.setSelectionStrategy(createSelectionStrategy(selection));
    m_ga.setCrossoverStrategy(createCrossoverStrategy(crossover));
    m_ga.setMutationStrategy(createMutationStrategy(mutation));
    m_ga.reset();
}