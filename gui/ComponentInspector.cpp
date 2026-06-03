#include "ComponentInspector.h"

ComponentInspector::ComponentInspector(QWidget* parent) : QWidget(parent) {
    setFixedWidth(280);
    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(8);
    m_layout->setContentsMargins(10, 10, 10, 10);

    m_lblName = new QLabel("Component: None");
    m_lblType = new QLabel("Type: None");
    m_lblSize = new QLabel("Size: N/A");
    m_lblPos = new QLabel("Position: (N/A, N/A)");
    m_lblGearRatio = new QLabel("Gear Ratio: N/A");

    m_layout->addWidget(m_lblName);
    m_layout->addWidget(m_lblType);
    m_layout->addWidget(m_lblSize);
    m_layout->addWidget(m_lblPos);
    m_layout->addWidget(m_lblGearRatio);
    m_layout->addStretch();

    // Dark theme to match your original UI
    setStyleSheet(R"(
        QWidget { background-color: #2b2b2b; color: #a9b7c6; }
        QLabel { font-size: 12px; color: #a9b7c6; }
    )");
}

void ComponentInspector::showComponentData(WatchComponent* comp) {
    if (!comp) { clearInspector(); return; }
    m_lblName->setText(QString("Component: %1").arg(comp->getName()));
    m_lblType->setText(QString("Type: %1").arg(comp->getType()));
    m_lblSize->setText(QString("Size: %1 px").arg(comp->getSize()));
    m_lblPos->setText(QString("Position: (%1, %2)").arg(comp->getPosition().x()).arg(comp->getPosition().y()));
    m_lblGearRatio->setText(comp->getType() == "Gear"
        ? QString("Gear Ratio: %1").arg(comp->getGearRatio(), 0, 'f', 2)
        : "Gear Ratio: N/A");
}

void ComponentInspector::clearInspector() {
    m_lblName->setText("Component: None");
    m_lblType->setText("Type: None");
    m_lblSize->setText("Size: N/A");
    m_lblPos->setText("Position: (N/A, N/A)");
    m_lblGearRatio->setText("Gear Ratio: N/A");
}