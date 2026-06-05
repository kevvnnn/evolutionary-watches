#include "WatchCanvas.h"
#include <QPainter>
#include <cmath>
#include <QToolTip>
#include <QLineF>
#include <QMouseEvent>


#include "../genome/WatchComponent.h"
#include "../genome/components/BalanceWheel.h"
#include "../genome/components/Gear.h"
#include "../genome/components/Hand.h"
#include "../genome/components/Jewel.h" 
#include "../genome/components/Spring.h"


using namespace WatchGA::Genome;
using namespace WatchGA::Genome::Components; 

WatchCanvas::WatchCanvas(QWidget *parent)
    : QWidget(parent),
      m_startTime(std::chrono::steady_clock::now())
{
    setFixedSize(600, 600);
    setMouseTracking(true);
    setStyleSheet("background-color: #121118;");

    // REAL COMPONENTS FROM YOUR ALGORITHM
    m_components.emplace_back(std::make_unique<Jewel>("Center Jewel", 0.1, 0.02, 300, 300, 9.0, true));
    m_components.emplace_back(std::make_unique<Spring>("Mainspring", 1.8, 0.08, 220, 380, Spring::SpringType::MAINSPRING, 0.92, 0.9, 12.0));
    m_components.emplace_back(std::make_unique<Gear>("Third Gear", 0.7, 0.05, 260, 240, 12, 4.5, 0.88));
    m_components.emplace_back(std::make_unique<Gear>("Second Gear", 0.9, 0.06, 380, 360, 16, 6.0, 0.9));
    m_components.emplace_back(std::make_unique<BalanceWheel>("Balance Wheel", 1.4, 0.12, 360, 220, 2.2, 0.92, 270.0));

    // REAL HANDS — NO DUMMY DRAWING
    m_components.emplace_back(std::make_unique<Hand>("Hour Hand", 0.5, 0.03, 300, 300, Hand::HandType::HOUR, 110.0, 0.95));
    m_components.emplace_back(std::make_unique<Hand>("Minute Hand", 0.4, 0.02, 300, 300, Hand::HandType::MINUTE, 150.0, 0.96));
    m_components.emplace_back(std::make_unique<Hand>("Second Hand", 0.3, 0.02, 300, 300, Hand::HandType::SECOND, 180.0, 0.98));

    // ANIMATION TIMER
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, QOverload<>::of(&QWidget::update));
    m_animationTimer->start(50); 
}

void WatchCanvas::paintEvent(QPaintEvent *event) 
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // BACKGROUND
    p.fillRect(rect(), QColor(20, 20, 20));

    // WATCH FACE
    p.setPen(QPen(Qt::white, 2));
    p.drawEllipse(50, 50, 500, 500);

    // HOUR MARKERS
    for (int i = 0; i < 12; ++i) {
        double ang = i * M_PI/6 - M_PI/2;
        int x1 = 300 + 230 * cos(ang);
        int y1 = 300 + 230 * sin(ang);
        int x2 = 300 + 250 * cos(ang);
        int y2 = 300 + 250 * sin(ang);
        p.drawLine(x1, y1, x2, y2);
    }

    // DRAW COMPONENTS
    for (const auto& comp : m_components) {
        bool hovered = (comp.get() == m_hovered);
        QColor color;

        // COLORS
        if (hovered)
            color = QColor(255, 210, 0);
        else if (dynamic_cast<Gear*>(comp.get()))
            color = QColor(90, 160, 240);
        else if (dynamic_cast<Spring*>(comp.get()))
            color = QColor(255, 180, 0);
        else if (dynamic_cast<BalanceWheel*>(comp.get())) 
            color = QColor(220, 50, 50);
        else if (dynamic_cast<Jewel*>(comp.get()))
            color = QColor(190, 190, 220);
        else if (dynamic_cast<Hand*>(comp.get()))
            color = QColor(240, 240, 240);
        else
            color = QColor(160, 160, 160);

        // DRAW HANDS AS REAL LINES (NOT CIRCLES)
        if (Hand* hand = dynamic_cast<Hand*>(comp.get())) {
            double angle = 3 * M_PI / 2;

            // REAL TICKING TIME
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - m_startTime).count();

            if (hand->getType() == Hand::HandType::HOUR) {
                angle += (elapsed % 43200) * (2 * M_PI / 43200.0);
                p.setPen(QPen(color, 5));
            }
            else if (hand->getType() == Hand::HandType::MINUTE) {
                angle += (elapsed % 3600) * (2 * M_PI / 3600.0);
                p.setPen(QPen(color, 3));
            }
            else if (hand->getType() == Hand::HandType::SECOND) {
                angle += (elapsed % 60) * (100000 * M_PI / 60.0);
                p.setPen(QPen(Qt::red, 2));
            }

            double len = hand->getLength();
            int ex = 300 + len * cos(angle);
            int ey = 300 + len * sin(angle);
            p.drawLine(300, 300, ex, ey);
            continue;
        }

        // DRAW OTHER COMPONENTS AS CIRCLES
        p.setBrush(color);
        p.setPen(Qt::black);
        p.drawEllipse(QPointF(comp->getX(), comp->getY()), 26, 26);
    }
}

void WatchCanvas::mouseMoveEvent(QMouseEvent *event)
{
    WatchComponent* found = nullptr;

    for (const auto& comp : m_components) {
        QPointF pos(comp->getX(), comp->getY());
        if (QLineF(event->pos(), pos).length() <= 30) {
            found = comp.get();
            break;
        }
    }

    if (found != m_hovered) {
        m_hovered = found;
        if (found)
            QToolTip::showText(event->globalPosition().toPoint(), QString::fromStdString(found->toString()));
        else
            QToolTip::hideText();
        update();
    }
}  