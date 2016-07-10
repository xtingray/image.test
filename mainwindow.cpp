#include "mainwindow.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QDockWidget>

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

    panel = new Controls(this);

    QDockWidget *dock = new QDockWidget(tr("Controls"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(panel);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    connect(panel, SIGNAL(rotationChanged(int)), this, SLOT(rotate(int)));
    connect(panel, SIGNAL(scaleChanged(int)), this, SLOT(scale(int)));

    showFullScreen();
    
    rotationAngle = 0;
    scaleFactor = 1;
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
    panel->resetSliders();
    rotationAngle = 0;
    scaleFactor = 1;

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

void MainWindow::rotate(int angle)
{
    if (item) {
        QPointF point = item->boundingRect().center();
        QTransform t;
        t.translate(point.x(), point.y());
        t.rotate(angle);
        t.scale(scaleFactor, scaleFactor);
        t.translate(-point.x(), -point.y());
        item->setTransform(t);
        rotationAngle = angle;

        manager->syncNodes();
    }
}

void MainWindow::scale(int percent)
{
    if (item) {
        QPointF point = item->boundingRect().center();
        QTransform t;
        t.translate(point.x(), point.y());
        t.rotate(rotationAngle);
        t.scale(percent, percent);
        t.translate(-point.x(), -point.y());
        item->setTransform(t);
        scaleFactor = percent;

        manager->syncNodes();
    }
}
