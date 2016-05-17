#include "myglwidget.h"
#include <QTimer>


MyGLWidget::MyGLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    showNormal();
    // Initialisation du timer
    timer = new QTimer(this);
    timer->start(50);

    // Initialisation des variables
    Sz = 0, Sx = 0, Cz = 0, Cx = 0, S=0, C=0, R=0, T=0, W=-0.95, a=0 , b=0, Taille = 0.35;
    get=false , status = false;
}


MyGLWidget::~MyGLWidget()
{
    glDeleteTextures(1, &m_texture[0]);
}

void MyGLWidget::resizeGL(int w, int h)
{
    glViewport (0, 0, (GLsizei)(w), (GLsizei)(h));                          // Reset le Viewport en cours
    glMatrixMode (GL_PROJECTION);                                           // Choix du mode projection
    glLoadIdentity ();                                                      // Réinitialise la matrice précédente
    gluPerspective (45.0f, (GLfloat)(w)/((GLfloat)(h)*1.5), 1.0f, 100.0f);  // Calcul des dimensions de la fenêtre
    glMatrixMode (GL_MODELVIEW);                                            // Choix du mode Modelview
    glLoadIdentity ();                                                      //Réinitialise la matrice précédente
}

void MyGLWidget::initializeGL()
{
    loadWoodTexture();
    loadLogoTexture();
    loadTargetTexture();
    glClearColor (0.5, 0.5, 0.5, 0.5);                              // Couleur du fond
    glClearDepth (1.0f);											// Réglage du Depth Buffer
    glDepthFunc (GL_LEQUAL);										// The Type Of Depth Testing (Less Or Equal)
    glEnable (GL_DEPTH_TEST);										// Enable Depth Testing
    glShadeModel (GL_FLAT);											// Select Flat Shading (Nice Definition Of Objects)
    glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);				// Set Perspective Calculations To Most Accurate
    glEnable(GL_LIGHT0);											// Enable Default Light
    glEnable(GL_COLOR_MATERIAL);									// Enable Color Material
}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Clear Screen And Depth Buffer
    glLoadIdentity();												// Réinitialise le Modelview Matrix actuel
    glTranslatef(0.0f,0.0f,-6.0f);									// Translation pour se placer dans le cadre de la scène


    glPushMatrix();

        glScalef(1.5,1.5,1.5);
        glRotatef(120,0,1,0);
        glRotatef(60,0,0,1);
        glRotatef(-20,1,0,0);
        glTranslatef(-0.15,0.5,0);
        glPushMatrix();
            glTranslatef(0,-0.75,0);
            arene();
        glPopMatrix();


        glPushMatrix();
            brasRobot(-30);
        glPopMatrix();

        glPushMatrix();
            if(get==false)
            GenerateSphere();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,-1,0);
            glRotatef(90,1,0,0);
            glScalef(0.0665,0.0665,0.0665);
            trouDeuxArcsCentres (12, 1.5, 15, 7,M_PI,M_PI,1,4,150,0,0);
        glPopMatrix();

        glPushMatrix();
            GenerateCible();
        glPopMatrix();

    glPopMatrix();


}


void MyGLWidget::arene()
{

    // On active le blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glBegin(GL_QUAD_STRIP);
    for (int j=0;j<=360;j+=5)
    {
        glColor4f(1.0,0.8,0.4,0.2);
        glVertex3f(Cos(j),+0.25,Sin(j));
        glColor4f(1,0.8,0.4,0.2);
        glVertex3f(Cos(j),-0.25,Sin(j));
    }
    glEnd();
    glDepthMask(GL_TRUE);
}


void MyGLWidget::draw_circle(const GLfloat radius,const GLuint num_vertex)
{
  GLfloat vertex[4];
  GLfloat texcoord[2];

  const GLfloat delta_angle = 360/num_vertex;

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, l_texture[0]);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
  glBegin(GL_TRIANGLE_FAN);

  //draw the vertex at the center of the circle
  texcoord[0] = 0.5;
  texcoord[1] = 0.5;
  glTexCoord2fv(texcoord);

  vertex[0] = vertex[1] = vertex[2] = 0.0;
  vertex[3] = 1.0;
  glVertex4fv(vertex);

  for(GLuint i = 0; i < num_vertex ; i++)
  {
    texcoord[0] = (Cos(delta_angle*i) + 1.0)*0.5;
    texcoord[1] = (Sin(delta_angle*i) + 1.0)*0.5;
    glTexCoord2fv(texcoord);

    vertex[0] = Cos(delta_angle*i) * radius;
    vertex[1] = Sin(delta_angle*i) * radius;
    vertex[2] = 0.0;
    vertex[3] = 1.0;
    glVertex4fv(vertex);
  }

  texcoord[0] = (1.0 + 1.0)*0.5;
  texcoord[1] = (0.0 + 1.0)*0.5;
  glTexCoord2fv(texcoord);

  vertex[0] = 1.0 * radius;
  vertex[1] = 0.0 * radius;
  vertex[2] = 0.0;
  vertex[3] = 1.0;
  glVertex4fv(vertex);
  glEnd();

  glDisable(GL_TEXTURE_2D);

}

void MyGLWidget::draw_cible(const GLfloat radius,const GLuint num_vertex)
{
  GLfloat vertex[4];
  GLfloat texcoord[2];

  const GLfloat delta_angle = 360/num_vertex;
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, t_texture[0]);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
  glBegin(GL_TRIANGLE_FAN);

  //draw the vertex at the center of the circle
  texcoord[0] = 0.5;
  texcoord[1] = 0.5;
  glTexCoord2fv(texcoord);

  vertex[0] = vertex[1] = vertex[2] = 0.0;
  vertex[3] = 1.0;
  glColor4f(1.0,1.0,1.0,0.5);
  glVertex4fv(vertex);

  for(GLuint i = 0; i < num_vertex ; i++)
  {
    texcoord[0] = (Cos(delta_angle*i) + 1.0)*0.5;
    texcoord[1] = (Sin(delta_angle*i) + 1.0)*0.5;
    glTexCoord2fv(texcoord);

    vertex[0] = Cos(delta_angle*i) * radius;
    vertex[1] = Sin(delta_angle*i) * radius;
    vertex[2] = 0.0;
    vertex[3] = 1.0;
    glColor4f(1.0,1.0,1.0,0.5);
    glVertex4fv(vertex);
  }

  texcoord[0] = (1.0 + 1.0)*0.5;
  texcoord[1] = (0.0 + 1.0)*0.5;
  glTexCoord2fv(texcoord);

  vertex[0] = 1.0 * radius;
  vertex[1] = 0.0 * radius;
  vertex[2] = 0.0;
  vertex[3] = 1.0;
  glColor4f(1.0,1.0,1.0,0.5);
  glVertex4fv(vertex);
  glEnd();

  glDisable(GL_TEXTURE_2D);


}

void MyGLWidget::trouDeuxArcsCentres (GLfloat dis, GLfloat r,GLfloat rext, GLfloat rint,GLfloat angext, GLfloat angint, GLint on,GLint nbsupl, GLint nbext, GLint nbint,GLfloat z) {
  GLfloat alpha, dalpha, alphaext, dalphaext, alphaint, dalphaint;
  GLfloat xext, yext, xint, yint, dx, dy;
  GLint i;
  GLint  nb = nbsupl * 2 + nbext + nbint + 2;
  dalpha = 2.0F * M_PI / nb;
  dalphaext = angext * 2.0F / nbext;
  dalphaint = angint * 2.0F / nbint;
  alpha = (M_PI - nbext * dalpha) * 0.5F;
  alphaext = M_PI * 0.5F - angext;
  alphaint = M_PI * 0.5F + angint;


  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_texture[0]);
  glBegin (GL_QUAD_STRIP);
  glNormal3f (0.0F, 0.0F, on);
  glColor4f(1.0, 1.0, 1.0, 1.0); // reset gl color
  for (i = 0; i <= nbext; ++i)
  {
    glTexCoord2f(r * cos (alpha)/15, (r * sin (alpha) + dis)/15);
    glVertex3f (r * cos (alpha), r * sin (alpha) + dis, z);
    xext = rext * cos (alphaext);
    yext = rext * sin (alphaext);
    glTexCoord2f(xext/15,yext/15);
    glVertex3f (xext, yext, z);
    alpha += dalpha;
    alphaext += dalphaext;
  }
  xint = rint * cos (alphaint);
  yint = rint * sin (alphaint);
  dx = (xint - xext) / (nbsupl + 1);
  dy = (yint - yext) / (nbsupl + 1);
  for (i = 1; i <= nbsupl; ++i)
  {
    glTexCoord2f(r * cos (alpha)/15,(r * sin (alpha) + dis)/15);
    glVertex3f (r * cos (alpha), r * sin (alpha) + dis, z);
    glTexCoord2f((xext + dx * i)/15,(yext + dy * i)/15);
    glVertex3f (xext + dx * i, yext + dy * i, z);
    alpha += dalpha;
  }
  for (i = 0; i <= nbint; ++i)
  {
    glTexCoord2f(r * cos (alpha)/15,(r * sin (alpha) + dis)/15);
    glVertex3f (r * cos (alpha), r * sin (alpha) + dis, z);
    xint = rint * cos (alphaint);
    yint = rint * sin (alphaint);
    glTexCoord2f(xint/15,yint/15);
    glVertex3f (xint, yint, z);
    alpha += dalpha;
    alphaint -= dalphaint;
  }
  for (i = 1; i <= nbsupl; ++i) {
    glTexCoord2f(r * cos (alpha)/15,(r * sin (alpha) + dis)/15);
    glVertex3f (r * cos (alpha), r * sin (alpha) + dis, z);
    glTexCoord2f((xint + dx * i)/15,(yint - dy * i)/15);
    glVertex3f (xint + dx * i, yint - dy * i, z);
    alpha += dalpha;
  }
  alphaext = M_PI * 0.5F - angext;
  glTexCoord2f(r * cos (alpha)/15,r * sin (alpha)/15);
  glVertex3f (r * cos (alpha), r * sin (alpha) + dis, z);
  glTexCoord2f(rext * cos (alphaext)/15,rext * sin (alphaext)/15);
  glVertex3f (rext * cos (alphaext), rext * sin (alphaext), z);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPushMatrix();
  glTranslatef(0.0f,0,-0.05);
  glScalef(0.5,0.5,0.5);
  draw_circle(10,50);
  glPopMatrix();

}

void MyGLWidget::loadWoodTexture()
{

    QImage image(":/image/Wood.bmp");
    image = image.convertToFormat(QImage::Format_RGB888);
    image = image.mirrored();
    glGenTextures(1, &m_texture[0]);
    glBindTexture(GL_TEXTURE_2D, m_texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 image.width(), image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 image.bits());

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

void MyGLWidget::loadLogoTexture()
{

    QImage image(":/image/TSE.bmp");
    image = image.convertToFormat(QImage::Format_RGB888);
    image = image.mirrored();
    glGenTextures(1, &l_texture[0]);
    glBindTexture(GL_TEXTURE_2D, l_texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 image.width(), image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 image.bits());

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

void MyGLWidget::loadTargetTexture()
{

    QImage image(":/image/target.bmp");
    image = image.convertToFormat(QImage::Format_RGB888);
    image = image.mirrored();
    glGenTextures(1, &t_texture[0]);
    glBindTexture(GL_TEXTURE_2D, t_texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 image.width(), image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 image.bits());

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}


void MyGLWidget::sphere(int lats, int longs) {
    int i, j;
    for(i = 0; i <= lats; i++) {
      double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
      double z0  = sin(lat0);
      double zr0 =  cos(lat0);

      double lat1 = M_PI * (-0.5 + (double) i / lats);
      double z1 = sin(lat1);
      double zr1 = cos(lat1);

      glBegin(GL_QUAD_STRIP);
      for(j = 0; j <= longs; j++) {
        double lng = 2 * M_PI * (double) (j - 1) / longs;
        double x = cos(lng);
        double y = sin(lng);
        glColor3f(0,0,1);
        glNormal3f(x * zr0, y * zr0, z0);
        glVertex3f(x * zr0, y * zr0, z0);
        glNormal3f(x * zr1, y * zr1, z1);
        glVertex3f(x * zr1, y * zr1, z1);
      }
      glEnd();
    }
  }


void MyGLWidget::cube(double X, double Y,double hauteur)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, l_texture[0]);
    glBegin(GL_POLYGON);
    glColor4f(1.0, 1.0, 1.0, 1.0); // reset gl color
    glTexCoord2f(1, 0);
    glVertex3f( X, -Y, hauteur );
    glTexCoord2f(0, 0);
    glVertex3f( X, Y, hauteur );
    glTexCoord2f(0, 1);
    glVertex3f( -X, Y, hauteur );
    glTexCoord2f(1, 1);
    glVertex3f( -X, -Y, hauteur );
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, l_texture[0]);
    glBegin(GL_POLYGON);
    glColor4f(1.0, 1.0, 1.0, 1.0); // reset gl color
    glTexCoord2f(1, 0);
    glVertex3f( X, -Y, -hauteur );
    glTexCoord2f(0, 0);
    glVertex3f( X, Y,- hauteur );
    glTexCoord2f(0, 1);
    glVertex3f( -X, Y,- hauteur );
    glTexCoord2f(1, 1);
    glVertex3f( -X, -Y,- hauteur );
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, l_texture[0]);
    glBegin(GL_POLYGON);
    glColor4f(1.0, 1.0, 1.0, 1.0); // reset gl color
    glTexCoord2f(1, 0);
    glVertex3f(X, -Y, -hauteur );
    glTexCoord2f(0, 0);
    glVertex3f(X, Y, -hauteur );
    glTexCoord2f(0, 1);
    glVertex3f(X, Y,  hauteur );
    glTexCoord2f(1, 1);
    glVertex3f(X, -Y,  hauteur );
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, l_texture[0]);
    glBegin(GL_POLYGON);
    glColor4f(1.0, 1.0, 1.0, 1.0); // reset gl color
    glTexCoord2f(0, 1);
    glVertex3f( -X, -Y,  hauteur );
    glTexCoord2f(1, 1);
    glVertex3f( -X, Y,  hauteur );
    glTexCoord2f(1, 0);
    glVertex3f( -X, Y, -hauteur );
    glTexCoord2f(0, 0);
    glVertex3f( -X, -Y, -hauteur );
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, l_texture[0]);
    glBegin(GL_POLYGON);
    glColor4f(1.0, 1.0, 1.0, 1.0); // reset gl color
    glTexCoord2f(1, 1);
    glVertex3f( X, Y,  hauteur );
    glTexCoord2f(1, 0);
    glVertex3f( X, Y, -hauteur );
    glTexCoord2f(0, 0);
    glVertex3f( -X, Y, -hauteur );
    glTexCoord2f(0, 1);
    glVertex3f( -X, Y,  hauteur );
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, l_texture[0]);
    glBegin(GL_POLYGON);
    glColor4f(1.0, 1.0, 1.0, 1.0); // reset gl color
    glTexCoord2f(0, 0);
    glVertex3f( X, -Y, -hauteur );
    glTexCoord2f(0, 1);
    glVertex3f( X, -Y,  hauteur );
    glTexCoord2f(1, 1);
    glVertex3f( -X, -Y,  hauteur );
    glTexCoord2f(1, 0);
    glVertex3f( -X, -Y, -hauteur );
    glEnd();
    glDisable(GL_TEXTURE_2D);


}


void MyGLWidget::brasRobot(double P) // S= Rotation Socle C= Rotation du coude P= pincement des doigts à 20 pinces fermées
{   glPushMatrix();
    glScalef(0.5,0.5,0.5);
    glTranslatef(0,-1,0);
        // Sphère 1
        glPushMatrix();
            glTranslatef(0,-0.87,0);
            glRotatef(-90,1,0,0);
            glRotatef(Sz,0,0,1); // Rotation suivant Z
            glRotatef(Sx,1,0,0); // Rotation suivant X
            glRotatef(S,1,0,0); // Rotation suivant X
            //glRotatef(90,1,0,0);
            glScalef(0.125,0.125,0.125);
            sphere(100,100);
            // Cube 1
            glPushMatrix();
                glTranslatef(0,0,3.5);
                cube(0.6,0.6,2.5);
                // Sphère 2
                glPushMatrix();
                    glTranslatef(0,0,3.5);
                    //glRotatef(Cz,0,0,1); // Rotation suivant Z
                    glRotatef(Cx,1,0,0); // Rotation suivant X
                    glRotatef(C,1,0,0); // Rotation suivant X
                    //glRotatef(0,1,0,0);// 180 deg /partie inférieure
                    sphere(100,100);
                    // Cube 2
                    glPushMatrix();
                        glTranslatef(0,0,3.5);
                        cube(0.6,0.6,2.5);
                        // Sphère 3
                        glPushMatrix();
                            glTranslatef(0,0,3.5);
                            glRotatef(Cz,0,0,1); // Rotation suivant Z
                            sphere(100,100);
                            //Doigt 1 à gauche
                            glPushMatrix();
                                cylindre = gluNewQuadric ();
                                glRotatef(-180,0,1,0);
                                glTranslatef(0,1,-1.7);
                                glRotatef(-P,1,0,0); //Contrôle des deux doigts
                                glColor3f(1,1,1);
                                gluCylinder(cylindre,0.08,0.08,0.7,100,100);
                                //Doigt 2 à gauche
                                glPushMatrix();
                                    cylindre = gluNewQuadric ();
                                    glTranslatef(0,-0.1,-0.8);
                                    glRotatef(-5,1,0,0);
                                    glColor3f(1,1,0);
                                    gluCylinder(cylindre,0.08,0.08,0.7,100,100);
                                glPopMatrix();
                            glPopMatrix();
                            //Doigt 1 à droite
                            glPushMatrix();
                                cylindre = gluNewQuadric ();
                                glRotatef(-180,0,1,0);
                                glTranslatef(0,-1,-1.7);
                                glRotatef(P,1,0,0); //Contrôle des deux doigts
                                glColor3f(1,1,1);
                                gluCylinder(cylindre,0.08,0.08,0.7,100,100);
                                //Doigt 2 à droite
                                glPushMatrix();
                                    cylindre = gluNewQuadric ();
                                    glTranslatef(0,0.1,-0.8);
                                    glRotatef(5,1,0,0);
                                    glColor3f(1,1,0);
                                    gluCylinder(cylindre,0.08,0.08,0.7,100,100);
                                glPopMatrix();
                                glPopMatrix();
                                // Balle
                                glPushMatrix();
                                balle = gluNewQuadric();
                                glTranslatef(0,0,2);
                                glColor3f(1,1,1);
                                if(get==true)
                                gluSphere(balle,1,100,100);
                                glPopMatrix();
                            glPopMatrix();
                        glPopMatrix();
                    glPopMatrix();
            glPopMatrix();
        glPopMatrix();
     glPopMatrix();
}

void MyGLWidget::jeterObjet(double v)
{
    while(Sx<45)
     {
         Sx=Sx+v;
         Cx=Cx+v;
         updateGL();
     }
     while(Cx<75)
     {
         Cx=Cx+v;
         updateGL();
     }
     while(Sx>0)
     {
         get=false;
         Sx=Sx-v;
         Cx=Cx-v;
         R = 0.8;
         T = 0;
         W = W -0.01;
         updateGL();
     }
     while(Cx>0)
     {
         get = false;
         Cx=Cx-v;
         updateGL();
     }
     timer->stop();
}

void MyGLWidget::orienterObjet(double x, double y, double v)
{
    double angle = 0; // L'angle par rapport à Z où se trouve la sphère
    angle = atan(y/x);
    angle = angle*180/PI;

    if(angle<0)
    {
    while(Sz>angle)
     {
         Sz=Sz-v;
         Cz=Cz-v;
         updateGL();

     }
    while(Cz>90)
    {
        Cz = Cz - v;
        updateGL();
    }
    }
    else
    {
        while(Sz<angle)
         {
             Sz=Sz+v;
             Cz=Cz+v;
             updateGL();
         }
        while(Cz<90)
        {
            Cz = Cz + v;
            updateGL();
        }
    }

     timer->stop();
}


void MyGLWidget::orienterTrou(double v)
{
    double angle = 0; // L'angle par rapport à Z où se trouve la sphère
    if(Sz<0)
    {
    while(Sz<angle)
     {
         Sz=Sz+v;
         Cz=Cz+v;
         updateGL();
     }
    while(Cz<90)
    {
        Cz = Cz + v;
        updateGL();
    }
    }
    else
    {
        while(Sz>angle)
         {
             Sz=Sz-v;
             Cz=Cz-v;
             updateGL();
         }
        while(Cz>90)
        {
            Cz = Cz - v;
            updateGL();
        }
    }

     timer->stop();
}

void MyGLWidget::chercherObjet(double x, double y, double v)
{

    double q1=0; // Angle entre 1ère sphère et l'horizontale
    double q2=0; // Angle entre 2ème sphère et q1
    double a = 0.5; // Demi-Hauteur du bras robot
    double b = 0.6; // Partie supérieure
    double c;
    c=sqrt(x*x+y*y);
    q1 = acos((a*a+c*c-b*b)/(2*a*c))*180/PI;
    q2 = acos((a*a+b*b-c*c)/(2*a*b))*180/PI;
    q1=90-q1;
    q2=180-q2;

    if(x>=0)
    {
        while(S<q1)
             {
                 S=S+v;
                 updateGL();
             }
        while(C<q2)
             {
                 C=C+v;
                 //C=q2;
                 updateGL();
             }
        while(C>0)
             {
                get=true;
                 C=C-v;
                 updateGL();
             }
        while(S>0)
             {
                 get=true;
                 S=S-v;
                 updateGL();
             }
        timer->stop();
    }
    else
    {
        q1=-q1;
        q2=-q2;
        while(S>q1 && C>q2)
             {
                  S=S-v;
                  C=C-4*v;
                 updateGL();
             }
        while(C>q2)
             {
                 C=C-v;
                 updateGL();
             }
        while(C<0)
             {
                get=true;
                 C=C+v;
                 updateGL();
             }
        while(S<0)
             {
                get=true;
                 S=S+v;
                 updateGL();
             }
        timer->stop();
    }
}


void MyGLWidget::animerRobot()
{
    S=0, C=0, Sz=0;
    while(S<30)
    {
        S=S+1;
        C=C+2;
        updateGL();
    }

    while(Sz<360)
    {
        Sz = Sz+5;
        updateGL();
    }
    while(S>0)
    {
        S=S-1;
        C=C-2;
        updateGL();
    }
    Sz = 0;
    timer->stop();
}

void MyGLWidget::RoulerSphere(double x, double y, double z)
{
    R = x;
    T = y;
    W = z;
}

double *MyGLWidget::NombreAleatoire()
{
    double *nb = new double [2];
    /* initialize random seed: */
    srand (time(NULL));

    /* generate secret number between 1 and 10: */
    y = rand() % 10 + 1;
    y = y/10;
    x = rand() % 10 + 1;
    x = x/10;
    while(sqrt(x*x+y*y)>=0.8 ||sqrt(x*x+y*y)<=0.5 || (y==0 && x==0.8) || (y<=0.2 && y>=-0.2 && x>0))
    {
        y = rand()  % 19 + (-9);
        y = y/10;
        x = rand()  % 19 + (-9);
        x = x/10;
    }
    nb[0]=x;
    nb[1]=y;
    return nb;
}

double *MyGLWidget::SphereAleatoire()
{
    double *nb = new double [2];
    /* initialize random seed: */
    srand (time(NULL));

    /* generate secret number between 1 and 10: */
    nb[1] = rand() % 10 + 1;
    nb[1] = nb[1]/10;
    nb[0] = rand() % 10 + 1;
    nb[0] = nb[0]/10;
    while(sqrt(nb[0]*nb[0]+nb[1]*nb[1])>=0.8 ||sqrt(nb[0]*nb[0]+nb[1]*nb[1])<=0.5 || (nb[1]==0 && nb[0]==0.8) || (nb[1]<=0.2 && nb[1]>=-0.2 && nb[0]>0))
    {
        nb[1] = rand()  % 19 + (-9);
        nb[1] = nb[1]/10;
        nb[0] = rand()  % 19 + (-9);
        nb[0] = nb[0]/10;
    }
    return nb;
}

void MyGLWidget::PlacerCible(double x , double y)
{
    if(status)
    {
        a=x;
        b=y;
    }
}

void MyGLWidget::ChangeStatusTrue()
{
    status = true;
}

void MyGLWidget::ChangeStatusFalse()
{
    status = false;
}

void MyGLWidget::ChangeCibleSize(double Size)
{
    Taille = Size;
}

void MyGLWidget::GenerateCible()
{

    glRotatef(90,1,0,0);
    glTranslatef(b,a,0.99); // Y,X,Z
    glScalef(0.5,0.5,0.5);
    draw_cible(Taille,50);
}

void MyGLWidget::GenerateSphere()
{
    balle = gluNewQuadric();
    glTranslatef(T,W,R);//Y,Z,X
    gluSphere(balle,0.05,100,100);
}

void MyGLWidget::ChuteSphere()
{
    double z = -0.95;
    while(z> -1.4)
    {
        z = z - 0.005;
        RoulerSphere(0.8,0,z);
        updateGL();
    }
}


//surchage de la fonction d'opengl, permet de définir la taille de la zone de dessin
QSize MyGLWidget::sizeHint() const
{
    return QSize(800, 800);
}

