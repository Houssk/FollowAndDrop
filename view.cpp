#include "view.h"

using namespace std;

//constructeur de la View, qui met en place l'interface graphique
View::View(QWidget *parent) : QWidget(parent)
{

    QString path=QString("%1/records.txt").arg(QDir::homePath());
    ifstream is(path.toStdString().c_str());
    if(!is)
    {
        ofstream os(path.toStdString().c_str());
        string s = "Niveau : 1; Nom : Alpha; Temps : 59:59:999; Niveau : 2; Nom : Beta; Temps : 59:59:999; Niveau : 3; Nom : Gamma; Temps : 59:59:999;" ;
        os << s;
        os.close();
    }

    // Initialisation des variables
    x = 0,y = 0;
    joue = false;
    jeuFini = false;
    numSphere = 3;
    niveau = 1;

    // Initialisation des objets graphiques
    oGLArea = new MyGLWidget();
    oCamWidget = new WebCamWindow();
    lvlChrono = new Chronometre();
    lvlTime = new QLabel();
    sphereChrono = new Chronometre();
    sphereTime = new QLabel();
    lvl = new QLabel();
    nbrSphere = new QLabel();
    nom = new QLabel();
    zoneNom = new QLineEdit();
    replayButton = new QPushButton();
    quitButton = new QPushButton();
    enregistrerButton = new QPushButton();
    scoreButton = new QPushButton();

    record = 0;
    tailleCible = 0.35;
    lvlTime->setText("Votre temps pour ce niveau :");
    sphereTime->setText("Votre temps pour la sphère courante :");
    QString nbrstr = QString("Il vous reste %1 sphère(s) à placer").arg(numSphere);
    nbrSphere->setText(nbrstr);

    replayButton->setText("Recommencer");
    enregistrerButton->setText("Enregistrer");
    scoreButton->setText("Afficher les records");
    quitButton->setText("Quitter le jeu");

    QString lvlstr = QString("Vous êtes au niveau %1").arg(niveau);
    lvl->setText(lvlstr);

    nom->setText("Veuillez entrer votre nom pour jouer");

    nomLayout = new QHBoxLayout();
    nomLayout->addWidget(nom);
    nomLayout->addWidget(zoneNom);
    nomLayout->addWidget(enregistrerButton);


    bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(scoreButton);
    bottomLayout->addWidget(replayButton);
    bottomLayout->addWidget(quitButton);



    //création du layout vertical de droite
    oVertLayout = new QVBoxLayout();
    oVertLayout->addLayout(nomLayout);
    oVertLayout->addWidget(oCamWidget);
    oVertLayout->addWidget(lvlTime);
    oVertLayout->addWidget(lvlChrono);
    oVertLayout->addWidget(sphereTime);
    oVertLayout->addWidget(sphereChrono);
    oVertLayout->addWidget(lvl);
    oVertLayout->addWidget(nbrSphere);
    oVertLayout->addLayout(bottomLayout);
    oVertLayout->setAlignment(Qt::AlignTop);


    //création du layout principal
    oLayout = new QHBoxLayout();
    oLayout->addWidget(oGLArea);
    oLayout->addLayout(oVertLayout);



    //définition du layout principal
    setLayout(oLayout);

    //définition du titre de la fenêtre
    setWindowTitle(tr("Follow&Drop"));

    //initialisation du timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(applyGL()));
    timer->start(50);

    chrono = new QTimer(this);
    connect(chrono, SIGNAL(timeout()), this, SLOT(startTime()));
    chrono->start(1000);

    connect(quitButton,SIGNAL(clicked()),qApp,SLOT(quit()));
    connect(enregistrerButton,SIGNAL(clicked()),this,SLOT(saveNom()));
    connect(scoreButton,SIGNAL(clicked()),this,SLOT(showScore()));
    connect(replayButton,SIGNAL(clicked()),this,SLOT(replayGame()));




    oGLArea->ChangeStatusTrue();
    ciblePosition = oGLArea->NombreAleatoire();

}

View::~View()
{

}

void View::showScore()
{
    score = new Score();
    score->show();
}

void View::replayGame()
{
    timer->start(50);
    chrono->start(1000);
    tailleCible = 0.35;
    niveau = 1;
    numSphere = 3;
    lvlChrono->resetTime();
    sphereChrono->resetTime();
    QString nbrstr = QString("Il vous reste %1 sphère(s) à placer").arg(numSphere);
    nbrSphere->setText(nbrstr);
    QString lvlstr = QString("Vous êtes au niveau %1").arg(niveau);
    lvl->setText(lvlstr);
    oCamWidget->setCheckedFalse();
    oGLArea->ChangeStatusTrue();
    ciblePosition = oGLArea->NombreAleatoire();
    jeuFini = false;

}

void View::saveName(int niveau, string name)
{

    QString path=QString("%1/records.txt").arg(QDir::homePath());
    QString pathtemp=QString("%1/recordstemp.txt").arg(QDir::homePath());

    string nom;
    nom = getNom(niveau);
    nom = nom.substr(7).c_str();
    string strReplace = nom+";";
    string strNew = name+";";
    ifstream filein(path.toStdString().c_str()); //File to read from
    ofstream fileout(pathtemp.toStdString().c_str()); //Temporary file


    string strTemp;
    bool test = true;
    while(filein >> strTemp)
    {
       if(strTemp == strReplace && test){
           strTemp = strNew;
           test = false;
       }
       strTemp += " ";
       fileout << strTemp;
    }
    filein.close();
    fileout.close();
    remove(path.toStdString().c_str());
    rename(pathtemp.toStdString().c_str(),path.toStdString().c_str());
}

void View::saveTime(int niveau , string chrono)
{

    QString path=QString("%1/records.txt").arg(QDir::homePath());
    QString pathtemp=QString("%1/recordstemp.txt").arg(QDir::homePath());

    string time;
    time = getChrono(niveau);
    time = time.substr(9,9).c_str();
    string strReplace = time+";";
    string strNew = chrono+";";
    qDebug()<<strReplace.c_str();
    qDebug()<<strNew.c_str();
    ifstream filein(path.toStdString().c_str()); //File to read from
    ofstream fileout(pathtemp.toStdString().c_str()); //Temporary file


    string strTemp;
    bool test = true;
    while(filein >> strTemp)
    {
       if(strTemp == strReplace && test){
           strTemp = strNew;
           test = false;

       }

       strTemp += " ";
       fileout << strTemp;
    }
    filein.close();
    fileout.close();
    remove(path.toStdString().c_str());
    rename(pathtemp.toStdString().c_str(),path.toStdString().c_str());
}

string View::getChrono(int niveau)
{
    string lvl,time,name;
    QString path=QString("%1/records.txt").arg(QDir::homePath());
    ifstream is(path.toStdString().c_str());
    for(int i = 0;i<niveau;i++)
    {
        getline(is,lvl,';');
        getline(is,name,';');
        getline(is,time,';');
    }

    return time.c_str();
}


string View::getNom(int niveau)
{
    string lvl,time,name;
    QString path=QString("%1/records.txt").arg(QDir::homePath());
    ifstream is(path.toStdString().c_str());
    for(int i = 0;i<niveau;i++)
    {
        getline(is,lvl,';');
        getline(is,name,';');
        getline(is,time,';');
    }
    return name.c_str();
}


void View::saveNom()
{
    name = zoneNom->text();
    if(name != "")
    {
        QLayoutItem *item;
        while ((item = nomLayout->takeAt(1)) != NULL)
        {
            delete item->widget();
            delete item;
        }

        QString str = QString("Bienvenue %1").arg(name);
        nom->setText(str);
    }

}

void View::startTime()
{
    if(joue)
    {
        lvlChrono->startTime();
        sphereChrono->startTime();
    }
}



//fonction qui est appelée à chaque coup d'horloge
void View::applyGL()
{
    if(oCamWidget->IsChecked() && jeuFini == false && name != "")
    {

        oGLArea->PlacerCible(ciblePosition[0],ciblePosition[1]);
        joue = true;
        double c,d; // Pour éviter que la sphere ne sorte de l'arene
        oGLArea->ChangeStatusFalse();
        oGLArea->ChangeCibleSize(tailleCible);
        spherePosition = oCamWidget->GetPosition();
        c = -0.006*spherePosition[0] + 1;
        d = -0.014*spherePosition[1] + 1;
        if(sqrt(c*c+d*d)<1)
        {
            x=c;
            y=d;
        }
        double a , b ;

        a = ( (int)(floor( fabs( x ) * 10 ) ) ) % 10;
        b = ( (int)(floor( fabs( y ) * 10 ) ) ) % 10;

        if(x<0)
        {
            a = -a/10;
        }
        else
        {
            a = a/10;
        }
        if(y<0)
        {
            b = -b/10;
        }
        else
        {
            b = b/10;
        }

        if ((a==0.8 && b==0))
        {
            oGLArea->ChuteSphere();
            randomSpherePosition = oGLArea->SphereAleatoire();
            x = randomSpherePosition[0];
            y = randomSpherePosition[1];
        }

        if(a != ciblePosition[0] || b != ciblePosition[1])
        {
            oGLArea->RoulerSphere(x,y,-0.95);
            oGLArea->updateGL();
        }
        else
        {
            oGLArea->ChangeStatusTrue();
            oGLArea->orienterObjet(ciblePosition[0],ciblePosition[1],1);
            oGLArea->chercherObjet(ciblePosition[0],ciblePosition[1],1);
            oGLArea->orienterTrou(1);
            oGLArea->jeterObjet(1);
            sphereChrono->resetTime();
            if(numSphere != 0)
            {
                numSphere--;
                QString str = QString("Il vous reste %1 sphère(s) à placer").arg(numSphere);
                nbrSphere->setText(str);
                if(numSphere!=0)
                {
                    ciblePosition = oGLArea->NombreAleatoire();
                    oGLArea->PlacerCible(ciblePosition[0],ciblePosition[1]);
                }
            }
        }
    }
    else
        if(joue)
        {
            lvlChrono->stopTime();
            sphereChrono->stopTime();
        }
    if(numSphere == 0 && niveau != 3)
    {
        oGLArea->animerRobot();
        tailleCible = tailleCible - 0.1;
        ciblePosition = oGLArea->NombreAleatoire();
        oGLArea->PlacerCible(ciblePosition[0],ciblePosition[1]);
        numSphere = 3;
        string temp;
        temp = getChrono(niveau);
        sec = atoi(temp.substr(12,2).c_str());
        milisec = atoi(temp.substr(15,3).c_str());
        int sec1,milisec1;

        string temp1;
        temp1 = lvlChrono->toString().toStdString();
        sec1 = atoi(temp1.substr(3,2).c_str());
        milisec1 = atoi(temp1.substr(6,3).c_str());



        if(sec1<sec)
        {
            saveName(niveau,name.toStdString().c_str());
            saveTime(niveau,lvlChrono->toString().toStdString().c_str());
            record ++;

        }

        else
        {
            if(sec1 == sec && milisec1<milisec)
            {
                saveName(niveau,name.toStdString().c_str());
                saveTime(niveau,lvlChrono->toString().toStdString().c_str());
                record ++;
            }

            else
            {
                qDebug()<<"pas de record";
            }
        }

        niveau ++;
        lvlChrono->resetTime();
        QString lvlstr = QString("Vous êtes au niveau %1").arg(niveau);
        lvl->setText(lvlstr);
        QString str = QString("Il vous reste %1 sphère(s) à placer").arg(numSphere);
        nbrSphere->setText(str);
    }
    if(numSphere == 0 && niveau == 3)
    {
        string temp;
        temp = getChrono(niveau);
        sec = atoi(temp.substr(12,2).c_str());
        milisec = atoi(temp.substr(15,3).c_str());
        int sec1,milisec1;

        string temp1;
        temp1 = lvlChrono->toString().toStdString();
        sec1 = atoi(temp1.substr(3,2).c_str());
        milisec1 = atoi(temp1.substr(6,3).c_str());


        if(sec1<sec)
        {
            saveName(niveau,name.toStdString().c_str());
            saveTime(niveau,lvlChrono->toString().toStdString().c_str());
            record ++;
        }

        else
        {
            if(sec1 == sec && milisec1<milisec)
            {
                saveName(niveau,name.toStdString().c_str());
                saveTime(niveau,lvlChrono->toString().toStdString().c_str());
                record ++;
            }
        }
        lvl->setText("Niveaux terminés");
        nbrSphere->setText("Jeu terminé");
        lvlChrono->stopTime();
        sphereChrono->stopTime();
        oCamWidget->setCheckedFalse();
        jeuFini = true;
        chrono->stop();
        timer->stop();
        if(record !=0)
        {
            QMessageBox msgBox;
            msgBox.setText("Jeu terminé ! Vous avez battu un nouveau record :D !!");
            msgBox.exec();
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Jeu terminé ! Pas de record battu :( ");
            msgBox.exec();
        }
        record = 0;
    }


}
