#ifndef WATCHCOMPONENT_H
#define WATCHCOMPONENT_H

#include <QString>
#include <QPoint>

class WatchComponent {
public:
    WatchComponent(const QString& name, const QString& type, int size, const QPoint& pos, double gearRatio = 0.0)
        : m_name(name), m_type(type), m_size(size), m_pos(pos), m_gearRatio(gearRatio) {}

    QString getName() const { return m_name; }
    QString getType() const { return m_type; }
    int getSize() const { return m_size; }
    QPoint getPosition() const { return m_pos; }
    double getGearRatio() const { return m_gearRatio; }

private:
    QString m_name;
    QString m_type;
    int m_size;
    QPoint m_pos;
    double m_gearRatio;
};

#endif // WATCHCOMPONENT_H