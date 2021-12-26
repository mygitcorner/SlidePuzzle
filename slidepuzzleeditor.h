#ifndef SLIDEPUZZLEEDITOR_H
#define SLIDEPUZZLEEDITOR_H

#include "slidepuzzledrag.h"
#include <QWidget>

class SlidePuzzleEditor : public QWidget
{
    Q_OBJECT

    static const int HTILES = 4;
    static const int VTILES = 4;

public:
    SlidePuzzleEditor(QWidget* parent = nullptr);

    void setImage(QImage image);
    bool isSolved() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    QSize minimumSizeHint() const override;

public slots:
    void shuffle();

signals:
    void solved();

private:
    void reset();
    bool move(int row, int col, const Direction& dir);
    bool moveAndCheck(int row, int col, const Direction& dir);
    bool moveAndCheck(QPoint pos, const  Direction& dir);
    bool isValid(int row, int col) const;
    QPoint emptyTilePos() const;
    QRect imageRect() const;

    QImage m_image;
    QVector<QVector<int>> m_values;
    QVector<QImage> m_imageTiles;
    SlidePuzzleDrag m_drag;

    int m_tileWidth;
    int m_tileHeight;
};

#endif // SLIDEPUZZLEEDITOR_H
