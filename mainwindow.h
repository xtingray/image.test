#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QGraphicsScene>
#include <QGraphicsView>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();

    private slots:
        void loadImage();

    private:
        void importBitmap(const QString &path);
        void loadRawData(const QString &path, const QByteArray &data);

        QMenu *fileMenu;
        QAction *loadAction;
        QAction *exitAction;
        QGraphicsScene *scene;
        QGraphicsView *view;
};

#endif
