#ifndef SLIDEPUZZLEVIEWER_H
#define SLIDEPUZZLEVIEWER_H

#include <QWidget>

class SlidePuzzleViewer : public QWidget
{
public:
    SlidePuzzleViewer(QWidget* parent = nullptr);

    void setImage(QImage image);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize minimumSizeHint() const override;

private:
    QImage m_image;
};

#endif // SLIDEPUZZLEVIEWER_H
