#include "mainwindow.h"

#include <QDebug>
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
    qDebug() << "MainWindow::importBitmap() - image: " << path;
    QFile file(path);
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray array = file.readAll();
            QString msg1 = "MainWindow::importBitmap() - Object path: " + path;
            QString msg2 = "MainWindow::importBitmap() - Object size: " + QString::number(array.size());
            qWarning() << msg1;
            qWarning() << msg2;
            if (!array.isEmpty() && !array.isNull()) {
                loadRawData(path, array);
            } else {
                QString msg = "MainWindow::importBitmap() - Warning: Image file is empty -> " + path;
                qDebug() << msg;
                return;
            }
        } else {
            QString msg = "MainWindow::importBitmap() - Fatal Error: Can't access image file -> " + path;
            qDebug() << msg;
            return;
        }
    } else {
        QString msg = "MainWindow::importBitmap() - Fatal Error: Image file doesn't exist -> " + path;
        qDebug() << msg;
        return; 
    }
}

void MainWindow::loadRawData(const QString &path, const QByteArray &data)
{
    Q_UNUSED(data);

    /*
    int index = path.length() - path.lastIndexOf(".") - 1;
    QString extension = path.right(index).toUpper();
    if (extension.length() > 0) {
        qDebug() << "index: " << index;
        qDebug() << "EXT: " << extension;
        QByteArray array = extension.toUtf8();
        QPixmap pixmap(path);
        bool isOk = pixmap.loadFromData(data, array.data());
        if (!isOk) {
            QString msg = "MainWindow::loadRawData() - Fatal Error: Can't load image -> " + path;
            qDebug() << msg;
            return;
        }
    */

        QPixmap pixmap(path);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem;
        item->setPixmap(pixmap);
        scene->clear();
        scene->addItem(item);
    // }
}
