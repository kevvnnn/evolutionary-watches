#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "WatchCanvas.h"
#include "StatsPanel.h"
#include "ControlPanel.h"
#include "../fileio/EvolutionHistory.h"
#include "../algorithm/GeneticAlgorithm.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private:
 // UI
    WatchCanvas* watchCanvas;
    StatsPanel* statsPanel;

    // Logic
    WatchGA::FileIO::EvolutionHistory m_evolutionHistory;
    WatchGA::Algorithm::GeneticAlgorithm m_ga;
    int m_currentGeneration;
    
    // Holds the loaded watch in memory
    std::unique_ptr<WatchGA::Genome::Watch> m_loadedWatch;
    
    void initializeGeneticAlgorithmFirstRun();
    void runOneGeneration();
    void resetEvolution();

    bool m_strategiesInitialized = false; // Prevents crashing

    // Status
    bool m_isRunning = false;
    QTimer* m_runTimer;

    WatchGA::GUI::ControlPanel* controlPanel;
};

#endif // MAINWINDOW_H