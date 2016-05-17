#ifndef WEBCAMWINDOW_H
#define WEBCAMWINDOW_H

#include <QtWidgets>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>

//Classe qui gère l'intéraction avec la Webcam en fonction du mouvement de l'utilisateur.

class WebCamWindow : public QWidget
{
    Q_OBJECT
    
public:
    WebCamWindow(QWidget *parent = 0);
    ~WebCamWindow();
    void                displayImage();
    void                detectHand();
    void                trackHand(); //Fonction pour le tracking de la main.
    void                MatchingMethod( int, void* );
    void                Template(cv::Point P, double Val);//Fonction qui sert à détecter la perte de l'intéraction.
    void                NewRoiRech(cv::Point P);//Calcule la nouvelle position du rectangle de recherche.
    double*             GetPosition();
    bool                IsChecked();
    void                setCheckedFalse();



    cv::Rect            rectRoi;
    cv::Rect            rectRoiRech; //Rectangle de recherche.
    cv::Point           posrectRoiRech; //Point qui contient les coordonnées du rectangle de recherche.
                                       //On s'en sert au niveau de CView pour récupérer la coordonnées Y et
                                      //la transmettre comme nouvelle position pour le palet.
    QCheckBox           *trackCheckBox; //CheckBox multifonction. (initialisation, lancement du jeu, reprise, replay)
    QTimer              *timer; //Timer = 20

public slots:
    void aquire();
    void startWebCam();




private:
    cv::Mat             img;
    cv::Mat             templ;
    cv::Mat             result;
    cv::Mat             imgRoi_;
    cv::Mat             image_;

    int frameWidth_ ;
    int frameHeight_;
    int templateWidth_;
    int templateHeight_;
    int                 match_method;
    int                 Perdu; //Paramètre pour détecter la perte de l'intéraction.
    int seuil;

    QPushButton         *webCamButton;

    QComboBox *liste;
    QHBoxLayout *optionLayout;


    QLabel              *label;
    QLabel              *sensitivity;
    cv::Mat             imgCam;
    cv::Mat             imageTraitee;
    cv::VideoCapture    *webcam;
    };

#endif // WEBCAMWINDOW_H
