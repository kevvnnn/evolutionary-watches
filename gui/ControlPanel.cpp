#include "ControlPanel.h"
#include "ui_ControlPanel.h"
#include "../fileio/ConfigManager.h"
#include <QDebug>

namespace WatchGA {
namespace GUI {

// Global config shared across the application
WatchGA::FileIO::ConfigManager s_config;

// Constructor: Sets up UI, default config, and button connections
ControlPanel::ControlPanel(QWidget* parent) : 
    QWidget(parent),
    ui(new Ui::ControlPanel)
{
    ui->setupUi(this);

    // Set allowed mutation strategies
    ui->mutationCombo->clear();
    ui->mutationCombo->addItems({"Parameter", "Swap", "AddRemove"});

    // Initialize config file with default values
    s_config = FileIO::ConfigManager("config.txt");
    s_config.setInt("populationSize", 100);
    s_config.setDouble("mutationRate", 0.1);
    s_config.setDouble("crossoverRate", 0.8);
    s_config.setInt("elitismCount", 2);
    s_config.setString("selectionStrategy", "Tournament");
    s_config.setString("crossoverStrategy", "One Point");
    s_config.setString("mutationStrategy", "Parameter");
    s_config.saveConfig();

    // Initial button state: pause disabled
    ui->pauseBtn->setEnabled(false);

    // Connect UI input changes to internal slots
    connect(ui->populationSpin, &QSpinBox::valueChanged, this, &ControlPanel::onPopulationSizeChanged);
    connect(ui->mutationSpin, &QDoubleSpinBox::valueChanged, this, &ControlPanel::onMutationRateChanged);
    connect(ui->crossoverSpin, &QDoubleSpinBox::valueChanged, this, &ControlPanel::onCrossoverRateChanged);
    connect(ui->elitismSpin, &QSpinBox::valueChanged, this, &ControlPanel::onElitismCountChanged);
    
    connect(ui->selectionCombo, &QComboBox::currentIndexChanged, this, &ControlPanel::onSelectionStrategyChanged);
    connect(ui->crossoverCombo, &QComboBox::currentIndexChanged, this, &ControlPanel::onCrossoverStrategyChanged);
    connect(ui->mutationCombo, &QComboBox::currentIndexChanged, this, &ControlPanel::onMutationStrategyChanged);

    // Run Button Logic
    connect(ui->runBtn, &QPushButton::clicked, this, [this](){
        qDebug() << "Run button clicked";
        // Lock UI during auto-run
        ui->runBtn->setEnabled(false);
        ui->pauseBtn->setEnabled(true);
        ui->stepBtn->setEnabled(false);
        controlParameter(false);
        emit runClicked();
    });

    // Pause Button Logic
    connect(ui->pauseBtn, &QPushButton::clicked, this, [this](){
        qDebug() << "Pause button clicked";
        // Unlock UI
        ui->pauseBtn->setEnabled(false);
        ui->runBtn->setEnabled(true);
        ui->stepBtn->setEnabled(true);
        emit pauseClicked();
    });

    // Reset Button Logic
    connect(ui->resetBtn, &QPushButton::clicked, this, [this](){
        qDebug() << "Reset button clicked";
        // Restore default button state
        ui->runBtn->setEnabled(true);
        ui->pauseBtn->setEnabled(false);
        ui->stepBtn->setEnabled(true);
        // Clear stats display
        ui->generationValue->setText("0");
        ui->bestFitnessValue->setText("0.0");
        ui->avgFitnessValue->setText("0.0");
        controlParameter(true);
        emit resetClicked();
    });

    // Step Button Logic
    connect(ui->stepBtn, &QPushButton::clicked, this, [this](){
        qDebug() << "Step button clicked";
        controlParameter(false);
        emit stepClicked();
    });

    // Reset to Defaults
    connect(ui->resetToDefaults,&QPushButton::clicked, this, [this](){
        qDebug() << "Reset to Default clicked";
        // Restore UI defaults
        ui->populationSpin->setValue(100);
        ui->mutationSpin->setValue(0.1);
        ui->crossoverSpin->setValue(0.8);
        ui->elitismSpin->setValue(2);
        ui->selectionCombo->setCurrentText("Tournament");
        ui->crossoverCombo->setCurrentText("One Point");
        ui->mutationCombo->setCurrentText("Parameter");
        // Update config file
        s_config.setInt("populationSize", 100);
        s_config.setDouble("mutationRate", 0.1);
        s_config.setDouble("crossoverRate", 0.8);
        s_config.setInt("elitismCount", 2);
        s_config.setString("selectionStrategy", "Tournament");
        s_config.setString("crossoverStrategy", "One Point");
        s_config.setString("mutationStrategy", "Parameter");
        s_config.saveConfig();
    });
}

ControlPanel::~ControlPanel()
{
    delete ui;
}

// Parameter change handlers: save to config and emit signals

void ControlPanel::onPopulationSizeChanged(int value)
{
    qDebug() << "Population size changed to:" << value;
    s_config.setInt("populationSize", value);
    s_config.saveConfig();
    emit populationSizeChanged(static_cast<unsigned int>(value));
}

void ControlPanel::onMutationRateChanged(double value)
{
    qDebug() << "Mutation rate changed to:" << value;
    s_config.setDouble("mutationRate", value);
    s_config.saveConfig();
    emit mutationRateChanged(value);
}

void ControlPanel::onCrossoverRateChanged(double value)
{
    qDebug() << "Crossover rate changed to:" << value;
    s_config.setDouble("crossoverRate", value);
    s_config.saveConfig();
    emit crossoverRateChanged(value);
}

void ControlPanel::onElitismCountChanged(int value)
{
    qDebug() << "Elitism count changed to:" << value;
    s_config.setInt("elitismCount", value);
    s_config.saveConfig();
    emit elitismCountChanged(static_cast<unsigned int>(value));
}

void ControlPanel::onSelectionStrategyChanged(int index)
{
    QString strategy = ui->selectionCombo->itemText(index);
    qDebug() << "Selection strategy changed to:" << strategy;
    s_config.setString("selectionStrategy", strategy.toStdString());
    s_config.saveConfig();
    emit selectionStrategyChanged(strategy);
}

void ControlPanel::onCrossoverStrategyChanged(int index)
{
    QString strategy = ui->crossoverCombo->itemText(index);
    qDebug() << "Crossover strategy changed to:" << strategy;
    s_config.setString("crossoverStrategy", strategy.toStdString());
    s_config.saveConfig();
    emit crossoverStrategyChanged(strategy);
}

void ControlPanel::onMutationStrategyChanged(int index)
{
    QString strategy = ui->mutationCombo->itemText(index);
    qDebug() << "Mutation strategy changed to:" << strategy;
    s_config.setString("mutationStrategy", strategy.toStdString());
    s_config.saveConfig();
    emit mutationStrategyChanged(strategy);
}

// Getters

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

// Setters

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

// Enable/disable all parameter controls during evolution
void ControlPanel::controlParameter(bool control){
    ui->populationSpin->setEnabled(control);
    ui->mutationSpin->setEnabled(control);
    ui->crossoverSpin->setEnabled(control);
    ui->elitismSpin->setEnabled(control);
    ui->selectionCombo->setEnabled(control);
    ui->crossoverCombo->setEnabled(control);
    ui->mutationCombo->setEnabled(control);
    ui->resetToDefaults->setEnabled(control);
}

// Update statistics display

void ControlPanel::setCurrentGeneration(int gen)
{
    ui->generationValue->setText(QString::number(gen));
}

void ControlPanel::setBestFitness(double val)
{
    ui->bestFitnessValue->setText(QString::number(val, 'f', 2));
}

void ControlPanel::setAverageFitness(double val)
{
    ui->avgFitnessValue->setText(QString::number(val, 'f', 2));
}

} // namespace GUI
} // namespace WatchGA