#include "ComponentInspector.h"
#include "../genome/WatchComponent.h"

ComponentInspector::ComponentInspector(QWidget* parent) : QWidget(parent)
{
    setFixedWidth(280);
    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(8);
    m_layout->setContentsMargins(10, 10, 10, 10);

    m_lblName = new QLabel("Component: None");
    m_lblWeight = new QLabel("Weight: N/A");
    m_lblFriction = new QLabel("Friction: N/A");
    m_lblPosition = new QLabel("Position: (N/A, N/A)");
    m_lblEfficiency = new QLabel("Efficiency: N/A");

    m_layout->addWidget(m_lblName);
    m_layout->addWidget(m_lblWeight);
    m_layout->addWidget(m_lblFriction);
    m_layout->addWidget(m_lblPosition);
    m_layout->addWidget(m_lblEfficiency);
    m_layout->addStretch();

    setStyleSheet("background-color: #2b2b2b; color: #a9b7c6;");
}

void ComponentInspector::showComponentData(WatchGA::Genome::WatchComponent* comp)
{
    if (!comp) {
        clearInspector();
        return;
    }

    m_lblName->setText(QString("Component: %1").arg(QString::fromStdString(comp->getName())));
    m_lblWeight->setText(QString("Weight: %1 g").arg(comp->getWeight(), 0, 'f', 2));
    m_lblFriction->setText(QString("Friction: %1").arg(comp->getFriction(), 0, 'f', 3));
    m_lblPosition->setText(QString("Position: (%1, %2)").arg(comp->getX()).arg(comp->getY()));
    m_lblEfficiency->setText(QString("Efficiency: %1%").arg(comp->calculateEfficiency() * 100, 0, 'f', 1));
}

void ComponentInspector::clearInspector()
{
    m_lblName->setText("Component: None");
    m_lblWeight->setText("Weight: N/A");
    m_lblFriction->setText("Friction: N/A");
    m_lblPosition->setText("Position: (N/A, N/A)");
    m_lblEfficiency->setText("Efficiency: N/A");
}