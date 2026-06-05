#include "MainWindow.h"
#include "WatchCanvas.h"
#include "ControlPanel.h"
#include "StatsPanel.h"
#include "GeneticAlgorithm.h"  // Added
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <functional>           // Added



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("WatchGA - Component Inspector");
    setFixedSize(1366, 960); // Match your window size

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(30); // More space between left and right

    // --------------------------
    // ✅ LEFT: Centered Watch Canvas (final version)
    // --------------------------
    QWidget* leftContainer = new QWidget;
    QHBoxLayout* leftLayout = new QHBoxLayout(leftContainer);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setAlignment(Qt::AlignCenter);
    leftLayout->addStretch();

    WatchCanvas* canvas = new WatchCanvas;
    canvas->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    leftLayout->addWidget(canvas);

    leftLayout->addStretch();
    mainLayout->addWidget(leftContainer);

    // --------------------------
    // RIGHT: Control Panel + StatsPanel (clean layout)
    // --------------------------
    QWidget* rightContainer = new QWidget;
    QVBoxLayout* rightLayout = new QVBoxLayout(rightContainer);
    rightLayout->setSpacing(15);
    rightLayout->setContentsMargins(0, 0, 0, 0); 

    WatchGA::GUI::ControlPanel* controlPanel = new WatchGA::GUI::ControlPanel;
    rightLayout->addWidget(controlPanel);

    statsPanel = new StatsPanel(rightContainer);
    statsPanel->setFixedSize(450, 260);
    rightLayout->addWidget(statsPanel);

    // ---------------------------------------------------------------------
    // ✅ CALLBACK CODE INSERTED HERE
    // ---------------------------------------------------------------------
    WatchGA::Algorithm::GeneticAlgorithm ga;
    ga.setStatsCallback([this](int generation, double avgFitness) {
        statsPanel->updateAverageFitness(generation, avgFitness);
    });
    // ---------------------------------------------------------------------

    rightLayout->addStretch();
    mainLayout->addWidget(rightContainer); 
} 