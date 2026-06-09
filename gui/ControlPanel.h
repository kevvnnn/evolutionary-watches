#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include "../fileio/ConfigManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ControlPanel; }
QT_END_NAMESPACE

namespace WatchGA {
namespace GUI {

/**
 * @brief Global configuration shared between GUI and algorithm
 */
extern WatchGA::FileIO::ConfigManager s_config;

/**
 * @brief Main control panel for genetic algorithm parameters
 *
 * Provides UI controls for population size, mutation/crossover rates,
 * elitism, and strategy selection. Emits signals for Run/Pause/Step/Reset.
 * Also displays real-time evolution statistics.
 */
class ControlPanel : public QWidget {
    Q_OBJECT

private:
    Ui::ControlPanel* ui; // Qt Designer UI object

private slots:
    /// Called when population size changes
    void onPopulationSizeChanged(int value);
    /// Called when mutation rate changes
    void onMutationRateChanged(double value);
    /// Called when crossover rate changes
    void onCrossoverRateChanged(double value);
    /// Called when elitism count changes
    void onElitismCountChanged(int value);
    /// Called when selection strategy changes
    void onSelectionStrategyChanged(int index);
    /// Called when crossover strategy changes
    void onCrossoverStrategyChanged(int index);
    /// Called when mutation strategy changes
    void onMutationStrategyChanged(int index);

signals:
    /// Emitted when population size is modified
    void populationSizeChanged(unsigned int size);
    /// Emitted when mutation rate is modified
    void mutationRateChanged(double rate);
    /// Emitted when crossover rate is modified
    void crossoverRateChanged(double rate);
    /// Emitted when elitism count is modified
    void elitismCountChanged(unsigned int count);
    /// Emitted when selection strategy changes
    void selectionStrategyChanged(const QString& strategyName);
    /// Emitted when crossover strategy changes
    void crossoverStrategyChanged(const QString& strategyName);
    /// Emitted when mutation strategy changes
    void mutationStrategyChanged(const QString& strategyName);

    /// Emitted when Run button clicked
    void runClicked();
    /// Emitted when Pause button clicked
    void pauseClicked();
    /// Emitted when Step button clicked
    void stepClicked();
    /// Emitted when Reset button clicked
    void resetClicked();

public:
    /**
     * @brief Constructor - initializes UI and default settings
     * @param parent Parent widget
     */
    explicit ControlPanel(QWidget* parent = nullptr);
    ~ControlPanel() override;

    // Getters

    /// Get current population size
    unsigned int getPopulationSize() const;
    /// Get current mutation rate
    double getMutationRate() const;
    /// Get current crossover rate
    double getCrossoverRate() const;
    /// Get current elitism count
    unsigned int getElitismCount() const;

    /// Get selected selection strategy
    QString getSelectionStrategy() const;
    /// Get selected crossover strategy
    QString getCrossoverStrategy() const;
    /// Get selected mutation strategy
    QString getMutationStrategy() const;

    // Setters

    /// Set population size in UI
    void setPopulationSize(unsigned int size);
    /// Set mutation rate in UI
    void setMutationRate(double rate);
    /// Set crossover rate in UI
    void setCrossoverRate(double rate);
    /// Set elitism count in UI
    void setElitismCount(unsigned int count);

    /// Set selection strategy
    void setSelectionStrategy(const QString& strategyName);
    /// Set crossover strategy
    void setCrossoverStrategy(const QString& strategyName);
    /// Set mutation strategy
    void setMutationStrategy(const QString& strategyName);

    // ------------------------------
    // Statistics Display
    // ------------------------------

    /// Update displayed generation number
    void setCurrentGeneration(int gen);
    /// Update best fitness display
    void setBestFitness(double val);
    /// Update average fitness display
    void setAverageFitness(double val);

    // ------------------------------
    // UI Control
    // ------------------------------

    /**
     * @brief Enable/disable parameter controls during run
     * @param control True = enabled, False = disabled
     */
    void controlParameter(bool control);
};

} // namespace GUI
} // namespace WatchGA

#endif // CONTROLPANEL_H