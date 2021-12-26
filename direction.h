#ifndef DIRECTION_H
#define DIRECTION_H

#include <string>
#include <vector>

class QPoint;

class Direction {
    int m_dx;
    int m_dy;
    std::string m_name;

    Direction(int dx, int dy, const std::string& name);

public:
    static const Direction RIGHT;
    static const Direction LEFT;
    static const Direction UP;
    static const Direction DOWN;
    static const std::vector<Direction> DIRS;

    int dx() const { return m_dx; }
    int dy() const { return m_dy; }
    std::string str() const { return m_name; }

    operator QPoint() const;
};

#endif // DIRECTION_H
