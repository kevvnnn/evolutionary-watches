#ifndef COMPONENTINSPECTOR_H
#define COMPONENTINSPECTOR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "../genome/WatchComponent.h"

class ComponentInspector : public QWidget {
    Q_OBJECT
public:
    explicit ComponentInspector(QWidget* parent = nullptr);

public slots:
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