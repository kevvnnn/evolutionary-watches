#ifndef COMPONENTINSPECTOR_H
#define COMPONENTINSPECTOR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "../genome/WatchComponent.h"

/**
 * @brief UI panel that displays properties of a selected watch component
 *
 * Shows real-time info like name, weight, friction, and efficiency.
 * Used in the GUI to inspect individual watch parts when clicked.
 */
class ComponentInspector : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief Constructor - Sets up the inspector panel UI
     * @param parent Parent widget for Qt hierarchy
     */
    explicit ComponentInspector(QWidget* parent = nullptr);

public slots:
    /**
     * @brief Displays all properties from a given WatchComponent
     * @param comp The component to inspect and display
     */
    void showComponentData(WatchGA::Genome::WatchComponent* comp);

    /**
     * @brief Resets all labels to default "N/A" state
     */
    void clearInspector();

private:
    QVBoxLayout* m_layout;
    QLabel* m_lblName;
    QLabel* m_lblWeight;
    QLabel* m_lblFriction;
    QLabel* m_lblEfficiency;
};

#endif // COMPONENTINSPECTOR_H