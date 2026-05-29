#ifndef WATCHCANVAS_H
#define WATCHCANVAS_H

#include <QWidget>

class WatchCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit WatchCanvas(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif