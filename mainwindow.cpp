#include "mainwindow.h"

#include <QDebug>

#include <QMenu>
#include <QMenuBar>
#include <QToolButton>
#include <QSystemTrayIcon>

MyThread *mThread;

bool fileExists(QString path) {

    QFileInfo file(path);
    QFile file2(path);

    if (file.exists() && file.isFile() && (file2.open(QFile::ReadOnly | QFile::Text)) ) {
        return true;
    }

    return false;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    setAttribute(Qt::WA_TranslucentBackground);

    resize(512, 512);

    QSvgWidget *svgLogo = new QSvgWidget(QString(":/svg/logo.svg") ,this);
    m_Cache = NULL;
    btn_path = new QPushButton(this);
    btn_stop = new QPushButton(this);
    btn_close = new QPushButton(this);
    btn_info = new QPushButton(this);
    btn_minimize = new QPushButton(this);
    lb_passe = new QLabel(this);
    lb_file = new QLabel(this);
    lb_title = new QLabel(this);
    sld_passe = new QSlider (Qt::Horizontal,this);
    svgLoad = new QSvgWidget(this);
    msgBoxInfo = new QMessageBox(this);
    QColor grey (45,45,45);
    QColor green (40,143,90);

    msgBoxInfo->setText("<p>DeleteSure is a project carried out by Claverie Jean, the aim is to make the data contained in a file unrecoverable by software such as <a href='https://www.ccleaner.com/recuva'>Recuva</a>, <a href='https://www.cleverfiles.com/fr/disk-drill-windows.html'>DiskDrill</a>...</p> <p>Its operating principle is simple, DeleteSure overwrites the data in the file by replacing it with random data in order to make the initial data unrecoverable.</p> <p>This operation is performed as many times as there is a selected pass.</p> <p><a href='https://goracode.com'>Claverie Jean</a>.</p>");

    lb_title->setText("DeleteSure | Claverie Jean");
    lb_passe->setText("Number of passes : <font color='red'>1</font>");
    btn_stop->setEnabled(false);

    QFile fileStyle(":/style/slider_passe.qss");
    fileStyle.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(fileStyle.readAll());
    sld_passe->setStyleSheet(styleSheet);

    fileStyle.remove();
    fileStyle.setFileName(":/style/btn_start.qss");
    fileStyle.open(QFile::ReadOnly);
    styleStart = QLatin1String(fileStyle.readAll());

    fileStyle.remove();
    fileStyle.setFileName(":/style/btn_stop.qss");
    fileStyle.open(QFile::ReadOnly);
    styleStop = QLatin1String(fileStyle.readAll());
    btn_stop->setStyleSheet(styleStart);

    fileStyle.remove();
    fileStyle.setFileName(":/style/btn_info.qss");
    fileStyle.open(QFile::ReadOnly);
    styleSheet = QLatin1String(fileStyle.readAll());
    btn_info->setStyleSheet(styleSheet);

    fileStyle.remove();
    fileStyle.setFileName(":/style/btn_close.qss");
    fileStyle.open(QFile::ReadOnly);
    styleSheet = QLatin1String(fileStyle.readAll());
    btn_close->setStyleSheet(styleSheet);

    fileStyle.remove();
    fileStyle.setFileName(":/style/btn_minimize.qss");
    fileStyle.open(QFile::ReadOnly);
    styleSheet = QLatin1String(fileStyle.readAll());
    btn_minimize->setStyleSheet(styleSheet);

    fileStyle.remove();
    fileStyle.setFileName(":/style/btn_path.qss");
    fileStyle.open(QFile::ReadOnly);
    styleSheet = QLatin1String(fileStyle.readAll());
    btn_path->setStyleSheet(styleSheet);

    lb_title->setFont(font(10));
    lb_passe->setFont(font(15));
    lb_file->setFont(font(11));
    sld_passe->setCursor(Qt::PointingHandCursor);
    sld_passe->setMinimum(1);
    sld_passe->setMaximum(32);

    lb_title->setGeometry(40,5,285,25);
    btn_close->setGeometry(482,8,20,20);
    btn_minimize->setGeometry(455,8,20,20);
    btn_path->setGeometry(165,110,180,70);
    btn_stop->setGeometry(165,185,180,70);
    btn_info->setGeometry(420,8,20,20);
    sld_passe->setGeometry(70,330,350,40);
    lb_passe->setGeometry(110,375,300,50);
    svgLogo->setGeometry(3, 3, 30, 30);
    svgLoad->setGeometry(208, 250, 100, 100);
    lb_file->setGeometry(50, 250, 400, 100);
    lb_file->setAlignment(Qt::AlignCenter);

    btn_info->setToolTip("Information");
    btn_path->setToolTip("Select file");
    btn_minimize->setToolTip("Minimize");
    btn_close->setToolTip("Close");

    QObject::connect(sld_passe, SIGNAL(valueChanged(int)), this, SLOT(showPasse(int)));
    QObject::connect(btn_close, SIGNAL(clicked()), this, SLOT(quit()));
    QObject::connect(btn_minimize, SIGNAL(clicked()), this, SLOT(minimize()));
    QObject::connect(btn_path, SIGNAL(clicked()), this, SLOT(selectPath()));
    QObject::connect(btn_stop, SIGNAL(clicked()), this, SLOT(stopStart()));
    QObject::connect(btn_info, SIGNAL(clicked()), this, SLOT(infoWindow()));

    m_Cache = new QPixmap(size());
    m_Cache->fill(Qt::transparent);

    QPainter painter(m_Cache);
    painter.setPen(Qt::NoPen);

    QPolygon background;
    background << QPoint(0, 0)
               << QPoint(512,  0)
               << QPoint(512, 127)
               << QPoint(500, 140)
               << QPoint(500, 384)
               << QPoint(512, 396)
               << QPoint(512, 512)
               << QPoint(396, 512)
               << QPoint(384, 500)
               << QPoint(30, 500)
               << QPoint(30, 500)
               << QPoint(12, 482)
               << QPoint(12, 140)
               << QPoint(0, 128);

    painter.setBrush(QBrush(grey));
    painter.drawPolygon(background);

    QPolygon polygon;
    polygon << QPoint(400, 35)
            << QPoint(0, 35)
            << QPoint(0, 60)
            << QPoint(400, 60);

    QLinearGradient linearGrad(QPointF(0, 50), QPointF(0, 0));
    linearGrad.setColorAt(0, grey);
    linearGrad.setColorAt(1, green);

    painter.setBrush(QBrush(linearGrad));
    painter.drawPolygon(polygon);

    QPolygon polygon2;
    polygon2   << QPoint(512, 475)
               << QPoint(512, 512)
               << QPoint(396, 512)
               << QPoint(384, 500)
               << QPoint(30, 500)
               << QPoint(30, 500)
               << QPoint(12, 482)
               << QPoint(12, 475);

    QLinearGradient linearGrad2( QPointF(0, 512), QPointF(0, 480));
    linearGrad2.setColorAt(0, green);
    linearGrad2.setColorAt(1, grey);

    painter.setBrush(QBrush(linearGrad2));
    painter.drawPolygon(polygon2);

    QPolygon btnBox;
    btnBox << QPoint(width() -  5,  4 )
            << QPoint(width() -  5, 33)
            << QPoint(width() - 88 + 25, 33)
            << QPoint(width() - 88 + 25, 4);
    painter.setPen  (QPen  (green));
    painter.setBrush(QBrush(green));
    painter.drawPolygon(btnBox);

}


void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    sld_passe->setCursor(Qt::ArrowCursor);

    if(m_Cache != NULL)
    {
        QPainter painter(this);

        painter.drawPixmap(0, 0, *m_Cache);
    }
}

void MainWindow::showPasse(int passe)
{
    QString color;

    if (passe > 20){

        color = "green";

    } else if (passe > 10) {

        color = "orange";

    } else{
        color = "red";
    }

    lb_passe->setText("Number of passes : <font color='" + color + "'>" + QString::number(passe) + "</font>");

}


void MainWindow::selectPath()
{

    QFileInfo fichier = QFileDialog::getOpenFileName(this, "Select file", QString(), "File (*.*)");

    m_path = fichier.absoluteFilePath();

    if (fileExists(m_path)){

        btn_stop->setEnabled(true);

        btn_stop->setToolTip("Start");

        lb_file->setText("Selected file : <font color='green'>" + fichier.fileName() + "</font>");

    } else {
        btn_stop->setEnabled(false);
    }

}

void MainWindow::stopStart()
{
    if (m_path != "" && fileExists(m_path)){

        if (!debut){

            svgLoad->load(QString(":/svg/loaderStart.svg"));

            int m_passe = sld_passe->value();
            mThread = new MyThread(this, false, m_path, m_passe);

            mThread->start();

            btn_stop->setStyleSheet(styleStop);

            receiveValue(0);

            btn_stop->setEnabled(true);

            btn_stop->setToolTip("Stop");

            lb_file->setText("");

        } else {

            receiveValue(0);

            svgLoad->load(QString(":/svg/loaderStop.svg"));

            mThread->m_stop = true;

            btn_stop->setEnabled(false);
        }

    }
}

void MainWindow::receiveValue(bool value)
{
    debut = !value;

    btn_path->setEnabled(value);

    if (value){
        btn_stop->setStyleSheet(styleStart);
        btn_stop->setToolTip("Start");
        m_path = "";
        btn_stop->setEnabled(false);
        svgLoad->close();
    }


}

void MainWindow::quit()
{
    QCoreApplication::quit();
}

void MainWindow::minimize()
{
    showMinimized();
}

void MainWindow::infoWindow(){

    msgBoxInfo->exec();
}

QFont MainWindow::font(int size){

    QFont font("Ubuntu", size, QFont::Medium);
    return font;
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->y() < 35 && event->x() < 450){

        m_DiffX = event->x();
        m_DiffY = event->y();

        move = 1;

    } else {
        move = 0;
    }
}


void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (move){

        QPoint p = event->globalPos();

        window()->move(p.x() - m_DiffX, p.y() - m_DiffY);
    }

}


