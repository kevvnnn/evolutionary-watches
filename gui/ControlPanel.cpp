#include "ControlPanel.h"
#include "ui_ControlPanel.h"
#include "../fileio/ConfigManager.h"
#include <QDebug>

namespace WatchGA {
namespace GUI {

ControlPanel::ControlPanel(QWidget* parent) : 
    QWidget(parent),
    ui(new Ui::ControlPanel)
{
    ui->setupUi(this);
    
    // Connect signals from UI elements to our private slots
    connect(ui->populationSpin, &QSpinBox::valueChanged, this, &ControlPanel::onPopulationSizeChanged);
    connect(ui->mutationSpin, &QDoubleSpinBox::valueChanged, this, &ControlPanel::onMutationRateChanged);
    connect(ui->crossoverSpin, &QDoubleSpinBox::valueChanged, this, &ControlPanel::onCrossoverRateChanged);
    connect(ui->elitismSpin, &QSpinBox::valueChanged, this, &ControlPanel::onElitismCountChanged);
    connect(ui->maxComponentsSpin, &QSpinBox::valueChanged, this, &ControlPanel::onMaxComponentsChanged);
    
    connect(ui->selectionCombo, &QComboBox::currentIndexChanged, this, &ControlPanel::onSelectionStrategyChanged);
    connect(ui->crossoverCombo, &QComboBox::currentIndexChanged, this, &ControlPanel::onCrossoverStrategyChanged);
    connect(ui->mutationCombo, &QComboBox::currentIndexChanged, this, &ControlPanel::onMutationStrategyChanged);
    

    connect(ui->runBtn, &QPushButton::clicked, this, [this](){
        qDebug() << "Run button clicked";
        ui->runBtn->setEnabled(false);
        ui->pauseBtn->setEnabled(true);
    });
    
    connect(ui->pauseBtn, &QPushButton::clicked, this, [this](){
        qDebug() << "Pause button clicked";
        ui->pauseBtn->setEnabled(false);
        ui->runBtn->setEnabled(true);
    });
    
    connect(ui->resetBtn, &QPushButton::clicked, this, [this](){
        qDebug() << "Reset button clicked";
        ui->runBtn->setEnabled(true);
        ui->pauseBtn->setEnabled(false);
        ui->generationValue->setText("0");
        ui->bestFitnessValue->setText("0.0");
        ui->avgFitnessValue->setText("0.0");
    });
    
    connect(ui->stepBtn, &QPushButton::clicked, this, [this](){
        qDebug() << "Step button clicked";
    });
}

ControlPanel::~ControlPanel()
{
    delete ui;
}

// Private slots implementation
void ControlPanel::onPopulationSizeChanged(int value)
{
    qDebug() << "Population size changed to:" << value;
    emit populationSizeChanged(static_cast<unsigned int>(value));
}

void ControlPanel::onMutationRateChanged(double value)
{
    qDebug() << "Mutation rate changed to:" << value;
    emit mutationRateChanged(value);
}

void ControlPanel::onCrossoverRateChanged(double value)
{
    qDebug() << "Crossover rate changed to:" << value;
    emit crossoverRateChanged(value);
}

void ControlPanel::onElitismCountChanged(int value)
{
    qDebug() << "Elitism count changed to:" << value;
    emit elitismCountChanged(static_cast<unsigned int>(value));
}

void ControlPanel::onMaxComponentsChanged(int value)
{
    qDebug() << "Max components changed to:" << value;
    emit maxComponentsChanged(static_cast<unsigned int>(value));
}

void ControlPanel::onSelectionStrategyChanged(int index)
{
    QString strategy = ui->selectionCombo->itemText(index);
    qDebug() << "Selection strategy changed to:" << strategy;
    emit selectionStrategyChanged(strategy);
}

void ControlPanel::onCrossoverStrategyChanged(int index)
{
    QString strategy = ui->crossoverCombo->itemText(index);
    qDebug() << "Crossover strategy changed to:" << strategy;
    emit crossoverStrategyChanged(strategy);
}

void ControlPanel::onMutationStrategyChanged(int index)
{
    QString strategy = ui->mutationCombo->itemText(index);
    qDebug() << "Mutation strategy changed to:" << strategy;
    emit mutationStrategyChanged(strategy);
}

// Getters implementation
unsigned int ControlPanel::getPopulationSize() const
{
    return static_cast<unsigned int>(ui->populationSpin->value());
}

double ControlPanel::getMutationRate() const
{
    return ui->mutationSpin->value();
}

double ControlPanel::getCrossoverRate() const
{
    return ui->crossoverSpin->value();
}

unsigned int ControlPanel::getElitismCount() const
{
    return static_cast<unsigned int>(ui->elitismSpin->value());
}

unsigned int ControlPanel::getMaxComponents() const
{
    return static_cast<unsigned int>(ui->maxComponentsSpin->value());
}

QString ControlPanel::getSelectionStrategy() const
{
    return ui->selectionCombo->currentText();
}

QString ControlPanel::getCrossoverStrategy() const
{
    return ui->crossoverCombo->currentText();
}

QString ControlPanel::getMutationStrategy() const
{
    return ui->mutationCombo->currentText();
}

// Setters implementation
void ControlPanel::setPopulationSize(unsigned int size)
{
    ui->populationSpin->setValue(static_cast<int>(size));
}

void ControlPanel::setMutationRate(double rate)
{
    ui->mutationSpin->setValue(rate);
}

void ControlPanel::setCrossoverRate(double rate)
{
    ui->crossoverSpin->setValue(rate);
}

void ControlPanel::setElitismCount(unsigned int count)
{
    ui->elitismSpin->setValue(static_cast<int>(count));
}

void ControlPanel::setMaxComponents(unsigned int max)
{
    ui->maxComponentsSpin->setValue(static_cast<int>(max));
}

void ControlPanel::setSelectionStrategy(const QString& strategyName)
{
    int index = ui->selectionCombo->findText(strategyName);
    if (index != -1) {
        ui->selectionCombo->setCurrentIndex(index);
    }
}

void ControlPanel::setCrossoverStrategy(const QString& strategyName)
{
    int index = ui->crossoverCombo->findText(strategyName);
    if (index != -1) {
        ui->crossoverCombo->setCurrentIndex(index);
    }
}

void ControlPanel::setMutationStrategy(const QString& strategyName)
{
    int index = ui->mutationCombo->findText(strategyName);
    if (index != -1) {
        ui->mutationCombo->setCurrentIndex(index);
    }
}

} // namespace GUI
} // namespace WatchGA