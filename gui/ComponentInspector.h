#ifndef COMPONENTINSPECTOR_H
#define COMPONENTINSPECTOR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "WatchComponent.h"

class ComponentInspector : public QWidget {
    Q_OBJECT
public:
    explicit ComponentInspector(QWidget* parent = nullptr);

public slots:
    void showComponentData(WatchComponent* comp);
    void clearInspector();

private:
    QVBoxLayout* m_layout;
    QLabel* m_lblName;
    QLabel* m_lblType;
    QLabel* m_lblSize;
    QLabel* m_lblPos;
    QLabel* m_lblGearRatio;
};

#endif // COMPONENTINSPECTOR_H