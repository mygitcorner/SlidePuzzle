#ifndef SIDEPUZZLEDRAG_H
#define SIDEPUZZLEDRAG_H

#include "direction.h"

#include <QPointF>


class SlidePuzzleDrag
{
public:
    SlidePuzzleDrag();

    void startDrag(int row, int col, QPointF mousePos, Direction dir);
    void updateDisplacement(QPointF mousePos);
    void stopDrag();

    QPointF displacement() const { return m_displacement; }
    bool isDragging() const { return m_isDragging; }
    int row() const { return m_row; }
    int column() const { return m_col; }
    Direction direction() const { return m_direction; }
    bool shouldMove() const;

    void setTileWidth(int width) { m_tileWidth = width; }
    void setTileHeight(int height) { m_tileHeight = height; }

private:
    QPointF m_startPos;
    QPointF m_displacement;
    Direction m_direction;

    int m_row;
    int m_col;
    bool m_isDragging;

    int m_tileWidth;
    int m_tileHeight;
};

#endif // SIDEPUZZLEDRAG_H
