#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "nodemanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();

    private slots:
        void loadImage();
        void resetSpace();

    private:
        void importBitmap(const QString &path);

        QMenu *fileMenu;
        QAction *loadAction;
        QAction *resetAction;
        QAction *exitAction;
        QGraphicsScene *scene;
        QGraphicsView *view;
        QGraphicsPixmapItem *item;
        NodeManager *manager;
};

#endif
