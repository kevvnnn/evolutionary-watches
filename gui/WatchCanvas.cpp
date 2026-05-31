#include "WatchCanvas.h"

#include <QPainter>

WatchCanvas::WatchCanvas(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(400, 300);

    // Create Stats Button 
    statsButton = new QPushButton("📊 Stats",this);
    statsButton->setStyleSheet("background-color: #4CAF50; color: white; border: none; border-radius: 4px; padding: 6px 12px; font-size: 12px;");
    statsButton->setFixedSize(80,30);
    statsButton->show();

    // Create the panel (hidden)
    statsPanel = new StatsPanel(this);
    // Connect Signal
    connect(statsButton,&QPushButton::clicked, this,&WatchCanvas::showStatsPanel);
    updateButtonPosition();

}

void WatchCanvas::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.fillRect(rect(), QColor(30, 30, 30));
    
    painter.setPen(Qt::white);
    painter.drawText(rect(), Qt::AlignCenter, "Watch Canvas");
}

void WatchCanvas::resizeEvent(QResizeEvent* event){
    QWidget::resizeEvent(event);
    updateButtonPosition();
}

void WatchCanvas::updateButtonPosition()
{
    // Position button in top-right corner with margin
    int margin = 10;
    statsButton->move(width() - statsButton->width() - margin, margin);
}
void WatchCanvas::showStatsPanel(){
    if (statsPanel){
        statsPanel->show();
    }
}