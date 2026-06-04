#include "MainWindow.h"
#include "WatchCanvas.h"
#include "ControlPanel.h"
#include "StatsPanel.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("WatchGA - Component Inspector");
    setFixedSize(1000, 700);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // LEFT: Watch Canvas (original look preserved)
    WatchCanvas* canvas = new WatchCanvas;
    mainLayout->addWidget(canvas);

    // RIGHT: Control Panel + Component Inspector
    QVBoxLayout* rightLayout = new QVBoxLayout;
    WatchGA::GUI::ControlPanel* controlPanel = new WatchGA::GUI::ControlPanel;
    rightLayout->addWidget(controlPanel);

    statsPanel = new StatsPanel(this);
    statsPanel->setWindowFlags(Qt::Widget);  // Embed in main window
    rightLayout->addWidget(statsPanel); 




    // rightLayout->addStretch();

    mainLayout->addLayout(rightLayout);


}