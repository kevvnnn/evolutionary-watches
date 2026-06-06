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

class WatchCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit WatchCanvas(QWidget *parent = nullptr);
    ~WatchCanvas() override = default;

    // Add a setter to inject the real Watch from your algorithm
    void setWatch(WatchGA::Genome::Watch* watch);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void updateAnimation();

private:
    WatchGA::Genome::Watch* m_watch = nullptr; // Non-owning raw pointer
    WatchGA::Genome::WatchComponent* m_hovered = nullptr;
    std::chrono::steady_clock::time_point m_startTime;
    QTimer* m_animationTimer = nullptr;
};

#endif // WATCHCANVAS_H