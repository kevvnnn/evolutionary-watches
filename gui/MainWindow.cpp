#include "MainWindow.h"
#include "WatchCanvas.h"
#include "ControlPanel.h"
#include "StatsPanel.h"
#include "../genome/Watch.h"
#include "../fileio/EvolutionHistory.h"
#include "../algorithm/GeneticAlgorithm.h"
#include "../algorithm/CrossoverStrategy.h"
#include "../algorithm/MutationStrategy.h"
#include "../algorithm/SelectionStrategy.h"
#include "../fileio/WatchFileIO.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <functional>
#include <QToolTip> // for step button pop up
#include <memory>
#include <QPushButton>
#include <QFileDialog>
#include <QScrollArea> // for resizable windows

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
    return std::make_unique<WatchGA::Algorithm::ParameterMutation>(0.1);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
setWindowTitle("WatchGA - Evolution Viewer");
    resize(1000, 750); 

    // Timer Initialization
    m_runTimer = new QTimer(this);
    m_runTimer->setInterval(300); // 0.3 seconds delay
    connect(m_runTimer, &QTimer::timeout, this, &MainWindow::runOneGeneration);
    m_isRunning = false;

    // Wrapping the GUI in a scroll area so it is resizable
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);       // Lets the inner UI breathe
    scrollArea->setFrameShape(QFrame::NoFrame); // Removes ugly inset borders

    QWidget* centralWidget = new QWidget(scrollArea);
    scrollArea->setWidget(centralWidget);
    setCentralWidget(scrollArea);               // Set the scroll area as the core widget

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

    controlPanel = new WatchGA::GUI::ControlPanel;
    rightLayout->addWidget(controlPanel);

    statsPanel = new StatsPanel(rightContainer);
    setStatsPanelEvolutionHistory(&m_evolutionHistory);
    statsPanel->setFixedSize(450, 230);
    rightLayout->addWidget(statsPanel);

    // Save & Load Buttons
    QWidget *SaveLoadWatchRow = new QWidget;
    QHBoxLayout* buttonLayout = new QHBoxLayout(SaveLoadWatchRow);
    buttonLayout->setSpacing(10);
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton* btnSaveWatch = new QPushButton("Save Best Watch");
    QPushButton* btnLoadWatch = new QPushButton("Load Watch");
    btnSaveWatch->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    btnLoadWatch->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    buttonLayout->addWidget(btnSaveWatch);
    buttonLayout->addWidget(btnLoadWatch);
    rightLayout->addWidget(SaveLoadWatchRow);

    QPushButton* btnLogHistory = new QPushButton("Save Evolution History");
    rightLayout->addWidget(btnLogHistory);

    rightLayout->addStretch();
    mainLayout->addWidget(rightContainer);
    
    // --------------------------
    // SAVE BEST WATCH
    // --------------------------
    connect(btnSaveWatch, &QPushButton::clicked, this, [this]() {
        auto bestWatch = m_ga.getBestWatch();
        if (!bestWatch) {
            QToolTip::showText(mapToGlobal(rect().center()), "No watch to save!", nullptr, QRect(), 2000);
            return;
        }

        QString path = QFileDialog::getSaveFileName(this, "Save Best Watch", "", "Binary Watch (*.bin)");
        if (path.isEmpty()) return;

        WatchGA::FileIO::WatchFileIO saver(path.toStdString());
        bool ok = saver.saveWatch(*bestWatch, 0);

        if (ok)
            QToolTip::showText(mapToGlobal(rect().center()), "Watch saved!", nullptr, QRect(), 2000);
        else
            QToolTip::showText(mapToGlobal(rect().center()), "Save failed!", nullptr, QRect(), 2000);
    });

    // --------------------------
    // LOAD WATCH
    // --------------------------
    connect(btnLoadWatch, &QPushButton::clicked, this, [this]() {
        QString path = QFileDialog::getOpenFileName(this, "Load Watch", "", "Binary Watch (*.bin)");
        if (path.isEmpty()) return;

        WatchGA::FileIO::WatchFileIO loader(path.toStdString());
        
        // FIX: Transfer ownership to the MainWindow variable so it doesn't get destroyed!
        m_loadedWatch = loader.loadWatch(0);

        if (m_loadedWatch) {
            watchCanvas->setWatch(m_loadedWatch.get());
            QToolTip::showText(mapToGlobal(rect().center()), "Watch loaded!", nullptr, QRect(), 2000);
        } else {
            QToolTip::showText(mapToGlobal(rect().center()), "Load failed!", nullptr, QRect(), 2000);
        }
    });
    
    // =========================================================
    // INITIALIZE REAL GENETIC ALGORITHM
    // =========================================================
    m_currentGeneration = 0;

    // =========================================================
    // STEP BUTTON — EVOLVE + SHOW BEST WATCH (REAL SYSTEM)
    // =========================================================
    connect(controlPanel, &WatchGA::GUI::ControlPanel::stepClicked, this, [this]() {
        // Start Population if at gen 0, DO THE SAME FOR RUN 
        runOneGeneration();
    });

    // RUN BUTTON (AUTO EVOLVE)
    connect(controlPanel, &WatchGA::GUI::ControlPanel::runClicked, this, [this]() {
        if (!m_isRunning) {
            m_isRunning = true;
            m_runTimer->start();
            qDebug() << "Started Auto Run";
        }
    });

    // STOP BUTTON
    connect(controlPanel, &WatchGA::GUI::ControlPanel::pauseClicked, this, [this]() {
        m_isRunning = false;
        m_runTimer->stop();
        qDebug() << "Stopped Auto Run";
    });

    connect(controlPanel, &WatchGA::GUI::ControlPanel::resetClicked, this, [this](){
        resetEvolution();
    });

    connect(btnLogHistory, &QPushButton::clicked, this, [this](){
        qDebug() << "Save Log clicked";
        // Check if theres history to save
        if (m_evolutionHistory.getRecordCount() == 0) {
            QToolTip::showText(mapToGlobal(rect().center()),"No evolution history to save!", nullptr, QRect(), 2000);
            return;
        }

        QString path = QFileDialog::getSaveFileName(
            this,
            "Save Evolution History",
            "",
            "Evolution History log (*.txt *.csv);;All Files (*.*)"
        );
        if (path.isEmpty()) return;

        bool ok = m_evolutionHistory.saveToFile(path.toStdString());
        if (ok)
            QToolTip::showText(mapToGlobal(rect().center()), "Evolution history saved!",nullptr, QRect(), 2000);
        else
            QToolTip::showText(mapToGlobal(rect().center()), "Save failed!",nullptr, QRect(), 2000);

    });

    m_ga.setStatsCallback([this](int gen, double avg) {
        statsPanel->updateAverageFitness(gen, avg);
    });
}

void MainWindow::initializeGeneticAlgorithmFirstRun(){
    if (m_strategiesInitialized)
        return;
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
    m_strategiesInitialized = true; // prevent creating the strategies again
    // m_ga.reset();
}

void MainWindow::runOneGeneration()
{
    if (m_currentGeneration == 0) {
        initializeGeneticAlgorithmFirstRun();
        m_ga.reset();
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

    // Update Stats panel
    controlPanel->setCurrentGeneration(m_currentGeneration);
    controlPanel->setBestFitness(m_ga.getBestFitness());
    controlPanel->setAverageFitness(m_ga.getAverageFitness());

}

void MainWindow::resetEvolution()
{
    // STOP ANY RUNNING AUTO-EVOLUTION FIRST
    if (m_isRunning) {
        m_runTimer->stop();
        m_isRunning = false;
        qDebug() << "Auto-run stopped during reset";
    }

    // Clear old callback first (prevents dangling pointer)
    m_ga.setStatsCallback(nullptr);

    // RESET EVOLUTION STATE
    m_currentGeneration = 0;
    m_evolutionHistory.clear(); 
    m_strategiesInitialized = false;
    
    // Clear graph/history
    statsPanel->clearGraph();

    // 4. RESET THE GENETIC ALGORITHM
    // Re-bind callback safely
    m_ga.setStatsCallback([this](int gen, double avg) {
        statsPanel->updateAverageFitness(gen, avg);
    });

    qDebug() << "Evolution FULLY reset to Generation 0";
}