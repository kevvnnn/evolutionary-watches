#include "MainWindow.h"

#include <QWidget>
#include <QHBoxLayout>

#include "ControlPanel.h"
#include "WatchCanvas.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setupUI();
    setWindowTitle("WatchGA");
    setFixedSize(1200, 700); // Set Size here: vertical,horizontal 
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout* layout = new QHBoxLayout(central);

    WatchCanvas* canvas = new WatchCanvas(central);
    WatchGA::GUI::ControlPanel* controls = new WatchGA::GUI::ControlPanel(central);

    controls->setFixedWidth(250);

    layout->addWidget(canvas, 1);
    layout->addWidget(controls, 0);
}