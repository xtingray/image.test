#include "mainwindow.h"

#include <QFileDialog>
#include <QFile>
#include <QByteArray>
#include <QPixmap>
#include <QGraphicsPixmapItem>

MainWindow::MainWindow()
{
    loadAction = new QAction(tr("&Load image"), this);
    loadAction->setShortcuts(QKeySequence::Open);
    loadAction->setStatusTip(tr("Load image"));
    connect(loadAction, SIGNAL(triggered()), this, SLOT(loadImage()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(loadAction);
    fileMenu->addAction(exitAction);

    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);

    setCentralWidget(view);

    showFullScreen();
}

void MainWindow::loadImage()
{
    QFileDialog dialog(this, tr("Load images..."), QDir::homePath());
    dialog.setNameFilter(tr("Images") + " (*.png *.xpm *.jpg *.jpeg *.gif)");
    dialog.setFileMode(QFileDialog::ExistingFiles);

    if (dialog.exec() == QDialog::Accepted) {
        QStringList files = dialog.selectedFiles();
        int size = files.size();
        for (int i = 0; i < size; ++i)
             importBitmap(files.at(i));
    }
}

void MainWindow::importBitmap(const QString &path)
{
    QPixmap pixmap(path);
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem;
    item->setPixmap(pixmap);
    scene->clear();
    scene->addItem(item);
}
