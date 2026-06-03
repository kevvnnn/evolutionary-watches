#include "MainWindow.h"
#include "WatchCanvas.h"
#include "ComponentInspector.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QFormLayout>

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

    // LEFT: Watch Canvas (DARK MODE + HOVER TOOLTIPS + FIXED GEARS)
    WatchCanvas* canvas = new WatchCanvas;
    mainLayout->addWidget(canvas);

    // RIGHT: FULL CONTROL PANEL (YOUR CODE, 1:1) + COMPONENT INSPECTOR
    QVBoxLayout* rightLayout = new QVBoxLayout;

    // ==============================================
    // 👇 YOUR FULL CONTROL PANEL CODE EMBEDDED HERE
    // ==============================================
    {
        // Simulation Control Buttons
        QGroupBox* simGroup = new QGroupBox("Simulation Control");
        QHBoxLayout* simLayout = new QHBoxLayout(simGroup);
        QPushButton* runBtn = new QPushButton("Run");
        QPushButton* pauseBtn = new QPushButton("Pause");
        QPushButton* resetBtn = new QPushButton("Reset");
        QPushButton* stepBtn = new QPushButton("Step");
        pauseBtn->setEnabled(false);
        simLayout->addWidget(runBtn);
        simLayout->addWidget(pauseBtn);
        simLayout->addWidget(resetBtn);
        simLayout->addWidget(stepBtn);
        rightLayout->addWidget(simGroup);

        // Algorithm Parameters
        QGroupBox* paramGroup = new QGroupBox("Algorithm Parameters");
        QFormLayout* paramForm = new QFormLayout(paramGroup);
        QSpinBox* populationSpin = new QSpinBox;
        QDoubleSpinBox* mutationSpin = new QDoubleSpinBox;
        QDoubleSpinBox* crossoverSpin = new QDoubleSpinBox;
        QSpinBox* elitismSpin = new QSpinBox;
        QSpinBox* maxComponentsSpin = new QSpinBox;
        populationSpin->setRange(10, 1000); populationSpin->setValue(100);
        mutationSpin->setRange(0.0, 1.0); mutationSpin->setSingleStep(0.01); mutationSpin->setValue(0.1);
        crossoverSpin->setRange(0.0, 1.0); crossoverSpin->setSingleStep(0.01); crossoverSpin->setValue(0.8);
        elitismSpin->setRange(0, 10); elitismSpin->setValue(2);
        maxComponentsSpin->setRange(1, 20); maxComponentsSpin->setValue(6);
        paramForm->addRow("Population Size:", populationSpin);
        paramForm->addRow("Mutation Rate:", mutationSpin);
        paramForm->addRow("Crossover Rate:", crossoverSpin);
        paramForm->addRow("Elitism Count:", elitismSpin);
        paramForm->addRow("Max Components:", maxComponentsSpin);
        rightLayout->addWidget(paramGroup);

        // Algorithm Strategies
        QGroupBox* stratGroup = new QGroupBox("Algorithm Strategies");
        QFormLayout* stratForm = new QFormLayout(stratGroup);
        QComboBox* selectionCombo = new QComboBox;
        QComboBox* crossoverCombo = new QComboBox;
        QComboBox* mutationCombo = new QComboBox;
        selectionCombo->addItems({"Roulette", "Tournament", "Rank"});
        crossoverCombo->addItems({"SinglePoint", "TwoPoint", "Uniform"});
        mutationCombo->addItems({"Random", "Gaussian", "Uniform"});
        stratForm->addRow("Selection:", selectionCombo);
        stratForm->addRow("Crossover:", crossoverCombo);
        stratForm->addRow("Mutation:", mutationCombo);
        rightLayout->addWidget(stratGroup);

        // Statistics
        QGroupBox* statsGroup = new QGroupBox("Statistics");
        QFormLayout* statsForm = new QFormLayout(statsGroup);
        QLabel* genValue = new QLabel("0");
        QLabel* bestValue = new QLabel("0.0");
        QLabel* avgValue = new QLabel("0.0");
        statsForm->addRow("Generation:", genValue);
        statsForm->addRow("Best Fitness:", bestValue);
        statsForm->addRow("Avg Fitness:", avgValue);
        rightLayout->addWidget(statsGroup);

        // --------------------------
        // YOUR BUTTON & SIGNAL LOGIC (EXACTLY AS YOU WROTE IT)
        // --------------------------
        connect(runBtn, &QPushButton::clicked, this, [=]() {
            qDebug() << "Run button clicked";
            runBtn->setEnabled(false);
            pauseBtn->setEnabled(true);
        });
        connect(pauseBtn, &QPushButton::clicked, this, [=]() {
            qDebug() << "Pause button clicked";
            pauseBtn->setEnabled(false);
            runBtn->setEnabled(true);
        });
        connect(resetBtn, &QPushButton::clicked, this, [=]() {
            qDebug() << "Reset button clicked";
            runBtn->setEnabled(true);
            pauseBtn->setEnabled(false);
            genValue->setText("0");
            bestValue->setText("0.0");
            avgValue->setText("0.0");
        });
        connect(stepBtn, &QPushButton::clicked, this, [=]() {
            qDebug() << "Step button clicked";
        });

        // Parameter change debug logs (YOUR CODE)
        connect(populationSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int v) {
            qDebug() << "Population size changed to:" << v;
        });
        connect(mutationSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [=](double v) {
            qDebug() << "Mutation rate changed to:" << v;
        });
        connect(crossoverSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [=](double v) {
            qDebug() << "Crossover rate changed to:" << v;
        });
        connect(elitismSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int v) {
            qDebug() << "Elitism count changed to:" << v;
        });
        connect(maxComponentsSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int v) {
            qDebug() << "Max components changed to:" << v;
        });
        connect(selectionCombo, &QComboBox::currentIndexChanged, this, [=](int i) {
            qDebug() << "Selection strategy changed to:" << selectionCombo->itemText(i);
        });
        connect(crossoverCombo, &QComboBox::currentIndexChanged, this, [=](int i) {
            qDebug() << "Crossover strategy changed to:" << crossoverCombo->itemText(i);
        });
        connect(mutationCombo, &QComboBox::currentIndexChanged, this, [=](int i) {
            qDebug() << "Mutation strategy changed to:" << mutationCombo->itemText(i);
        });
    }

    // Component Inspector Panel (HOVER DATA DISPLAY)
    ComponentInspector* inspector = new ComponentInspector;
    rightLayout->addWidget(inspector);
    rightLayout->addStretch();

    mainLayout->addLayout(rightLayout);

    // Connect Hover Signals (NO CHANGES)
    connect(canvas, &WatchCanvas::componentHovered, inspector, &ComponentInspector::showComponentData);
    connect(canvas, &WatchCanvas::componentLeft, inspector, &ComponentInspector::clearInspector);
}