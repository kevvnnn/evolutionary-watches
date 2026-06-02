#include "StatsPanel.h"
#include <QIcon>

StatsPanel::StatsPanel(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Statistics");
    setFixedSize(400, 300);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    // Main layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // Header with title and close button
    headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(10);

    titleLabel = new QLabel("Statistics Panel", this);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    headerLayout->addWidget(titleLabel);

    headerLayout->addStretch();

    // closeButton = new QPushButton("X", this);
    // closeButton->setFixedSize(30, 30);
    // closeButton->setStyleSheet("background-color: #363636; border: 2px solid #292929; border-radius: 3px;");
    // headerLayout->addWidget(closeButton);

    mainLayout->addLayout(headerLayout);

    // Placeholder content
    placeholderLabel = new QLabel("Statistics graph will be displayed here.\n\nThis is a placeholder for the actual statistics visualization.", this);
    placeholderLabel->setAlignment(Qt::AlignCenter);
    placeholderLabel->setStyleSheet("font-size: 14px; color: #666;");
    mainLayout->addWidget(placeholderLabel, 1);

    // Connect signals
    // connect(closeButton, &QPushButton::clicked, this, &StatsPanel::handleClose);
}

StatsPanel::~StatsPanel()
{
}

// void StatsPanel::handleClose()
// {
//     close();
// }