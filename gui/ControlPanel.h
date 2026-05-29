#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
class QPushButton;

class ControlPanel: public QWidget
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget* parent = nullptr);

signals:
    void runClicked();
    void resetClicked();
    void stepClicked();

private:
    QPushButton* runBtn;
    QPushButton* resetBtn;
    QPushButton* stepBtn;

    void setupUI();
};

#endif