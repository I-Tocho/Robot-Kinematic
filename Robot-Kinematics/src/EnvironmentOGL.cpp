#include "EnvironmentOGL.h"


EnvironmentOGL::EnvironmentOGL() {}

void EnvironmentOGL::initializer(float rcamera )
{

    Rcamera = rcamera;
    phiCamera = PI/3.0;
    thetaCamera = PI/4.0;
    camerafactor = 0.005;

    glClearColor(1,1,1,1);
    glEnable( GL_LINE_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_COLOR_MATERIAL );

}


EnvironmentOGL::~EnvironmentOGL() {}

void EnvironmentOGL::render()
{
    radians =  double(PI*(angle-90.0f)/180.0f);

    cameraX=Rcamera*cos(phiCamera)*cos(thetaCamera);
    cameraY=Rcamera*cos(phiCamera)*sin(thetaCamera);
    cameraZ=Rcamera*sin(phiCamera);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT );
    glLoadIdentity();
    gluLookAt(cameraX,cameraY,cameraZ, 0, 0,0, 0.0, 0,fabs(cos(thetaCamera))*sin(phiCamera+0.1));

    drawarrow3D({0,0,0}, {50,0,0}, redColor,1,0.5);
    drawarrow3D({0,0,0}, {0,50,0}, greenColor,1,0.5);
    drawarrow3D({0,0,0}, {0,0,50}, blueColor,1,0.5);
}

void EnvironmentOGL::resize(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(52.0f,(GLdouble)width/(GLdouble)height,1.0f,1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void EnvironmentOGL::drawarrow3D( vector3d A,  vector3d B, vector3d color, double cota1,double R)
{
    double color1,color2,color3;
    color1=color.x;
    color2=color.y;
    color3=color.z;
    glColor4f( color1,color2, color3, 1);

    vector3d n=B-A,np,vertex[10],normallight;
    n.normalize();
    if(n.z!=0)
        np= {1,1,(-1/n.z)*(n.x+n.y)};
    else if(n.y!=0)
        np= {1,(-1/n.y)*(n.x+n.z),1};
    else
        np= {(-1/n.x)*(n.y+n.z),1,1};

    np.normalize();
    vertex[0]=R*np;
    vertex[2]=R*(n*np).normalize();
    vertex[1]=R*((0.5)*(vertex[2]-vertex[0])+vertex[0]).normalize();
    vertex[4]=R*(n*vertex[2]).normalize();
    vertex[3]=R*((0.5)*(vertex[4]-vertex[2])+vertex[2]).normalize();
    vertex[6]=R*(n*vertex[4]).normalize();
    vertex[5]=R*((0.5)*(vertex[6]-vertex[4])+vertex[4]).normalize();
    vertex[7]=R*((0.5)*(vertex[0]-vertex[6])+vertex[6]).normalize();
    vertex[8]=vertex[0];
    vertex[9]=vertex[1];

    double fraccion=0.1,radioflecha=R+.7*R;

    glBegin( GL_TRIANGLE_STRIP );

    for(int i=0; i<9; i++)
    {
        normallight=n*(vertex[i-1]-vertex[i+1]);
        normallight.normalize();
        glNormal3f(normallight.x, normallight.y, normallight.z);
        glVertex3f(vertex[i].x+A.x,vertex[i].y+A.y,vertex[i].z+A.z);
        glVertex3f(vertex[i].x+B.x-fraccion*(B.x-A.x),vertex[i].y+B.y-fraccion*(B.y-A.y),vertex[i].z+B.z-fraccion*(B.z-A.z));
    }

    glEnd();

    vertex[0]=radioflecha*np;
    vertex[2]=radioflecha*(n*np).normalize();
    vertex[1]=radioflecha*((0.5)*(vertex[2]-vertex[0])+vertex[0]).normalize();
    vertex[4]=radioflecha*(n*vertex[2]).normalize();
    vertex[3]=radioflecha*((0.5)*(vertex[4]-vertex[2])+vertex[2]).normalize();
    vertex[6]=radioflecha*(n*vertex[4]).normalize();
    vertex[5]=radioflecha*((0.5)*(vertex[6]-vertex[4])+vertex[4]).normalize();
    vertex[7]=radioflecha*((0.5)*(vertex[0]-vertex[6])+vertex[6]).normalize();
    vertex[8]=vertex[0];
    vertex[9]=vertex[1];
    vector3d Ap(B-fraccion*(B-A));

    glBegin( GL_TRIANGLE_STRIP );  //flecha

    for(int i=0; i<9; i++)
    {
        normallight=n*(vertex[i-1]-vertex[i+1]);
        normallight.normalize();
        glNormal3f(normallight.x, normallight.y, normallight.z);
        glVertex3f(vertex[i].x+Ap.x,vertex[i].y+Ap.y,vertex[i].z+Ap.z);


        glNormal3f(n.x, n.y, n.z);
        glVertex3f(Ap.x+fraccion*(B-A).x,Ap.y+fraccion*(B-A).y,Ap.z+fraccion*(B-A).z);
    }

    glEnd();


}

void EnvironmentOGL::drawarrow3Dmodel(const object3D &model)
{

    drawarrow3D(model.Origin,model.Origin+80*model.ux,redColor,0.03,1);
    drawarrow3D(model.Origin,model.Origin+80*model.uy,greenColor,0.03,1);
    drawarrow3D(model.Origin,model.Origin+80*model.uz,blueColor,0.03,1);

}


