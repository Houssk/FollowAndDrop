#include "chronometre.h"

Chronometre::Chronometre(QWidget *parent) :
    QWidget(parent)
{
    num = new QLCDNumber(this);
    num->setDigitCount(9);

    temps = new QTime();
    temps->setHMS(0,0,0,0);

    chrono = new QTimer(this);
    connect(chrono, SIGNAL(timeout()), this, SLOT(showTime()));

    i=0;
    QString text = temps->toString("mm:ss:zzz");
    num->display(text);
    num->setSegmentStyle(QLCDNumber::Flat);

    layout = new QVBoxLayout(this);
    layout->addWidget(num);
}

Chronometre::~Chronometre()
{

}

void Chronometre::resetTime()
{
    temps->setHMS(0,0,0);
    QString text = temps->toString("mm:ss:zzz");
    num->display(text);
    i=0;
    stopTime();
}

void Chronometre::startTime()
{
    chrono->start(1);
}

void Chronometre::stopTime()
{
    chrono->stop();
}


void Chronometre::showTime()
{
    QTime newtime;
    i=i+10;
    newtime=temps->addMSecs(i);
    text = newtime.toString("mm:ss:zzz");
    num->display(text);
}

QString Chronometre::toString()
{
    return text;
}

