#include "MainWindow.h"
#include "WatchCanvas.h"
// #include "ComponentInspector.h"
#include "ControlPanel.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("WatchGA");
    setFixedSize(1000, 700);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // LEFT: Watch Canvas 
    WatchCanvas* canvas = new WatchCanvas;
    mainLayout->addWidget(canvas);

    // RIGHT: ControlPanel + Component Inspector
    QVBoxLayout* rightLayout = new QVBoxLayout;

    WatchGA::GUI::ControlPanel* controlPanel = new WatchGA::GUI::ControlPanel;
    rightLayout->addWidget(controlPanel);

    // Component Inspector panel
    // ComponentInspector* inspector = new ComponentInspector;
    // rightLayout->addWidget(inspector);

    rightLayout->addStretch();

    mainLayout->addLayout(rightLayout);

    // Connect hover signals 
    // connect(canvas, &WatchCanvas::componentHovered, &ComponentInspector::showComponentData);
    // connect(canvas, &WatchCanvas::componentLeft, &ComponentInspector::clearInspector);

    //most of the comments here are for the data bars on the right side of the window
}