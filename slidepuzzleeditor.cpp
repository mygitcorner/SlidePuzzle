#include "slidepuzzleeditor.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QRandomGenerator>

SlidePuzzleEditor::SlidePuzzleEditor(QWidget* parent) :
    QWidget(parent),
    m_values(VTILES, QVector<int>(HTILES)),
    m_imageTiles(HTILES * VTILES)
{
    setFocusPolicy(Qt::StrongFocus);
    reset();
}

void SlidePuzzleEditor::setImage(QImage image)
{
    m_image = image;
    m_tileWidth = m_image.width() / HTILES;
    m_tileHeight = m_image.height() / VTILES;

    int index = 0;
    for (int i = 0; i < VTILES; i++)
        for (int j = 0; j < HTILES; j++)
            m_imageTiles[index++] = m_image.copy(
                        j * m_tileWidth, i * m_tileHeight,
                        m_tileWidth, m_tileHeight);

    m_drag.setTileWidth(m_tileWidth);
    m_drag.setTileHeight(m_tileHeight);

    update();
    updateGeometry();
}

bool SlidePuzzleEditor::isSolved() const
{
    int value = 0;
    for (int i = 0; i < VTILES; i++)
        for (int j = 0; j < HTILES; j++)
            if ((i != VTILES - 1 || j != HTILES - 1) && m_values[i][j] != value++)
                    return false;
    return true;
}

void SlidePuzzleEditor::paintEvent(QPaintEvent* /* event */)
{
    QPainter painter(this);
    painter.translate(imageRect().topLeft());

    for (int i = 0; i < VTILES; i++) {
        for (int j = 0; j < HTILES; j++) {
            if (m_values[i][j] != -1) {
                QPointF pos(j * m_tileWidth, i * m_tileHeight);

                if (m_drag.isDragging() && i == m_drag.row() && j == m_drag.column())
                    pos += m_drag.displacement();

                painter.drawImage(pos, m_imageTiles[m_values[i][j]]);
            }
        }
    }
}

void SlidePuzzleEditor::mousePressEvent(QMouseEvent* event)
{

    if (event->button() == Qt::LeftButton &&
            imageRect().contains(event->pos())) {
        QPointF mousePos = event->pos() - imageRect().topLeft();
        int row = mousePos.y() / (m_tileHeight);
        int col = mousePos.x() / (m_tileWidth);

        for (auto& dir : Direction::DIRS) {
            int otherRow = row + dir.dy();
            int otherCol = col + dir.dx();

            if (isValid(otherRow, otherCol) && m_values[otherRow][otherCol] == -1) {
                m_drag.startDrag(row, col, mousePos, dir);
                break;
            }
        }
    }
}

void SlidePuzzleEditor::mouseMoveEvent(QMouseEvent* event)
{
    if (m_drag.isDragging()) {
        m_drag.updateDisplacement(event->pos() - imageRect().topLeft());
        update();
    }
}

void SlidePuzzleEditor::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_drag.isDragging()) {
        if (m_drag.shouldMove())
            moveAndCheck(m_drag.row(), m_drag.column(), m_drag.direction());
        m_drag.stopDrag();
        update();
    }
}

void SlidePuzzleEditor::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
    case Qt::Key_Right:
        moveAndCheck(emptyTilePos() + Direction::LEFT, Direction::RIGHT);
        break;
    case Qt::Key_Left:
        moveAndCheck(emptyTilePos() + Direction::RIGHT, Direction::LEFT);
        break;
    case Qt::Key_Up:
        moveAndCheck(emptyTilePos() + Direction::DOWN, Direction::UP);
        break;
    case Qt::Key_Down:
        moveAndCheck(emptyTilePos() + Direction::UP, Direction::DOWN);
        break;
    default:
        QWidget::keyPressEvent(event);
    }

    update();
}

QSize SlidePuzzleEditor::minimumSizeHint() const
{
    return m_image.size();
}

void SlidePuzzleEditor::shuffle()
{
    reset();
    int row = VTILES - 1;
    int col = HTILES - 1;
    std::vector<Direction> dirs = Direction::DIRS;

    for (int i = 0; i < 1e5; i++) {
        QList<int> allowedIndices;

        for (int j = 0; j < 4; j++)
            if (isValid(row + dirs[j].dy(), col + dirs[j].dx()))
                allowedIndices.append(j);

        int index = allowedIndices[QRandomGenerator::global()->bounded(allowedIndices.size())];
        int reverse = (index + 2) % 4;

        row += dirs[index].dy();
        col += dirs[index].dx();
        move(row, col, dirs[reverse]);
    }

    while (row < VTILES - 1) {
        move(row + 1, col, Direction::UP);
        row++;
    }
    while (col < HTILES - 1) {
        move(row, col + 1, Direction::LEFT);
        col++;
    }

    update();
}

void SlidePuzzleEditor::reset()
{
    int i = 0;
    for (auto& row : m_values)
        for (auto& value : row)
            value = i++;
    m_values.last().last() = -1;
    m_drag.stopDrag();
}

bool SlidePuzzleEditor::move(int row, int col, const Direction& dir)
{
    int otherRow = row + dir.dy();
    int otherCol = col + dir.dx();

    if (!isValid(row, col) || !isValid(otherRow, otherCol) ||
            m_values[row][col] == -1 || m_values[otherRow][otherCol] != -1)
        return false;

    qSwap(m_values[row][col], m_values[otherRow][otherCol]);
    update();
    return true;
}

bool SlidePuzzleEditor::moveAndCheck(int row, int col, const Direction& dir)
{
    bool result = move(row, col, dir);
    if (isSolved())
        emit solved();
    return result;
}

bool SlidePuzzleEditor::moveAndCheck(QPoint pos, const Direction& dir)
{
   return moveAndCheck(pos.y(), pos.x(), dir);
}

bool SlidePuzzleEditor::isValid(int row, int col) const
{
    return row >= 0 && row < VTILES && col >= 0 && col < HTILES;
}

QPoint SlidePuzzleEditor::emptyTilePos() const
{
    for (int i = 0; i < VTILES; i++)
        for (int j = 0; j < HTILES; j++)
            if (m_values[i][j] == -1)
                return QPoint(j, i);
    return QPoint();
}

QRect SlidePuzzleEditor::imageRect() const
{

    return m_image.rect().translated((width() - m_image.width()) / 2,
                        (height() - m_image.height()) / 2);
}
