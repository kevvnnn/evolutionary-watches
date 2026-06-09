#ifndef COMPONENTINSPECTOR_H
#define COMPONENTINSPECTOR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "../genome/WatchComponent.h"

/**
 * @class ComponentInspector
 * @brief A Qt UI widget that displays real-time properties of a selected WatchComponent
 *
 * This widget acts as a property inspector panel. It shows key attributes
 * of a watch component (name, weight, friction, efficiency) using QLabels.
 * It connects to slots to update display when a component is selected 
 */
class ComponentInspector : public QWidget {
    Q_OBJECT
public:
    explicit ComponentInspector(QWidget* parent = nullptr);

public slots:
    /**
     * @brief Displays detailed data from a given WatchComponent in the UI
     * @param comp Pointer to the watch component to inspect
     */
    void showComponentData(WatchGA::Genome::WatchComponent* comp);
    void clearInspector();

private:
    QVBoxLayout* m_layout;
    QLabel* m_lblName;
    QLabel* m_lblWeight;
    QLabel* m_lblFriction;
    QLabel* m_lblEfficiency;
};

#endif // COMPONENTINSPECTOR_H