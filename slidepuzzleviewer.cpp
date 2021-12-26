#include "slidepuzzleviewer.h"

#include <QPainter>

SlidePuzzleViewer::SlidePuzzleViewer(QWidget* parent) :
    QWidget(parent)
{ 
}

void SlidePuzzleViewer::setImage(QImage image)
{
    m_image = image;
    update();
    updateGeometry();
}

void SlidePuzzleViewer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawImage((width() - m_image.width()) / 2,
                      (height() - m_image.height()) / 2,
                      m_image);
}

QSize SlidePuzzleViewer::minimumSizeHint() const
{
    return m_image.size();
}
