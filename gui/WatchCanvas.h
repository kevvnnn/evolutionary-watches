#ifndef WATCHCANVAS_H
#define WATCHCANVAS_H

#include <QWidget>
#include <QTimer>
#include <vector>
#include <memory>
#include <chrono>

namespace WatchGA {
namespace Genome {
    class WatchComponent;
} // namespace Genome
} // namespace WatchGA

class WatchCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit WatchCanvas(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    std::vector<std::unique_ptr<WatchGA::Genome::WatchComponent>> m_components;
    WatchGA::Genome::WatchComponent* m_hovered = nullptr;
    std::chrono::steady_clock::time_point m_startTime;
    QTimer* m_animationTimer;
};

#endif // WATCHCANVAS_H