#ifndef CHRONOMETRE_H
#define CHRONOMETRE_H


#include <QTimer>
#include "myglwidget.h"
#include "webcamwindow.h"

/*
Cette classe permet d'afficher le chronomètre et de gérer toutes les fonctionnalités realtives à celui-ci
Fait par :
*/

// La classe hérite de Qwidget pour son affichage.
class Chronometre : public QWidget
{

    Q_OBJECT

public:
    // Constructeur de la classe où toutes les variables sont initialisées et que l'affichage est géré
    Chronometre(QWidget *parent = 0);

    // Destructeur de la classe
    ~Chronometre();

    /* Méthode qui permet de récupérer le chronomètre
    Paramètre de retour : QString */
    QString toString();

private:
    int i; // Pas d'incrémentation du Timer pour chaque coup d'horloge
    QLCDNumber *num; // Afficher le chrono sous le format LCD
    QTime *temps; // Ce qui est affiché dans LCD
    QTimer* chrono; // Le Timer qui agit sur l'actualisation du chrono
    QVBoxLayout* layout; // Le layout où le chronomètre est placé;
    QString text; // Le chronomètre sous format QString exploitable par les autres classes


public slots :
    void showTime(); // Affiche le chronomètre à chaque coup d'horloge
    void resetTime(); // Réinitialise le temps du chronomètre
    void startTime(); // Lance le chronomètre
    void stopTime(); // Arrête le chronomètre
};

#endif // CHRONOMETRE_H
