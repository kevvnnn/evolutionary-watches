#include "MainWindow.h"

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setupUI();
    setWindowTitle("WatchGA GUI");
    resize(1000, 700);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* layout = new QVBoxLayout(central);

    QLabel* label = new QLabel("WatchGA GUI Running", central);
    label->setAlignment(Qt::AlignCenter);

    layout->addWidget(label);
}