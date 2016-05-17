#include "score.h"


Score::Score(QWidget *parent) : QWidget(parent)
{

    QString path=QString("%1/records.txt").arg(QDir::homePath());
    ifstream is(path.toStdString().c_str());

    quitButton = new QPushButton("Fermer");
    connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));

    // Pour les titres
    titlesLayout = new QHBoxLayout();
    LvlTitle = new QLabel();
    TimeTitle = new QLabel();
    NameTitle = new QLabel();

    LvlTitle->setText("Niveau");
    TimeTitle->setText("Temps");
    NameTitle->setText("  Nom");


    titlesLayout->addWidget(LvlTitle);
    titlesLayout->addWidget(NameTitle);
    titlesLayout->addWidget(TimeTitle);

    // Pour le premier niveau
    getline(is,lvl,';');
    getline(is,name,';');
    getline(is,time,';');

    lvl = lvl.substr(9);
    name = name.substr(6);
    time = time.substr(8);

    score1Layout = new QHBoxLayout();
    Lvl1 = new QLabel();
    Time1 = new QLabel();
    Name1 = new QLabel();

    Lvl1->setText(QString::fromStdString(" "+lvl));
    Time1->setText(QString::fromStdString(time));
    Name1->setText(QString::fromStdString(name));


    score1Layout->addWidget(Lvl1);
    score1Layout->addWidget(Name1);
    score1Layout->addWidget(Time1);

    // Pour le deuxième niveau

    getline(is,lvl,';');
    getline(is,name,';');
    getline(is,time,';');


    lvl = lvl.substr(9);
    name = name.substr(6);
    time = time.substr(8);

    score2Layout = new QHBoxLayout();
    Lvl2 = new QLabel();
    Time2 = new QLabel();
    Name2 = new QLabel();

    Lvl2->setText(QString::fromStdString(lvl));
    Time2->setText(QString::fromStdString(time));
    Name2->setText(QString::fromStdString(name));


    score2Layout->addWidget(Lvl2);
    score2Layout->addWidget(Name2);
    score2Layout->addWidget(Time2);

    // Pour le troisième niveau


    getline(is,lvl,';');
    getline(is,name,';');
    getline(is,time,';');


    lvl = lvl.substr(9);
    name = name.substr(6);
    time = time.substr(8);

    score3Layout = new QHBoxLayout();
    Lvl3 = new QLabel();
    Time3 = new QLabel();
    Name3 = new QLabel();

    Lvl3->setText(QString::fromStdString(lvl));
    Time3->setText(QString::fromStdString(time));
    Name3->setText(QString::fromStdString(name));


    score3Layout->addWidget(Lvl3);
    score3Layout->addWidget(Name3);
    score3Layout->addWidget(Time3);

    buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(quitButton);
    buttonLayout->setAlignment(Qt::AlignCenter);

    scoreLayout = new QVBoxLayout();
    scoreLayout->addLayout(titlesLayout);
    scoreLayout->addLayout(score1Layout);
    scoreLayout->addLayout(score2Layout);
    scoreLayout->addLayout(score3Layout);
    scoreLayout->addLayout(buttonLayout);



    setLayout(scoreLayout);
    resize(400,200);


}

Score::~Score()
{

}

