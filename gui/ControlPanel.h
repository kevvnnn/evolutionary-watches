#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class ControlPanel; }
QT_END_NAMESPACE

namespace WatchGA {
namespace GUI {

/**
 * @brief The control panel for simulation parameters
 * 
 * Allows the user to set:
 * - Population size
 * - Mutation rate
 * - Crossover rate
 * - Elitism count
 * - Maximum components per watch
 * - Which algorithms to use (selection, crossover, mutation)
 * 
 * OWNER: Person D
 */
class ControlPanel : public QWidget {
    Q_OBJECT

private:
    Ui::ControlPanel* ui; // Generated UI from Qt Designer

private slots:
    // These slots are called when the user changes a value
    void onPopulationSizeChanged(int value);
    void onMutationRateChanged(double value);
    void onCrossoverRateChanged(double value);
    void onElitismCountChanged(int value);
    // void onMaxComponentsChanged(int value);
    void onSelectionStrategyChanged(int index);
    void onCrossoverStrategyChanged(int index);
    void onMutationStrategyChanged(int index);

signals:
    // These signals are emitted when the user changes a value
    // The MainWindow connects to these signals to update the genetic algorithm
    void populationSizeChanged(unsigned int size);
    void mutationRateChanged(double rate);
    void crossoverRateChanged(double rate);
    void elitismCountChanged(unsigned int count);
    // void maxComponentsChanged(unsigned int max);
    void selectionStrategyChanged(const QString& strategyName);
    void crossoverStrategyChanged(const QString& strategyName);
    void mutationStrategyChanged(const QString& strategyName);
    void runClicked();
    void pauseClicked();
    void stepClicked();
    

public:
    explicit ControlPanel(QWidget* parent = nullptr);
    ~ControlPanel() override;

    // Getters for current values
    unsigned int getPopulationSize() const;
    double getMutationRate() const;
    double getCrossoverRate() const;
    unsigned int getElitismCount() const;
    // unsigned int getMaxComponents() const;
    QString getSelectionStrategy() const;
    QString getCrossoverStrategy() const;
    QString getMutationStrategy() const;

    // Setters
    void setPopulationSize(unsigned int size);
    void setMutationRate(double rate);
    void setCrossoverRate(double rate);
    void setElitismCount(unsigned int count);
    // void setMaxComponents(unsigned int max);
    void setSelectionStrategy(const QString& strategyName);
    void setCrossoverStrategy(const QString& strategyName);
    void setMutationStrategy(const QString& strategyName);
};

} // namespace GUI
} // namespace WatchGA

#endif // CONTROLPANEL_H