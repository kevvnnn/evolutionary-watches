#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "WatchCanvas.h"
#include "StatsPanel.h"
#include "ControlPanel.h"
#include "../fileio/EvolutionHistory.h"
#include "../algorithm/GeneticAlgorithm.h"

/**
 * @brief Main application window
 *
 * Contains the watch canvas, control panel, and stats graph.
 * Manages the genetic algorithm, evolution flow, and save/load features.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    /**
     * @brief Constructor - initializes UI, timer, and layout
     * @param parent Parent widget
     */
    MainWindow(QWidget* parent = nullptr);

private:
    // UI Components
    WatchCanvas* watchCanvas;
    StatsPanel* statsPanel;
    WatchGA::GUI::ControlPanel* controlPanel;

    // Core Logic
    WatchGA::FileIO::EvolutionHistory m_evolutionHistory;
    WatchGA::Algorithm::GeneticAlgorithm m_ga;
    int m_currentGeneration;

    // Loaded watch storage
    std::unique_ptr<WatchGA::Genome::Watch> m_loadedWatch;

    // Initialization & Evolution
    void initializeGeneticAlgorithmFirstRun();
    void runOneGeneration();
    void resetEvolution();

    bool m_strategiesInitialized; // To prevent strategies from being initialized twice, causing crashes
    bool m_isRunning;
    QTimer* m_runTimer;
};

#endif // MAINWINDOW_H