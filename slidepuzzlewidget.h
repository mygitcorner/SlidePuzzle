#ifndef SLIDEPUZZLEWIDGET_H
#define SLIDEPUZZLEWIDGET_H

#include <QWidget>

class SlidePuzzleEditor;
class SlidePuzzleViewer;

class SlidePuzzleWidget : public QWidget
{
    Q_OBJECT

public:
    SlidePuzzleWidget(QWidget* parent = nullptr);

public slots:
    void shuffle();
    void setImage(QImage image);

signals:
    void solved();

private:
    SlidePuzzleEditor* m_editor;
    SlidePuzzleViewer* m_viewer;
};

#endif // SLIDEPUZZLEWIDGET_H
