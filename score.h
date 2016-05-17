#ifndef SCORE_H
#define SCORE_H

#include <QWidget>
#include <fstream>
#include <QtWidgets>
#include <iostream>
#include <QString>
#include <QDebug>


using namespace std;



class Score : public QWidget
{
    Q_OBJECT
public:
    explicit Score(QWidget *parent = 0);
    ~Score();

private:
    QHBoxLayout* titlesLayout, * score1Layout,* score2Layout,* score3Layout, * buttonLayout;
    QVBoxLayout* scoreLayout;
    string lvl,time,name;
    QLabel *LvlTitle, *NameTitle, *TimeTitle, *Lvl, *Time, *Name,*Lvl1, *Time1, *Name1,*Lvl2, *Time2, *Name2,*Lvl3, *Time3, *Name3;
    QPushButton *quitButton;


signals:

public slots:
};

#endif // SCORE_H
