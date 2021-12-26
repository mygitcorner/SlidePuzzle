#include "slidepuzzleeditor.h"
#include "slidepuzzleviewer.h"
#include "slidepuzzlewidget.h"
#include <QDebug>
#include <QFrame>
#include <QHBoxLayout>

SlidePuzzleWidget::SlidePuzzleWidget(QWidget* parent) :
    QWidget(parent)
{
    m_viewer = new SlidePuzzleViewer;
    m_editor = new SlidePuzzleEditor;
    m_editor->setEnabled(false);

    QFrame* separator = new QFrame;
    separator->setFrameShape(QFrame::VLine);
    separator->setFrameShadow(QFrame::Sunken);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(m_viewer);
    layout->addWidget(separator);
    layout->addWidget(m_editor);
    setLayout(layout);

    connect(m_editor, &SlidePuzzleEditor::solved,
            [this] {
        m_editor->setEnabled(false);
        emit solved();
    });
}

void SlidePuzzleWidget::shuffle()
{
    m_editor->shuffle();
    m_editor->setEnabled(true);
}

void SlidePuzzleWidget::setImage(QImage image)
{
    QImage scaledImage = image.scaled(QSize(350, 350), Qt::KeepAspectRatio);
    m_viewer->setImage(scaledImage);
    m_editor->setImage(scaledImage);
    shuffle();
}
