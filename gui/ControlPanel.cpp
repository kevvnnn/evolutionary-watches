#include "ControlPanel.h"
#include "ui_ControlPanel.h"
#include <QDebug>

namespace WatchGA {
namespace GUI {

ControlPanel::ControlPanel(QWidget* parent) : 
    QWidget(parent),
    ui(new Ui::ControlPanel)
{
    ui->setupUi(this);
    
    // Connect signals from UI elements to our private slots
    connect(ui->populationSpin, &QSpinBox::valueChanged, this, &ControlPanel::OnPopulationSizeChanged);
    connect(ui->mutationSpin, &QDoubleSpinBox::valueChanged, this, &ControlPanel::OnMutationRateChanged);
    connect(ui->crossoverSpin, &QDoubleSpinBox::valueChanged, this, &ControlPanel::OnCrossoverRateChanged);
    connect(ui->elitismSpin, &QSpinBox::valueChanged, this, &ControlPanel::OnElitismCountChanged);
    connect(ui->maxComponentsSpin, &QSpinBox::valueChanged, this, &ControlPanel::OnMaxComponentsChanged);
    
    connect(ui->selectionCombo, &QComboBox::currentIndexChanged, this, &ControlPanel::OnSelectionStrategyChanged);
    connect(ui->crossoverCombo, &QComboBox::currentIndexChanged, this, &ControlPanel::OnCrossoverStrategyChanged);
    connect(ui->mutationCombo, &QComboBox::currentIndexChanged, this, &ControlPanel::OnMutationStrategyChanged);
    
    // Connect control buttons
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
void ControlPanel::OnPopulationSizeChanged(int value)
{
    qDebug() << "Population size changed to:" << value;
    emit PopulationSizeChanged(static_cast<unsigned int>(value));
}

void ControlPanel::OnMutationRateChanged(double value)
{
    qDebug() << "Mutation rate changed to:" << value;
    emit MutationRateChanged(value);
}

void ControlPanel::OnCrossoverRateChanged(double value)
{
    qDebug() << "Crossover rate changed to:" << value;
    emit CrossoverRateChanged(value);
}

void ControlPanel::OnElitismCountChanged(int value)
{
    qDebug() << "Elitism count changed to:" << value;
    emit ElitismCountChanged(static_cast<unsigned int>(value));
}

void ControlPanel::OnMaxComponentsChanged(int value)
{
    qDebug() << "Max components changed to:" << value;
    emit MaxComponentsChanged(static_cast<unsigned int>(value));
}

void ControlPanel::OnSelectionStrategyChanged(int index)
{
    QString strategy = ui->selectionCombo->itemText(index);
    qDebug() << "Selection strategy changed to:" << strategy;
    emit SelectionStrategyChanged(strategy);
}

void ControlPanel::OnCrossoverStrategyChanged(int index)
{
    QString strategy = ui->crossoverCombo->itemText(index);
    qDebug() << "Crossover strategy changed to:" << strategy;
    emit CrossoverStrategyChanged(strategy);
}

void ControlPanel::OnMutationStrategyChanged(int index)
{
    QString strategy = ui->mutationCombo->itemText(index);
    qDebug() << "Mutation strategy changed to:" << strategy;
    emit MutationStrategyChanged(strategy);
}

// Getters implementation
unsigned int ControlPanel::GetPopulationSize() const
{
    return static_cast<unsigned int>(ui->populationSpin->value());
}

double ControlPanel::GetMutationRate() const
{
    return ui->mutationSpin->value();
}

double ControlPanel::GetCrossoverRate() const
{
    return ui->crossoverSpin->value();
}

unsigned int ControlPanel::GetElitismCount() const
{
    return static_cast<unsigned int>(ui->elitismSpin->value());
}

unsigned int ControlPanel::GetMaxComponents() const
{
    return static_cast<unsigned int>(ui->maxComponentsSpin->value());
}

QString ControlPanel::GetSelectionStrategy() const
{
    return ui->selectionCombo->currentText();
}

QString ControlPanel::GetCrossoverStrategy() const
{
    return ui->crossoverCombo->currentText();
}

QString ControlPanel::GetMutationStrategy() const
{
    return ui->mutationCombo->currentText();
}

// Setters implementation
void ControlPanel::SetPopulationSize(unsigned int size)
{
    ui->populationSpin->setValue(static_cast<int>(size));
}

void ControlPanel::SetMutationRate(double rate)
{
    ui->mutationSpin->setValue(rate);
}

void ControlPanel::SetCrossoverRate(double rate)
{
    ui->crossoverSpin->setValue(rate);
}

void ControlPanel::SetElitismCount(unsigned int count)
{
    ui->elitismSpin->setValue(static_cast<int>(count));
}

void ControlPanel::SetMaxComponents(unsigned int max)
{
    ui->maxComponentsSpin->setValue(static_cast<int>(max));
}

void ControlPanel::SetSelectionStrategy(const QString& strategyName)
{
    int index = ui->selectionCombo->findText(strategyName);
    if (index != -1) {
        ui->selectionCombo->setCurrentIndex(index);
    }
}

void ControlPanel::SetCrossoverStrategy(const QString& strategyName)
{
    int index = ui->crossoverCombo->findText(strategyName);
    if (index != -1) {
        ui->crossoverCombo->setCurrentIndex(index);
    }
}

void ControlPanel::SetMutationStrategy(const QString& strategyName)
{
    int index = ui->mutationCombo->findText(strategyName);
    if (index != -1) {
        ui->mutationCombo->setCurrentIndex(index);
    }
}

} // namespace GUI
} // namespace WatchGA