#include "WatchCanvas.h"
#include "../genome/Watch.h"
#include "../genome/WatchComponent.h"
#include "../genome/components/Hand.h"
#include "../genome/components/Gear.h"
#include "../genome/components/Spring.h"
#include "../genome/components/BalanceWheel.h"
#include "../genome/components/Jewel.h"

#include <QPainter>
#include <cmath>
#include <QToolTip>
#include <QLineF>
#include <QMouseEvent>
#include <QTimer>
#include <QPainterPath>

using namespace WatchGA::Genome;
using namespace WatchGA::Genome::Components;

// ------------------------------
// Fixed, non-overlapping positions
// ------------------------------
const QPointF GEAR_SLOTS[] = {
    { -120,    0 },
    {  -60,  -40 },
    {    0,  -20 },
    {   60,    0 },
    {  120,    0 },
    {  -60,   40 },
    {   60,   40 },
    { -120,  -60 },
};
const int NUM_GEAR_SLOTS = 8;

#define POS_SPRING     QPointF(-180, -20)
#define POS_BALANCE    QPointF( 150,   0)
#define POS_JEWEL      QPointF(   0,   0)

// ------------------------------
// ORIGINAL GEAR — NO CHANGES
// ------------------------------
static QPainterPath createGearPath(const Gear* gear, qreal cx, qreal cy)
{
    QPainterPath path;
    int teeth    = gear->getToothCount();
    double diameter = gear->getDiameter();
    double quality  = gear->getMeshingQuality();
    (void)quality;

    if (teeth < 6)  teeth = 6;
    if (teeth > 24) teeth = 24;

    const qreal scaleFactor = 40.0;
    const qreal minRadius = 6.0;
    qreal baseRadius = minRadius + (diameter * scaleFactor);
    qreal toothHeight = baseRadius * 0.22;
    qreal outerRadius = baseRadius + toothHeight;

    const qreal maxOuter = 32.0;
    if (outerRadius > maxOuter) {
        outerRadius = maxOuter;
        baseRadius = outerRadius / 1.22;
    }

    qreal step = (2 * M_PI) / teeth;
    qreal angle = -M_PI / 2;

    path.moveTo(cx + outerRadius * cos(angle), cy + outerRadius * sin(angle));
    for (int i = 0; i < teeth; ++i) {
        path.lineTo(cx + outerRadius * cos(angle), cy + outerRadius * sin(angle));
        angle += step * 0.4;
        path.lineTo(cx + baseRadius * cos(angle), cy + baseRadius * sin(angle));
        angle += step * 0.6;
    }
    path.closeSubpath();
    return path;
}

// ------------------------------
// MAIN RENDERING
// ------------------------------
WatchCanvas::WatchCanvas(QWidget *parent)
    : QWidget(parent), m_startTime(std::chrono::steady_clock::now())
{
    setFixedSize(600, 600);
    setMouseTracking(true);
    setStyleSheet("background-color: #121118;");

    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &WatchCanvas::updateAnimation);
    m_animationTimer->start(50);
}

void WatchCanvas::setWatch(Watch* watch) {
    m_watch = watch;
    update();
}

void WatchCanvas::updateAnimation() {
    update();
}

void WatchCanvas::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), QColor(20, 20, 20));

    const int cx = 300, cy = 300;
    const int dialR = 250;

    p.setPen(QPen(Qt::white, 2));
    p.drawEllipse(50, 50, 500, 500);

    for (int i = 0; i < 12; ++i) {
        double a = i * M_PI/6 - M_PI/2;
        p.drawLine(cx+230*cos(a), cy+230*sin(a), cx+250*cos(a), cy+250*sin(a));
    }

    if (!m_watch) return;

    int gearIndex = 0;
    for (const auto& comp : m_watch->getAllComponents()) {
        bool hover = comp.get() == m_hovered;
        QColor color;

        if (hover)          color = QColor(255, 210, 0);
        else if (dynamic_cast<const Gear*>(comp.get()))        color = QColor(90, 160, 240);
        else if (dynamic_cast<const Spring*>(comp.get()))      color = QColor(255, 180, 0);
        else if (dynamic_cast<const BalanceWheel*>(comp.get()))color = QColor(220, 50, 50);
        else if (dynamic_cast<const Jewel*>(comp.get()))       color = QColor(190, 190, 220);
        else if (dynamic_cast<const Hand*>(comp.get()))        color = QColor(240, 240, 240);
        else                                                  color = QColor(160, 160, 160);

        QPointF pos;
        if (dynamic_cast<const Spring*>(comp.get()))      pos = POS_SPRING;
        else if (dynamic_cast<const BalanceWheel*>(comp.get())) pos = POS_BALANCE;
        else if (dynamic_cast<const Jewel*>(comp.get()))        pos = POS_JEWEL;
        else if (dynamic_cast<const Gear*>(comp.get()))         { pos = GEAR_SLOTS[gearIndex % NUM_GEAR_SLOTS]; gearIndex++; }
        else pos = {0, 0};

        int x = static_cast<int>(pos.x() + cx);
        int y = static_cast<int>(pos.y() + cy);

        // Draw Hands
        if (const Hand* h = dynamic_cast<const Hand*>(comp.get())) {
            double angle = 3 * M_PI / 2;
            auto sec = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - m_startTime).count();

            qreal scale; QPen pen;
            if (h->getType() == Hand::HandType::HOUR) {
                angle += (sec % 43200) * (2 * M_PI / 43200.0);
                pen = QPen(color, 5); scale = 0.65;
            } else if (h->getType() == Hand::HandType::MINUTE) {
                angle += (sec % 3600) * (2 * M_PI / 3600.0);
                pen = QPen(color, 3); scale = 0.8;
            } else {
                angle += (sec % 60) * (2 * M_PI / 60.0);
                pen = QPen(Qt::red, 2); scale = 0.85;
            }

            p.setPen(pen);
            qreal len = dialR * scale;
            p.drawLine(cx, cy, cx + len*cos(angle), cy + len*sin(angle));
            continue;
        }

        p.setBrush(color);
        p.setPen(Qt::black);

        // Draw Gear
        const Gear* gear = dynamic_cast<const Gear*>(comp.get());
        if (gear) {
            p.drawPath(createGearPath(gear, x, y));

            // ==============================================
            // DRAW JEWEL IN THE CENTER OF THE GEAR
            // ==============================================
            if (gear->hasJewel()) {
                p.setBrush(QColor(255, 83, 255));
                p.drawEllipse(x, y, 7, 7);
            }
            continue;
        }

        // Draw other components
        if (dynamic_cast<const BalanceWheel*>(comp.get()))
            p.drawEllipse(x, y, 27, 27);
        else if (dynamic_cast<const Jewel*>(comp.get()))
            p.drawEllipse(x, y, 7, 7);
        else if (dynamic_cast<const Spring*>(comp.get()))
            p.drawEllipse(x, y, 25, 25);
        else
            p.drawEllipse(x, y, 20, 20);
    }
}

// ------------------------------
// HITBOX ONLY AT HAND TIPS
// ------------------------------
void WatchCanvas::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_watch) return;

    WatchComponent* found = nullptr;
    int gearIdx = 0;
    const int cx = 300, cy = 300;

    // Check hand TIPS
    for (const auto& comp : m_watch->getAllComponents()) {
        const Hand* hand = dynamic_cast<const Hand*>(comp.get());
        if (!hand) continue;

        double angle = 3 * M_PI / 2;
        auto sec = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - m_startTime).count();

        qreal scale;
        if (hand->getType() == Hand::HandType::HOUR) {
            angle += (sec % 43200) * (2 * M_PI / 43200.0);
            scale = 0.65;
        } else if (hand->getType() == Hand::HandType::MINUTE) {
            angle += (sec % 3600) * (2 * M_PI / 3600.0);
            scale = 0.8;
        } else {
            angle += (sec % 60) * (2 * M_PI / 60.0);
            scale = 0.85;
        }

        QPointF tip(cx + 250*scale*cos(angle), cy + 250*scale*sin(angle));
        if (QLineF(event->pos(), tip).length() < 14) {
            found = const_cast<Hand*>(hand);
            break;
        }
    }

    // Check gears + components
    if (!found) {
        for (const auto& comp : m_watch->getAllComponents()) {
            if (dynamic_cast<const Hand*>(comp.get())) continue;

            QPointF pos;
            if (dynamic_cast<const Spring*>(comp.get()))      pos = POS_SPRING;
            else if (dynamic_cast<const BalanceWheel*>(comp.get())) pos = POS_BALANCE;
            else if (dynamic_cast<const Jewel*>(comp.get()))        pos = POS_JEWEL;
            else if (dynamic_cast<const Gear*>(comp.get()))         { pos = GEAR_SLOTS[gearIdx % NUM_GEAR_SLOTS]; gearIdx++; }
            else pos = {0,0};

            int x = static_cast<int>(pos.x() + 300);
            int y = static_cast<int>(pos.y() + 300);

            if (QLineF(event->pos(), QPointF(x,y)).length() <= 28) {
                found = comp.get();
                break;
            }
        }
    }

    // Tooltip
    if (found != m_hovered) {
        m_hovered = found;
        if (found)
            QToolTip::showText(event->globalPosition().toPoint(),
                QString::fromStdString(found->toString()));
        else
            QToolTip::hideText();
        update();
    }
}