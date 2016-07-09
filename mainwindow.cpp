#include "mainwindow.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QPixmap>
#include <QGraphicsPixmapItem>

MainWindow::MainWindow()
{
    loadAction = new QAction(tr("&Load image"), this);
    loadAction->setShortcuts(QKeySequence::Open);
    loadAction->setStatusTip(tr("Load image"));
    connect(loadAction, SIGNAL(triggered()), this, SLOT(loadImage()));

    resetAction = new QAction(tr("&Reset workspace"), this);
    resetAction->setShortcuts(QKeySequence::Delete);
    resetAction->setStatusTip(tr("Reset workspace"));
    connect(resetAction, SIGNAL(triggered()), this, SLOT(resetSpace()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(loadAction);
    fileMenu->addAction(resetAction);
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
    item = new QGraphicsPixmapItem;
    item->setPixmap(pixmap);
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setFlag(QGraphicsItem::ItemIsMovable, false);
    scene->clear();
    scene->addItem(item);
    item->setSelected(true);

    manager = new NodeManager(item, scene, 10);
    manager->syncNodes();
}

void MainWindow::resetSpace()
{
    scene->removeItem(item);
    manager->clear();
}
