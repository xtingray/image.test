#include "controls.h"

#include <QBoxLayout>
#include <QLabel>

Controls::Controls(QWidget *parent) : QWidget(parent)
{
    QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);

    QLabel *rotationLabel = new QLabel(tr("Rotation"));
    rotationLabel->setAlignment(Qt::AlignHCenter);
    rotationSlider = new QSlider(Qt::Horizontal);
    rotationSlider->setRange(0, 360);
    connect(rotationSlider, SIGNAL(valueChanged(int)), this, SIGNAL(rotationChanged(int)));

    QLabel *scaleLabel = new QLabel(tr("Scale"));
    scaleLabel->setAlignment(Qt::AlignHCenter);
    scaleSlider = new QSlider(Qt::Horizontal);
    scaleSlider->setRange(0, 100);
    connect(scaleSlider, SIGNAL(valueChanged(int)), this, SIGNAL(scaleChanged(int)));

    mainLayout->addWidget(rotationLabel);
    mainLayout->addWidget(rotationSlider);
    mainLayout->addWidget(scaleLabel);
    mainLayout->addWidget(scaleSlider);
    mainLayout->addStretch(2);
}

void Controls::resetSliders()
{
    blockSignals(true);
    rotationSlider->setValue(0);
    scaleSlider->setValue(1);
    blockSignals(false);
}
