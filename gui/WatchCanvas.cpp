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

// -----------------------------------------------------------------------------
// Helper: Draw a REAL gear from YOUR GENOME DATA (tooth count, diameter, quality)
// -----------------------------------------------------------------------------
static QPainterPath createGearPath(const Gear* gear, qreal cx, qreal cy)
{
    QPainterPath path;

    int    teeth    = gear->getToothCount();
    double diameter = gear->getDiameter();
    double quality  = gear->getMeshingQuality();
    (void)quality; // Suppress unused variable warning

    // Safe limits for tooth count
    if (teeth < 6) teeth = 6;
    if (teeth > 24) teeth = 24;

    // DIRECTLY SCALE diameter TO PIXEL SIZE (PROPORTIONAL!)
    // Formula: baseRadius = (diameter * scaleFactor) + minSize
    // This ensures:
    // - smallest diameter = smallest gear
    // - largest diameter = largest gear
    // - no arbitrary hard caps breaking proportionality
    const qreal scaleFactor = 45.0;  // Adjust this to change overall gear size
    const qreal minBaseRadius = 5.0;  // Minimum size for tiny gears
    qreal baseRadius = minBaseRadius + (diameter * scaleFactor);

    // ✅ Teeth are ALWAYS 22% of base radius (PROPORTIONAL!)
    qreal toothHeight = baseRadius * 0.22;
    qreal outerRadius = baseRadius + toothHeight;

    // Optional: Soft upper limit (only to prevent extreme overflow)
    const qreal maxOuterRadius = 35.0;
    if (outerRadius > maxOuterRadius) {
        outerRadius = maxOuterRadius;
        baseRadius = outerRadius / (1 + 0.22); // Keep tooth ratio intact
    }

    qreal step = (2 * M_PI) / teeth;
    qreal angle = -M_PI / 2; // Start at 12 o'clock

    // Draw gear teeth
    path.moveTo(cx + outerRadius * qCos(angle), cy + outerRadius * qSin(angle));

    for (int i = 0; i < teeth; ++i)
    {
        // Tooth tip
        path.lineTo(cx + outerRadius * qCos(angle), cy + outerRadius * qSin(angle));
        angle += step * 0.4;

        // Tooth valley
        path.lineTo(cx + baseRadius * qCos(angle), cy + baseRadius * qSin(angle));
        angle += step * 0.6;
    }

    path.closeSubpath();
    return path;
}

WatchCanvas::WatchCanvas(QWidget *parent)
    : QWidget(parent),
      m_startTime(std::chrono::steady_clock::now())
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

void WatchCanvas::updateAnimation()
{
    update();
}

void WatchCanvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    p.fillRect(rect(), QColor(20, 20, 20));

    const int centerX = 300;
    const int centerY = 300;
    const int dialRadius = 250;

    // Watch dial
    p.setPen(QPen(Qt::white, 2));
    p.drawEllipse(50, 50, 500, 500);

    // Hour marks
    for (int i = 0; i < 12; ++i) {
        double ang = i * M_PI / 6 - M_PI / 2;
        int x1 = centerX + 230 * cos(ang);
        int y1 = centerY + 230 * sin(ang);
        int x2 = centerX + 250 * cos(ang);
        int y2 = centerY + 250 * sin(ang);
        p.drawLine(x1, y1, x2, y2);
    }

    if (!m_watch) return;

    const auto& components = m_watch->getAllComponents();
    for (const auto& comp : components) {
        bool hovered = (comp.get() == m_hovered);
        QColor color;

        if (hovered)
            color = QColor(255, 210, 0);
        else if (dynamic_cast<const Gear*>(comp.get()))
            color = QColor(90, 160, 240);
        else if (dynamic_cast<const Spring*>(comp.get()))
            color = QColor(255, 180, 0);
        else if (dynamic_cast<const BalanceWheel*>(comp.get()))
            color = QColor(220, 50, 50);
        else if (dynamic_cast<const Jewel*>(comp.get()))
            color = QColor(190, 190, 220);
        else if (dynamic_cast<const Hand*>(comp.get()))
            color = QColor(240, 240, 240);
        else
            color = QColor(160, 160, 160);

        double x = comp->getX() + centerX;
        double y = comp->getY() + centerY;

        // ------------------------------
        // Draw Hands
        // ------------------------------
        if (const Hand* hand = dynamic_cast<const Hand*>(comp.get())) {
            double angle = 3 * M_PI / 2;
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - m_startTime).count();

            qreal scale;
            if (hand->getType() == Hand::HandType::HOUR) {
                angle += (elapsed % 43200) * (2 * M_PI / 43200.0);
                p.setPen(QPen(color, 5));
                scale = 0.65;
            } else if (hand->getType() == Hand::HandType::MINUTE) {
                angle += (elapsed % 3600) * (2 * M_PI / 3600.0);
                p.setPen(QPen(color, 3));
                scale = 0.80;
            } else {
                angle += (elapsed % 60) * (2 * M_PI / 60.0);
                p.setPen(QPen(Qt::red, 2));
                scale = 0.85;
            }

            qreal len = dialRadius * scale;
            int ex = centerX + (int)(len * cos(angle));
            int ey = centerY + (int)(len * sin(angle));
            p.drawLine(centerX, centerY, ex, ey);
            continue;
        }

        p.setBrush(color);
        p.setPen(Qt::black);

        // ------------------------------
        // DRAW REAL GEAR FROM DATA
        // ------------------------------
        if (const Gear* gear = dynamic_cast<const Gear*>(comp.get())) {
            QPainterPath gearPath = createGearPath(gear, x, y);
            p.drawPath(gearPath);
        }
        // ------------------------------
        // All other parts = circles
        // ------------------------------
        else {
            p.drawEllipse(QPointF(x, y), 24, 24);
        }
    }
}

void WatchCanvas::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_watch) return;

    WatchComponent* found = nullptr;
    const auto& components = m_watch->getAllComponents();

    for (const auto& comp : components) {
        double x = comp->getX() + 300;
        double y = comp->getY() + 300;
        if (QLineF(event->pos(), QPointF(x, y)).length() <= 28) {
            found = comp.get();
            break;
        }
    }

    if (found != m_hovered) {
        m_hovered = found;
        if (found) {
            QString tip = QString::fromStdString(found->toString());
            QToolTip::showText(event->globalPosition().toPoint(), tip);
        } else {
            QToolTip::hideText();
        }
        update();
    }

    
}