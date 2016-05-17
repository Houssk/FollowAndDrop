#include <QtWidgets>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include "webcamwindow.h"
#include <QDebug>

using namespace cv;

WebCamWindow::WebCamWindow(QWidget *parent)
    : QWidget(parent)
{
    seuil = 10;
    Perdu = 0;
    frameWidth_=424;
    frameHeight_=240;
    templateWidth_=50;
    templateHeight_=50;
    label = new QLabel(tr("Image"));
    trackCheckBox= new QCheckBox(tr("Jouer!"));


    sensitivity = new QLabel();
    sensitivity->setText("Veuillez choisir la sensibilité de la caméra");
    liste = new QComboBox();
    liste->addItem("10");
    liste->addItem("20");
    liste->addItem("30");
    liste->addItem("40");
    liste->addItem("50");

    optionLayout = new QHBoxLayout();
    optionLayout->addWidget(sensitivity);
    optionLayout->addWidget(liste);
    optionLayout->setAlignment(Qt::AlignCenter);

    QVBoxLayout *vl1=new QVBoxLayout;
    vl1->addWidget(trackCheckBox);


    vl1->setAlignment(Qt::AlignCenter);
    QHBoxLayout *hl=new QHBoxLayout;
    hl->addLayout(vl1);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addLayout(hl);
    layout->addLayout(optionLayout);

    setLayout(layout);
    setWindowTitle(tr("Track WebCam"));

    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(aquire()));
    startWebCam();

}

/*---------------------------------------------------------------------------*/

WebCamWindow::~WebCamWindow()
{
    timer->stop();
    delete webcam;
}

/*---------------------------------------------------------------------------*/

void WebCamWindow::displayImage()
{
    if (imageTraitee.data!=NULL)
    {
        cv::flip(imageTraitee,imageTraitee,1); // process the image
        // change color channel ordering
        cv::cvtColor(imageTraitee,imageTraitee,CV_BGR2RGB);
        // Qt image
        QImage img= QImage((const unsigned char*)(imageTraitee.data),
                           imageTraitee.cols,imageTraitee.rows,QImage::Format_RGB888);
        // display on label
        label->setPixmap(QPixmap::fromImage(img));
        // resize the label to fit the image
        const QPixmap* qMap=label->pixmap();
        label->resize(qMap->size());
    }
    else QMessageBox(QMessageBox::Critical,tr("Error"),tr("Image is void")).exec();
}

/*---------------------------------------------------------------------------*/


void WebCamWindow::aquire()
{
    if(webcam->isOpened())
    {
        if (webcam->read(imageTraitee))
        {
    if ( !trackCheckBox->isChecked()) detectHand();
    if (trackCheckBox->isChecked()) trackHand();
    displayImage();
        }
    }

}

/*---------------------------------------------------------------------------*/

void WebCamWindow::startWebCam()
{
    if (!timer->isActive())
    {
        webcam= new cv::VideoCapture(0);
        webcam->set(CV_CAP_PROP_FRAME_WIDTH,frameWidth_);
        webcam->set(CV_CAP_PROP_FRAME_HEIGHT,frameHeight_);
        timer->start(20);

    }
    else
    {
        timer->stop();
        delete webcam;
        webcam=0;
    }
}

/*---------------------------------------------------------------------------*/

void WebCamWindow::detectHand()
{
    rectRoi = cv::Rect(180,90,50,50);
    cv::Rect rectRoi2(182,92,46,46);
    rectRoiRech = cv::Rect(155,65,100,100);
    posrectRoiRech = cv::Point(155,65);
    rectangle(imageTraitee,rectRoi,Scalar( 0, 255, 0),2,8,0);
    rectangle(imageTraitee,rectRoiRech,Scalar( 0, 255, 0),2,8,0);
    imageTraitee(rectRoi2).copyTo(templ);
}

/*---------------------------------------------------------------------------*/

void WebCamWindow::Template(Point P, double Val)
{

    if(Val*100 > 90)
    {
        cv::Rect rectRoi2(P.x+posrectRoiRech.x,P.y+posrectRoiRech.y,46,46);
        imageTraitee(rectRoi2).copyTo(templ);
    }
    if(Val*100 < 73)
    {

        Perdu++;
        // seuil de flexibilité
        seuil = atoi(liste->currentText().toStdString().c_str());
        if(Perdu > seuil)
        {
            trackCheckBox->setChecked(false);

        }
    }
    else
    {
        Perdu = 0;
    }
}

/*---------------------------------------------------------------------------*/

void WebCamWindow::trackHand()
{
    /// Load image and template
    imageTraitee(rectRoiRech).copyTo(img);

    MatchingMethod( 0, 0 );
    return ;
}

/*---------------------------------------------------------------------------*/

void WebCamWindow::MatchingMethod( int, void* )
{
    /// Source image to display
    Mat img_display;
    img.copyTo( img_display );
    /// Create the result matrix
    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;


    result.create( result_cols, result_rows, CV_32FC1 );
    /// Do the Matching and Normalize
    matchTemplate( img, templ, result, CV_TM_CCOEFF_NORMED );


    /// Localizing the best match with minMaxLoc
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;

    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

    /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    matchLoc = maxLoc;

    /// Show me what you got
    rectangle(imageTraitee,rectRoiRech,Scalar(0,255,0), 2, 8, 0 );
    rectangle( imageTraitee, Point(matchLoc.x+posrectRoiRech.x-2,matchLoc.y+posrectRoiRech.y-2), Point( matchLoc.x +posrectRoiRech.x
                                                                                                        + templ.cols+2 , matchLoc.y +posrectRoiRech.y+ templ.rows+2 ),Scalar::all(0), 2, 8, 0 );
    /// Rendu du mvt du palet
    rectangle( imageTraitee, Point(matchLoc.x+posrectRoiRech.x-2,matchLoc.y+posrectRoiRech.y-2),
               Point( matchLoc.x +posrectRoiRech.x+ templ.cols+2 , matchLoc.y +posrectRoiRech.y+ templ.rows+2),Scalar(255,56,2), 2, 8, 0 );

    circle(result, matchLoc, 10, Scalar(0,0,255),-1);

    Template(matchLoc, maxVal);
    NewRoiRech(matchLoc);

    return;
}

/*---------------------------------------------------------------------------*/

// On calcule la nouvelle position du rectangle de recherche
void WebCamWindow::NewRoiRech(cv::Point P)
{
    // Le centre du rectangle est en (27,27)
    int x = P.x - 27;
    int y = P.y - 27;
    posrectRoiRech.x = posrectRoiRech.x + x;
    if (posrectRoiRech.x <=0)
        posrectRoiRech.x = 0;
    else if (posrectRoiRech.x>imageTraitee.cols-rectRoiRech.width)
        posrectRoiRech.x = imageTraitee.cols-rectRoiRech.width;

    posrectRoiRech.y = posrectRoiRech.y + y;
    if (posrectRoiRech.y <=0)
        posrectRoiRech.y = 0;
    else if (posrectRoiRech.y>imageTraitee.rows-rectRoiRech.height)
        posrectRoiRech.y = imageTraitee.rows-rectRoiRech.height;
    rectRoiRech = cv::Rect(posrectRoiRech.x,posrectRoiRech.y,100,100);

}

double* WebCamWindow::GetPosition()
{
    double *position = new double[2];
    position[0] = posrectRoiRech.x;
    position[1] = posrectRoiRech.y;
    return position;

}

bool WebCamWindow::IsChecked()
{
   return trackCheckBox->isChecked();
}

void WebCamWindow::setCheckedFalse()
{
    trackCheckBox->setChecked(false);
}







