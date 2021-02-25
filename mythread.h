#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <fstream>
#include <sstream>

#include <QFile>
#include <QFileInfo>

#include <iostream>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <ostream>
#include <stdio.h>


#include "mainwindow.h"

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = nullptr, bool stop = 0, QString path = NULL, int passe = 0);
    void run();
    bool m_stop;
    QString m_path;
    int m_passe;
    void value(bool);

signals:
    void sendValue(bool);

};

#endif // MYTHREAD_H
