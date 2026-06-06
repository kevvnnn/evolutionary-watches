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
     void setWatchCanvas(WatchCanvas* canvas) { watchCanvas = canvas; }

private:
    // QWidget* createControlPanel(); WHERE IS THIS USED?
    StatsPanel *statsPanel;
    WatchGA::FileIO::EvolutionHistory m_evolutionHistory;
    int m_currentGeneration;
     WatchCanvas* watchCanvas;
};

#endif // MAINWINDOW_H