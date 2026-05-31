#ifndef STATSPANEL_H
#define STATSPANEL_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

class StatsPanel : public QDialog
{
    Q_OBJECT

public:
    explicit StatsPanel(QWidget *parent = nullptr);
    ~StatsPanel();

private:
    QVBoxLayout* mainLayout;
    QHBoxLayout* headerLayout;
    QLabel* titleLabel;
    QPushButton* closeButton;
    QLabel* placeholderLabel;

private slots:
    void handleClose();
};

#endif // STATSPANEL_H