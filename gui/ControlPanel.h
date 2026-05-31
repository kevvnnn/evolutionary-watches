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
    void OnPopulationSizeChanged(int value);
    void OnMutationRateChanged(double value);
    void OnCrossoverRateChanged(double value);
    void OnElitismCountChanged(int value);
    void OnMaxComponentsChanged(int value);
    void OnSelectionStrategyChanged(int index);
    void OnCrossoverStrategyChanged(int index);
    void OnMutationStrategyChanged(int index);

signals:
    // These signals are emitted when the user changes a value
    // The MainWindow connects to these signals to update the genetic algorithm
    void PopulationSizeChanged(unsigned int size);
    void MutationRateChanged(double rate);
    void CrossoverRateChanged(double rate);
    void ElitismCountChanged(unsigned int count);
    void MaxComponentsChanged(unsigned int max);
    void SelectionStrategyChanged(const QString& strategyName);
    void CrossoverStrategyChanged(const QString& strategyName);
    void MutationStrategyChanged(const QString& strategyName);

public:
    explicit ControlPanel(QWidget* parent = nullptr);
    ~ControlPanel() override;

    // Getters for current values
    unsigned int GetPopulationSize() const;
    double GetMutationRate() const;
    double GetCrossoverRate() const;
    unsigned int GetElitismCount() const;
    unsigned int GetMaxComponents() const;
    QString GetSelectionStrategy() const;
    QString GetCrossoverStrategy() const;
    QString GetMutationStrategy() const;

    // Setters
    void SetPopulationSize(unsigned int size);
    void SetMutationRate(double rate);
    void SetCrossoverRate(double rate);
    void SetElitismCount(unsigned int count);
    void SetMaxComponents(unsigned int max);
    void SetSelectionStrategy(const QString& strategyName);
    void SetCrossoverStrategy(const QString& strategyName);
    void SetMutationStrategy(const QString& strategyName);
};

} // namespace GUI
} // namespace WatchGA

#endif // CONTROLPANEL_H