#ifndef VIEW_H
#define VIEW_H


#include "myglwidget.h"
#include "webcamwindow.h"
#include "chronometre.h"
#include "score.h"
#include <QKeyEvent>
#include <QPushButton>
#include <QTimer>
#include <fstream>
#include <iostream>

using namespace std;


class View: public QWidget
{
    Q_OBJECT

private :

    double *ciblePosition;
    double *spherePosition;
    double *randomSpherePosition;
    double x, y;
    bool joue;
    bool jeuFini;
    int numSphere;
    int niveau;
    int sec;
    int milisec;
    int record;
    double tailleCible;



    MyGLWidget* oGLArea;
    WebCamWindow* oCamWidget;
    Chronometre* lvlChrono;
    Chronometre* sphereChrono;
    Score* score;

    QVBoxLayout* oVertLayout;
    QHBoxLayout* nomLayout;
    QHBoxLayout* bottomLayout;
    QHBoxLayout* oLayout;

    QPushButton * replayButton;
    QPushButton * enregistrerButton;
    QPushButton * quitButton;
    QPushButton * scoreButton;



    QTimer* timer;
    QTimer* chrono;
    QLabel* lvlTime;
    QLabel* sphereTime;
    QLabel* lvl;
    QLabel* nbrSphere;
    QLabel* nom;
    QLineEdit* zoneNom;
    QString name;



public:
    View(QWidget *parent = 0);
    ~View();
    void saveTime(int niveau, string chrono);
    void saveName(int niveau, string name);
    string getChrono(int niveau);
    string getNom(int niveau);


public slots:
    void applyGL();
    void startTime();
    void saveNom();
    void showScore();
    void replayGame();


};

#endif // VIEW_H
