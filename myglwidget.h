#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <QKeyEvent>
#include <GL/glu.h>
#include <QMessageBox>
#include <QApplication>
#include <qmath.h>
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
Cette classe contient tout le contenu graphique du jeu
Fait par :
*/

// La classe hérite de QGLWidget pour gérer l'affichage 3D se basant sur la bibliothèque OpenGL
class MyGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    // Constructeur de la classe où le timer et les variables de dessin sont initialisées
    explicit MyGLWidget(QWidget *parent = 0);

    // Destructeur de la classe
    ~MyGLWidget();

protected:

    /* Définit la vue OpenGL et la matrice de projection.
    Est appelé à chaque fois que le widget est redimensionné ou est affiché pour la première fois
    Paramètre d'entrée : w = largeur et h = hauteur */
    void resizeGL(int w, int h);

    /* Définit le contexte de rendu OpenGL.
    Est appelé une seule fois avant que resizeGL() ou paintGL() ne soit appelé. */
    void initializeGL();

    /* Affiche la scène OpenGL.
    Est appelé à chaque coup d'horloge du timer */
    void paintGL();

public:

    void arene();     // Dessin du cylindre semi-transparent qui borde l'arène

    /* Dessin du plateau de l'arène avec un trou d'évacuation
    Paramètres en entrée :
    dis = la distance du trou par rapport au centre; r = le rayon du trou; rext = le rayon extérieur du plateau;
    rint = le rayon intérieur; angext et angint = les angles d'ouverture de chaque arc de cercle;
    on, nbsupl, nbext et nbint = le nombre de vertex dessinés; z = la hauteur du plateau */
    void trouDeuxArcsCentres (GLfloat dis, GLfloat r, GLfloat rext, GLfloat rint,GLfloat angext, GLfloat angint, GLint on,GLint nbsupl, GLint nbext, GLint nbint,GLfloat z);

    void loadWoodTexture();                                             // Chargement de la texture en bois pour le plateau
    void loadLogoTexture();                                             // Chargement de la texture du logo TSE
    void loadTargetTexture();                                           // Chargement de la texture sur le disque cible

    /* Dessin du bras Robot
    Paramètre en entrée : L'angle d'ouverture des pinces du bras robot */
    void brasRobot(double P);

    /* Dessin d'une sphère
    Paramètres en entrée : nombre de vertex à dessiner et le rayon */
    void sphere(int lats, int longs);

    /* Dessin d'un cube
    Paramètres en entrée : longueur, largeur et hauteur */
    void cube(double X, double Y,double hauteur);

    /* Dessin d'un cercle utilisé pour le plateau troué
    Paramètres en entrée : rayon et nombre de vertex */
    void draw_circle(const GLfloat radius,const GLuint num_vertex);

    /* Dessin du disque cible
    Paramètres en entrée : rayon et nombre de vertex */
    void draw_cible(const GLfloat radius,const GLuint num_vertex);

    /* Animation pour jeter la sphère dans le trou d'évacuation
    Paramètre en entrée : la vitesse d'animation */
    void jeterObjet(double v);

    /* Animation pour orienter le bras robot vers la sphère
    Paramètres en entrée : les coordonnées de la sphère et la vitesse d'animation */
    void orienterObjet(double x, double y, double v);

    /* Animation pour orienter le bras robot vers le trou d'évacuation
    Paramètre en entrée : la vitesse d'animation */
    void orienterTrou(double v);

    /* Animation pour recupérer la sphère par le bras robot
    Paramètres en entrée : les coordonnées de la sphère et la vitesse d'animation */
    void chercherObjet(double x, double y,double v);

    void animerRobot();                                 // Animation du bras robot pour indiquer le changement de niveau

    /* Animation pour faire rouler la sphère
    Paramètres en entrée : les coordonnées où doit se diriger la sphère  */
    void RoulerSphere(double x, double y, double z);

    /* Placer la cible sur le plateau
    Paramètres en entrée : les coordonnées où la cible sera placée */
    void PlacerCible(double x , double y);


    void ChangeStatusTrue();            // Indique que la sphère a été évacuée
    void ChangeStatusFalse();           // Indique que la sphère n'a pas été évacuée

    /* Change la taille du disque cible
    Paramètre en entrée : le rayon du disque */
    void ChangeCibleSize(double Size);

    /* Génère deux nombres aléatoirement pour placer le disque cible à ces coordonnées
    Paramètre de sortie : un pointeur sur un tableau contenant deux nombres aléatoires */
    double *NombreAleatoire();

    /* Génère deux nombres aléatoirement pour placer la sphère à ces coordonnées
    Paramètre de sortie : un pointeur sur un tableau contenant deux nombres aléatoires */
    double *SphereAleatoire();

    void GenerateCible();           // Dessiner le disque cible à un endroit aléatoire
    void GenerateSphere();          // Dessiner la sphère à un endroit aléatoire
    void ChuteSphere();             // Animation pour la chute de la sphère dans le trou d'évacuation

    /*  Surchage de la fonction d'opengl
    Paramètre de sortie : la taille de la zone de dessin */
    QSize sizeHint() const;

private:

    bool get , status ;                                 // Informations sur l'état de la sphère dans l'environnement de dessin
    GLuint m_texture[1], l_texture[1], t_texture[1];    // Les textures
    GLUquadric * cylindre, * balle;                     // Dessin des doigts du robot, du socle et du coude
    QTimer* timer;                                      // Le timer actualise l'affichage de la scène
    double Sz, Sx, Cz, Cx, S, C, R, T, W, *nombre, x, y, a , b, Taille; // Le rôle de ces variable est décrit au .cpp
    #define PI 3.1415926535898                          // Approximation grossière de PI
    #define Cos(th) cos(PI/180*(th))                    // Calcul direct du cosinus en degrès
    #define Sin(th) sin(PI/180*(th))                    // Calcul direct du sinus en degrès
};

#endif // MYGLWIDGET_H
