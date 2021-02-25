#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QSizeGrip>
#include <QGridLayout>
#include <QCoreApplication>
#include <QMouseEvent>
#include <QPen>
#include <QMainWindow>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QSlider>
#include <QtSvg/QSvgWidget>
#include <QMessageBox>
#include "mythread.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);


private:
    QString styleStart;
    QString styleStop;
    QPixmap *m_Cache;
    QString m_path;
    bool debut = 0;
    QPushButton *btn_path;
    QPushButton *btn_stop;
    QPushButton *btn_close;
    QPushButton *btn_info;
    QPushButton *btn_minimize;
    QMessageBox *msgBoxInfo;
    QLabel *lb_passe;
    QLabel *lb_title;
    QLabel *lb_file;
    QSlider *sld_passe;
    QSvgWidget *svgLoad;
    int m_DiffX;
    int m_DiffY;
    bool move = 0;

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void receiveValue(bool);

private slots:
    void showPasse(int);
    void quit();
    void minimize();
    void selectPath();
    void stopStart();
    QFont font(int);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void infoWindow();


};




#endif // MAINWINDOW_H

