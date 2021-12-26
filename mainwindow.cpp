#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "slidepuzzlewidget.h"
#include <QFileDialog>
#include <QToolBar>
#include <QStandardPaths>
#include <QSettings>
#include <QMessageBox>
#include <QImageReader>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_settings("capsule-corp.", "slide-puzzle")
{
    ui->setupUi(this);

    connect(ui->shuffleButton, &QPushButton::clicked,
            ui->slidePuzzleWidget, &SlidePuzzleWidget::shuffle);
    connect(ui->slidePuzzleWidget, &SlidePuzzleWidget::solved,
            this, &MainWindow::solved);

    m_openAction = new QAction("Open");
    m_openAction->setIcon(QIcon(":/images/image-gallery-icon.png"));
    m_openAction->setShortcut(QKeySequence::Open);
    m_openAction->setStatusTip(tr("Open an image"));
    connect(m_openAction, &QAction::triggered, this, &MainWindow::open);

    QToolBar* toolbar = addToolBar("Image");
    toolbar->addAction(m_openAction);

    QImage lastImage(m_settings.value("imagePath").toString());
    ui->slidePuzzleWidget->setImage(lastImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open()
{
    QStringList picturesPaths = QStandardPaths::standardLocations(
                QStandardPaths::PicturesLocation);
    QString picturesPath = picturesPaths.isEmpty()
            ? QDir::root().path()
            : picturesPaths.first();

    QStringList supportedFormats;
    for (auto& format : QImageReader::supportedImageFormats())
        supportedFormats.append("*." + format);

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), picturesPath,
                                                    tr("Images (%1)").arg(supportedFormats.join(" ")));

    if (!fileName.isEmpty()) {
        ui->slidePuzzleWidget->setImage(QImage(fileName));
        QSettings settings("capsule-corp.", "slide-puzzle");
        settings.setValue("imagePath", fileName);
    }
}

void MainWindow::solved()
{
    QMessageBox::information(this, tr("Slide Puzzle"), tr("You solved it!"));
}

