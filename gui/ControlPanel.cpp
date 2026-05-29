#include "ControlPanel.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

ControlPanel::ControlPanel(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
}

void ControlPanel::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* title = new QLabel("Controls", this);

    runBtn = new QPushButton("Run", this);
    resetBtn = new QPushButton("Reset", this);
    stepBtn = new QPushButton("Step", this);

    layout->addWidget(title);
    layout->addWidget(runBtn);
    layout->addWidget(resetBtn);
    layout->addWidget(stepBtn);
    layout->addStretch();

    connect(runBtn, &QPushButton::clicked, this, [](){
        qDebug() << "Run Button clicked";
    });
    connect(resetBtn, &QPushButton::clicked, this, [](){
        qDebug() << "Reset button clicked";
    });
    connect(stepBtn, &QPushButton::clicked, this, [](){
        qDebug() << "Step button clicked";
    });
}