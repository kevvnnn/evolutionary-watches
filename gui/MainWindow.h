#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "WatchCanvas.h"
#include "StatsPanel.h"
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
    void initializeGeneticAlgorithmFirstRun();
    void runOneGeneration();
    void resetEvolution();

    bool m_strategiesInitialized = false; // Prevents crashing

    // Status
    bool m_isRunning = false; // ADD THIS
    QTimer* m_runTimer;       // ADD THIS
};

#endif // MAINWINDOW_H