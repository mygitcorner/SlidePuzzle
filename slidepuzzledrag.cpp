#include "slidepuzzledrag.h"

SlidePuzzleDrag::SlidePuzzleDrag() :
    m_direction(Direction::RIGHT),
    m_isDragging(false)
{
}

void SlidePuzzleDrag::startDrag(int row, int col, QPointF mousePos, Direction dir)
{
    m_row = row;
    m_col = col;
    m_startPos = mousePos;
    m_direction = dir;
    m_isDragging = true;
    m_displacement = QPointF(0, 0);
}

void SlidePuzzleDrag::updateDisplacement(QPointF mousePos)
{
    m_displacement = mousePos - m_startPos;

    double dispx = m_displacement.x() * m_direction.dx();
    double dispy = m_displacement.y() * m_direction.dy();

    double maxdispx = abs(m_direction.dx() * m_tileWidth);
    double maxdispy = abs(m_direction.dy() * m_tileHeight);

    m_displacement.setX(std::max(0.0, std::min(dispx, maxdispx)) * m_direction.dx());
    m_displacement.setY(std::max(0.0, std::min(dispy, maxdispy)) * m_direction.dy());
}

void SlidePuzzleDrag::stopDrag()
{
    m_isDragging = false;
}

bool SlidePuzzleDrag::shouldMove() const
{
    return abs(m_displacement.x()) > m_tileWidth / 2 ||
            abs(m_displacement.y()) > m_tileHeight / 2;
}
