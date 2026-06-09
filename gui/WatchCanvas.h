#ifndef WATCHCANVAS_H
#define WATCHCANVAS_H

#include <QWidget>
#include <chrono>
#include <QTimer>

// Forward declarations ONLY (no full class definition needed)
namespace WatchGA::Genome {
    class Watch;
    class WatchComponent;
}

/**
 * @brief Renders the watch and components in the GUI
 *
 * Draws gears, springs, hands, and other parts.
 * Handles mouse hover, tooltips, and animation.
 */
class WatchCanvas : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor - sets up rendering and animation
     * @param parent Parent widget
     */
    explicit WatchCanvas(QWidget *parent = nullptr);
    ~WatchCanvas() override = default;

    /**
     * @brief Sets the watch to display
     * @param watch The watch to render
     */
    void setWatch(WatchGA::Genome::Watch* watch);

protected:
    /// Draws the watch and all components
    void paintEvent(QPaintEvent *event) override;
    /// Detects mouse hover for tooltips
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    /// Updates animation frame
    void updateAnimation();

private:
    WatchGA::Genome::Watch* m_watch = nullptr; // Non-owning raw pointer
    WatchGA::Genome::WatchComponent* m_hovered = nullptr;
    std::chrono::steady_clock::time_point m_startTime;
    QTimer* m_animationTimer = nullptr;
};

#endif // WATCHCANVAS_H