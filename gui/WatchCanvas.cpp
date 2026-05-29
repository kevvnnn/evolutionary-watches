#include "WatchCanvas.h"

#include <QPainter>

WatchCanvas::WatchCanvas(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(400, 300);
}

void WatchCanvas::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.fillRect(rect(), QColor(30, 30, 30));
    
    painter.setPen(Qt::white);
    painter.drawText(rect(), Qt::AlignCenter, "Watch Canvas");
}