#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include "WatchCanvas.h"
#include "ComponentInspector.h"
#include "StatsPanel.h"
#include "../fileio/EvolutionHistory.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private:
    // QWidget* createControlPanel(); WHERE IS THIS USED?
    StatsPanel *statsPanel;
    WatchGA::FileIO::EvolutionHistory m_evolutionHistory;
    int m_currentGeneration;
};

#endif // MAINWINDOW_H