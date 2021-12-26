#include "direction.h"

#include <QPoint>

Direction::Direction(int dx, int dy, const std::string& name)
    : m_dx{ dx }, m_dy{ dy }, m_name{ name } {
}

Direction::operator QPoint() const
{
    return QPoint(m_dx, m_dy);
}

const Direction Direction::RIGHT(1, 0, "RIGHT");
const Direction Direction::LEFT(-1, 0, "LEFT");
const Direction Direction::UP(0, -1, "UP");
const Direction Direction::DOWN(0, 1, "DOWN");
const std::vector<Direction> Direction::DIRS = { Direction::RIGHT, Direction::DOWN, Direction::LEFT , Direction::UP };
