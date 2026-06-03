#ifndef WATCHCANVAS_H
#define WATCHCANVAS_H

#include <QWidget>
#include <QMouseEvent>
#include <vector>
#include "WatchComponent.h"

class WatchCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit WatchCanvas(QWidget *parent = nullptr);

signals:
    void componentHovered(WatchComponent* comp);
    void componentLeft();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    std::vector<WatchComponent*> m_components;
    WatchComponent* m_hovered = nullptr;
    WatchComponent* getComponentAt(const QPoint& pos);
};

#endif