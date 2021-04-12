#ifndef VISION_THREAD_H
#define VISION_THREAD_H

#include <QObject>
#include "mainwindow.h"
#include "QThread"


class Vision_Thread : public QThread
{
    Q_OBJECT
public:
    Vision_Thread();

private slots:
        void run();


};

#endif // VISION_THREAD_H
