#include "WatchCanvas.h"
#include <QPainter>
#include <cmath>
#include <QLineF>
#include <QToolTip>  

WatchCanvas::WatchCanvas(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(600, 600);
    setMouseTracking(true); // Critical for hover
    setStyleSheet("background-color: #121212;");

    // FIXED positions (NO random placement)
    m_components.emplace_back(new WatchComponent("Center Bearing", "Bearing", 30, QPoint(300, 300), 0.0));
    m_components.emplace_back(new WatchComponent("Mainspring", "Spring", 40, QPoint(220, 380), 0.0));
    m_components.emplace_back(new WatchComponent("Third Gear", "Gear", 25, QPoint(260, 240), 0.3));
    m_components.emplace_back(new WatchComponent("Second Gear", "Gear", 30, QPoint(380, 360), 0.6));
    m_components.emplace_back(new WatchComponent("Balance Wheel", "Wheel", 45, QPoint(360, 220), 0.0));
}

void WatchCanvas::paintEvent(QPaintEvent *event)
{
    
    Q_UNUSED(event);
    QPainter p(this);
    p.fillRect(rect(),QColor(20,20,20));
    p.setRenderHint(QPainter::Antialiasing);

    // Watch face circle
    p.setPen(QPen(Qt::white, 2));
    p.drawEllipse(50, 50, 500, 500);

    // Hour markers
    for (int i = 0; i < 12; ++i) {
        double ang = i * M_PI/6 - M_PI/2;
        int x1 = 300 + 230 * cos(ang);
        int y1 = 300 + 230 * sin(ang);
        int x2 = 300 + 250 * cos(ang);
        int y2 = 300 + 250 * sin(ang);
        p.drawLine(x1, y1, x2, y2);
    }

    // watch hands
    p.setPen(QPen(Qt::white, 3));
    p.drawLine(300, 300, 300, 200);
    p.setPen(QPen(Qt::red, 2));
    p.drawLine(300, 300, 400, 300);

    // Draw components
    for (auto* c : m_components) {
        QColor color;
        if (c == m_hovered)
            color = QColor(255, 200, 0); // Highlight on hover
        else if (c->getType() == "Gear")
            color = QColor(100, 149, 237);
        else if (c->getType() == "Spring")
            color = QColor(255, 200, 0);
        else if (c->getType() == "Wheel")
            color = QColor(220, 60, 60);
        else
            color = QColor(160, 160, 160);

        p.setBrush(color);
        p.setPen(Qt::black);
        p.drawEllipse(c->getPosition(), c->getSize(), c->getSize());

        p.setPen(Qt::white);
        p.drawText(c->getPosition().x() - c->getSize(),
                   c->getPosition().y() - c->getSize() - 6,
                   c->getName());
    }
}

void WatchCanvas::mouseMoveEvent(QMouseEvent *event) //hover code/tooltip
{
    WatchComponent* newHover = getComponentAt(event->pos());

    if (newHover != m_hovered) {
        m_hovered = newHover;

        if (m_hovered) {
            QString tooltipText = QString(
                "Name: %1\n"
                "Type: %2\n"
                "Size: %3\n"
                "Gear Ratio: %4"
            ).arg(m_hovered->getName())
             .arg(m_hovered->getType())
             .arg(m_hovered->getSize())
             .arg(m_hovered->getGearRatio(), 0, 'f', 2);

            QToolTip::showText(event->globalPosition().toPoint(), tooltipText);

            emit componentHovered(m_hovered);
        } else {
            QToolTip::hideText();
            emit componentLeft();
        }
        update();
    }
}

WatchComponent* WatchCanvas::getComponentAt(const QPoint& pos)
{
    for (auto* comp : m_components) {
        if (QLineF(pos, comp->getPosition()).length() <= comp->getSize()) {
            return comp;
        }
    }
    return nullptr;
}