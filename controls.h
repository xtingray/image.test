#ifndef CONTROLS_H
#define CONTROLS_H

#include <QWidget>
#include <QSlider>

class Controls : public QWidget
{
    Q_OBJECT

    public:
        Controls(QWidget *parent = 0);
        void resetSliders();

    signals:
        void rotationChanged(int angle);
        void scaleChanged(int percent);

    private:
        QSlider *rotationSlider;
        QSlider *scaleSlider;
};

#endif
