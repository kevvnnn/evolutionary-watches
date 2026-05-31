#ifndef WATCHCANVAS_H
#define WATCHCANVAS_H

#include <QWidget>
#include <QPushButton>
#include "StatsPanel.h"

class WatchCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit WatchCanvas(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QPushButton* statsButton;
    StatsPanel* statsPanel;
    void updateButtonPosition();

private slots:
    void showStatsPanel();
};

#endif