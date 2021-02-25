#include "mythread.h"

#include <QDebug>

#include "mainwindow.h"

MyThread::MyThread(QObject *parent, bool b, QString path, int passe) :
    QThread(parent), m_stop(b), m_path(path), m_passe(passe)
{
    connect(this, SIGNAL(sendValue(bool)), parent, SLOT(receiveValue(bool)));
}

void EcritureDansFichier(std::string PathFichier, std::string EcritureFichier){

    std::ofstream fichier(PathFichier.c_str(), std::ios::out);

    if(fichier) {
        fichier << EcritureFichier;
        fichier.close();
    }

}

std::string hexToAscii(std::string Str){

    unsigned int y;
    std::string asciiValue;

    for (unsigned int i = 0; i < (Str.size())/2; i++){

        std::istringstream iss(Str);
        iss.flags(std::ios::hex);
        iss >> y;

        asciiValue += (char)y;

    }

    return asciiValue;
}

std::string fichierTostring(std::string fichier){

    std::ifstream file(fichier.c_str());
    std::string contenu;

    if(file) {

        std::ostringstream contenant;
        contenant << file.rdbuf();
        contenu = contenant.str();
    }

    return contenu;
}


std::string intToHex(int t)
{
    std::ostringstream oss;
    oss << std::hex << t;
    return oss.str();
}

void MyThread::run()
{
    qDebug() << "on thread";

    QFile file(m_path);
    file.open(QIODevice::ReadOnly);

    QByteArray data = file.readAll();

    srand((unsigned int)time(0));

    for (int y = 0; y < m_passe && !m_stop; y++){

        std::string nouvelleValeur;

        for(int i= 0 ; i < data.size() && !m_stop; i++)
        {
            int rnd = (rand() % 255) + 32;
            nouvelleValeur += hexToAscii(intToHex(rnd));
        }

        file.close();

//        qDebug() << "wait";
//        QThread::sleep(5);

        if (!m_stop){

            EcritureDansFichier(m_path.toUtf8().constData(), nouvelleValeur);
        }

        nouvelleValeur = "";

    }

    value(1);
}


void MyThread::value(bool value)
{
    emit sendValue(value);
}



