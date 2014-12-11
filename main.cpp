#define GL_GLEXT_PROTOTYPES

#include <GL/glut.h>
#include "initglui.h"

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include "object.h"
#include "plan13.h"
#include "sunpos.h"

#include <sys/time.h>

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

using namespace std;

// From initglui.h {
  Glui2* GluiHandle = NULL;
  g2Button* QuitButton = NULL;
  g2ProgressBar* satCamAz = NULL;
  g2ProgressBar* satCamEl = NULL;
  g2RadioGroup* MorA = NULL;
  g2Label* manualConfig = NULL;
  g2Label* spinAzTitle = NULL;
  g2Spinner* spinAz = NULL;
  g2Label* spinElTitle = NULL;
  g2Spinner* spinEl = NULL;
  g2Label* simulationConfig = NULL;
  g2Label* spinYearTitle = NULL;
  g2Spinner* spinYear = NULL;
  g2Label* spinMonthTitle = NULL;
  g2Spinner* spinMonth = NULL;
  g2Label* spinDayTitle = NULL;
  g2Spinner* spinDay = NULL;
  g2Label* spinHourTitle = NULL;
  g2Spinner* spinHour = NULL;
  g2Label* spinMinuteTitle = NULL;
  g2Spinner* spinMinute = NULL;
  g2Label* spinSecondTitle = NULL;
  g2Spinner* spinSecond = NULL;
  g2Label* spinMultTitle = NULL;
  g2Spinner* spinMult = NULL;
  g2Button* setTimeButton = NULL;
  g2Button* runButton = NULL;
// }

//plan13
time_t rawtime;
time_t firsttime;
double ms,nms;
Plan13 p13;
struct timeval tp;
struct timeval ntp;
float curTime, nextTime;

struct tm * ptm;
double startTime = 0;
double startClockTime = 0;
double elapsedClockTime = 0;
double elapsedTotalTime = 0;
int timerStarted = 0;

int satElevNum = 200;

double sunRad= 100;
//variables
// XZ position of the camera
float x=0.0, z=0.0, y = 3.0f;

// XZ position of the camera
float lx=sin(-M_PI/2),lz=-cos(-M_PI/2), ly = 0.0f;

//angle of rotation for the camera direction
float angle = -M_PI/2;
// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = 0;

// variables to compute frames per second
int frame;
long time1, timebase1;
char s[50];

// variables to hold window identifiers
int mainWindow, subWindow1,subWindow2,subWindow3;
//border between subwindows
int border = 6;

// width and height of the window
int width = 730+2*border;
int height = (width/16)*9;

int base, arm, dish, plane, earth;

float azimuth = 45;
float elevation = 45;

void drawSnowMan() {
  glColor3f(1.0f, 1.0f, 1.0f);

  // Draw Body
  glTranslatef(0.0f ,0.75f, 0.0f);
  glutSolidSphere(0.75f,20,20);

  // Draw Head
  glTranslatef(0.0f, 1.0f, 0.0f);
  glutSolidSphere(0.25f,20,20);

  // Draw Eyes
  glPushMatrix();
  glColor3f(0.0f,0.0f,0.0f);
  glTranslatef(0.05f, 0.10f, 0.18f);
  glutSolidSphere(0.05f,10,10);
  glTranslatef(-0.1f, 0.0f, 0.0f);
  glutSolidSphere(0.05f,10,10);
  glPopMatrix();

  // Draw Nose
  glColor3f(1.0f, 0.5f, 0.5f);
  glRotatef(0.0f,1.0f, 0.0f, 0.0f);
  glutSolidCone(0.08f,0.5f,10,2);

  glColor3f(1.0f, 1.0f, 1.0f);
}

void renderBitmapString(
  float x,
  float y,
  float z,
  void *font,
  char *string)
{

  char *c;
  glRasterPos3f(x, y,z);
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}

void restorePerspectiveProjection()
{
  glMatrixMode(GL_PROJECTION);
  // restore previous projection matrix
  glPopMatrix();

  // get back to modelview mode
  glMatrixMode(GL_MODELVIEW);
}

void setOrthographicProjection()
{
  // switch to projection mode
  glMatrixMode(GL_PROJECTION);

  // save previous matrix which contains the
  //settings for the perspective projection
  glPushMatrix();

  // reset matrix
  glLoadIdentity();

  // set a 2D orthographic projection
  gluOrtho2D(0, width, height, 0);

  // switch back to modelview mode
  glMatrixMode(GL_MODELVIEW);
}

void computePos(float deltaMove)
{
  x += deltaMove * lx * 0.1f;
  z += deltaMove * lz * 0.1f;
}

// Common Render Items for all subwindows
void renderScene2()
{
  // Draw ground
  glColor3f(0.3f, 0.6f, 0.3f);
  glBegin(GL_QUADS);
    glVertex3f(-500.0f, -1.0f, -500.0f);
    glVertex3f(-500.0f, -1.0f,  500.0f);
    glVertex3f( 500.0f, -1.0f,  500.0f);
    glVertex3f( 500.0f, -1.0f, -500.0f);
  glEnd();

  struct cLocation myLocation;
  myLocation.dLongitude = -105.25185183;
  myLocation.dLatitude = 40.01076298;
  struct cTime theTime;
  theTime.iYear = spinYear->GetInt();
  theTime.iMonth = spinMonth->GetInt();
  theTime.iDay = spinDay->GetInt();
  theTime.dHours = spinHour->GetInt();
  theTime.dMinutes = spinMinute->GetInt();
  theTime.dSeconds = spinSecond->GetInt();

  struct cSunCoordinates currentSunCoord;

  sunpos(theTime, myLocation, &currentSunCoord);

  float sunMulti = 0.7;
  float RGBA[4] = {0.6,0.7,1.0,1};  //  Colors
  if (currentSunCoord.dZenithAngle >= 90){
    RGBA[0] = 1;  //  Colors
    RGBA[2] = 0.6;
  }

  double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
  int zh = fmod(90*t,360.0);
  float Emission[]  = {sunMulti, sunMulti, sunMulti,1.0};
  float Ambient[]  = {0.5, 0.5, 0.5,1.0};
  float Diffuse[]   = {0.8,0.8,0.8,1.0};
  float Specular[]  = {0.8,0.8,0.8,1.0};
  float Position[]  = {sunRad*Sin(currentSunCoord.dZenithAngle)*Cos(currentSunCoord.dAzimuth), sunRad*Sin(currentSunCoord.dZenithAngle)*Sin(currentSunCoord.dAzimuth), sunRad*Cos(currentSunCoord.dZenithAngle), 1, 0};
  float Shinyness[] = {16};

  glEnable(GL_FOG);
  float fogIntense = (fabs(180-fabs((float)currentSunCoord.dZenithAngle))/180);
  float FogCol[3]={fogIntense, fogIntense, fogIntense}; // Define a nice light grey
  glFogfv(GL_FOG_COLOR,FogCol);     // Set the fog color
  // linear fog
  //changed vim format options form: croql
  glFogi(GL_FOG_MODE, GL_LINEAR); // Note the 'i' after glFog - the GL_LINEAR constant is an integer.
  glFogf(GL_FOG_START, 50.0f);
  glFogf(GL_FOG_END, 100.0f);
  // exponential fog
  //glFogi(GL_FOG_MODE, GL_EXP2);
  //glFogf(GL_FOG_DENSITY, 0.005f);

  //  Draw light position as sphere (still no lighting here)
  glColor3f(1,0.7,0.6);
  glPushMatrix();
  glTranslated(Position[1],Position[0],Position[2]);
  //glRotatef(90,0,0,1);
  glutSolidSphere(3,10,10);

  //  OpenGL should normalize normal vectors
  glEnable(GL_NORMALIZE);
  //  Enable lighting
  glEnable(GL_LIGHTING);
  //  Enable light 0
  glEnable(GL_LIGHT0);
  //  Set ambient, diffuse, specular components and position of light 0
  glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
  glLightfv(GL_LIGHT0,GL_POSITION,Position);
  //  Set materials
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,RGBA);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,RGBA);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
  glPopMatrix();

  glPushMatrix();
  glScalef(100,100,100);
  glRotatef(180,0,1,0);
  glCallList(plane);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, 1.18593f, 0);
  glCallList(base);
  glTranslatef(0, -1.18593f, 0);
  glPopMatrix();

  glPushMatrix();
  glRotatef(azimuth,0,1,0);
  glRotatef(-90,0,1,0);
  glTranslatef(0, 3.27869f, 0);
  glCallList(arm);
  glTranslatef(0, -3.27869f, 0);
  glRotatef(90,0,1,0);
  glRotatef(-azimuth,0,1,0);
  glPopMatrix();

  glPushMatrix();
  glRotatef(azimuth,0,1,0);
  glTranslatef(0, 3.89526f, 0);
  glRotatef(-90,0,1,0);
  glRotatef(90-elevation,0,0,1);
  glCallList(dish);
  glRotatef(-90+elevation,0,0,1);
  glRotatef(90,0,1,0);
  glTranslatef(0, -3.89526f, 0);
  glRotatef(-azimuth,0,1,0);
  glPopMatrix();
}

// Display func for main window
void renderScene()
{
  glutSetWindow(mainWindow);
  glClear(GL_COLOR_BUFFER_BIT);
  glutSwapBuffers();
  glutPostRedisplay();
}

void calcTimePassed(long* elapsedTime, float* elapsedMils)
{
  struct timeval tp;
  gettimeofday(&tp, NULL);

  double newElapClockTime = (tp.tv_sec * 1000 + tp.tv_usec / 1000) - startClockTime;

  double diffClockTime = (double)spinMult->GetFloat()*(newElapClockTime-elapsedClockTime);

  elapsedTotalTime += diffClockTime;

  float tempMils = fmod(elapsedTotalTime,1000);
  *elapsedMils = tempMils/1000.f;
  *elapsedTime = elapsedTotalTime/1000.f;
  elapsedClockTime = newElapClockTime;
}

void startTimer(g2Controller* Caller)
{
  if (timerStarted == 0)
  {
    struct tm t = {0};  // Initalize to all 0's
    t.tm_year = spinYear->GetInt()-1900;  // This is year-1900, so 112 = 2012
    t.tm_mon = spinMonth->GetInt()-1;
    t.tm_mday = spinDay->GetInt();
    t.tm_hour = spinHour->GetInt();
    t.tm_min = spinMinute->GetInt();
    t.tm_sec = spinSecond->GetInt();
    time_t timeSinceEpoch = mktime(&t);

    startTime = (double)timeSinceEpoch;
    elapsedTotalTime = (startTime)*1000;

    struct timeval tp;
    gettimeofday(&tp, NULL);
    startClockTime = (tp.tv_sec * 1000 + tp.tv_usec / 1000);
    timerStarted = 1;
    runButton->SetText(" Stop Simulation");
  }else{
    timerStarted = 0;
    runButton->SetText(" Start Simulation");
  }
}

void gluiRender()
{
    glutSetWindow(subWindow1);
    // Clear the back buffer (nice light blue color)
    glClearColor(0.92f, 0.94f, 0.97f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
 
    // Custom rendering code here...
    // ...
    // Left empty on purpose
    // ...
    
    // Explicitly call the Glui2 render last
    GluiHandle->Render();

    if (timerStarted == 1)
    {
      long elapsedTime = 0;
      float elapsedMils = 0;
      calcTimePassed(&elapsedTime, &elapsedMils);
      

      time_t utcOffsetTime = elapsedTime - 25200;
      struct tm * ptm = gmtime(&utcOffsetTime);
      spinYear->SetInt(1900+ptm->tm_year);
      spinMonth->SetInt(1+ptm->tm_mon);
      spinDay->SetInt(ptm->tm_mday);
      spinHour->SetInt(ptm->tm_hour);
      spinMinute->SetInt(ptm->tm_min);
      spinSecond->SetInt(ptm->tm_sec);

      p13.setTime(1900+ptm->tm_year, 1+ptm->tm_mon, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec+elapsedMils);
      p13.setLocation(-105.25185183, 40.01076298, 1604);
      //p13.setElements(2009, 232.55636497, 98.0531, 238.4104, 83652*1.0e-7, 290.6047,
      //    68.6188, 14.406497342, -0.00000001, 27022, 180.0); 
      p13.setElements(2014, 336.89193922, 51.6480, 343.1171, 0.0007491, 91.5059,
          336.3534, 15.51730338, 0.00017289, 91750, 180);
      p13.calculate();
    }

    if(MorA->GetSelectionIndex() == 0){
      spinAz->SetDisabled(true);
      spinEl->SetDisabled(true);
      spinYear->SetDisabled(false);
      spinMonth->SetDisabled(false);
      spinDay->SetDisabled(false);
      spinHour->SetDisabled(false);
      spinMinute->SetDisabled(false);
      spinSecond->SetDisabled(false);
      spinMult->SetDisabled(false);
      setTimeButton->SetDisabled(false);
      runButton->SetDisabled(false);

      azimuth = -p13.getAzimuth()-180;
      elevation = p13.getElevation();
      if(timerStarted == 1){
        spinYear->SetDisabled(true);
        spinMonth->SetDisabled(true);
        spinDay->SetDisabled(true);
        spinHour->SetDisabled(true);
        spinMinute->SetDisabled(true);
        spinSecond->SetDisabled(true);
      }else{
        spinYear->SetDisabled(false);
        spinMonth->SetDisabled(false);
        spinDay->SetDisabled(false);
        spinHour->SetDisabled(false);
        spinMinute->SetDisabled(false);
        spinSecond->SetDisabled(false);
      }
      spinAz->SetFloat(azimuth);
      spinEl->SetFloat(elevation);
    }else{
      spinAz->SetDisabled(false);
      spinEl->SetDisabled(false);
      spinYear->SetDisabled(true);
      spinMonth->SetDisabled(true);
      spinDay->SetDisabled(true);
      spinHour->SetDisabled(true);
      spinMinute->SetDisabled(true);
      spinSecond->SetDisabled(true);
      spinMult->SetDisabled(true);
      setTimeButton->SetDisabled(true);
      runButton->SetDisabled(true);

      azimuth = spinAz->GetFloat();
      elevation = spinEl->GetFloat();
    }

    if (elevation < 5)
      elevation = 5;

    // Flush / force a render, and swap back/front buffers
    glFlush();
    glutSwapBuffers();

    // Tell GLUT we want to re-render afterwards
    glutPostRedisplay();
}

void setToCurrentTime(g2Controller* Caller)
{
  time_t curTime;
  time(&curTime);
  struct tm * ptm = gmtime(&curTime);
  spinYear->SetInt(1900+ptm->tm_year);
  spinMonth->SetInt(1+ptm->tm_mon);
  spinDay->SetInt(ptm->tm_mday);
  spinHour->SetInt(ptm->tm_hour);
  spinMinute->SetInt(ptm->tm_min);
  spinSecond->SetInt(ptm->tm_sec);
}

// Display func for sub window 2
void renderScenesw2()
{
  glutSetWindow(subWindow2);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  gluLookAt(0, satElevNum, 0,
      0 ,0,0,
      1.0f,0,0);

  glPushMatrix();
  glScalef(10.0f,10.0f,10.0f);
  glRotatef(p13.getLatitude()-90,0,0,1);
  glRotatef(-p13.getLongitude()-90,0,1,0);
  glCallList(earth);
  glRotatef(-90, 0, 0, 1);

  struct cLocation myLocation;
  myLocation.dLongitude = 0;//p13.getLongitude();//-105.25185183;
  myLocation.dLatitude = 0;//p13.getLatitude();//40.01076298;
  //print satellite lat and long
  //cout << p13.getLongitude() << endl;
  //cout << p13.getLatitude() << endl;
  struct cTime theTime;
  theTime.iYear = spinYear->GetInt();
  theTime.iMonth = spinMonth->GetInt();
  theTime.iDay = spinDay->GetInt();
  theTime.dHours = spinHour->GetInt();
  theTime.dMinutes = spinMinute->GetInt();
  theTime.dSeconds = spinSecond->GetInt();

  struct cSunCoordinates currentSunCoord;

  sunpos(theTime, myLocation, &currentSunCoord);

  double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
  int zh = fmod(90*t,360.0);
  float RGBA[4] = {1,0.8,0.8,1.0};  //  Colors
  float Emission[]  = {0.5,0.5,0.5,0.6};
  float Ambient[]   = {0.5,0.5,0.5,1.0};
  float Diffuse[]   = {0.8,0.8,0.8,1.0};
  float Specular[]  = {0.8,0.8,0.8,1.0};
  float Position[]  = {0.5*sunRad*Sin(currentSunCoord.dZenithAngle)*Cos(currentSunCoord.dAzimuth),
    0.5*sunRad*Sin(currentSunCoord.dZenithAngle)*Sin(currentSunCoord.dAzimuth),
    0.5*sunRad*Cos(currentSunCoord.dZenithAngle), 0.0, 1.0};
  float Shinyness[] = {16};

  //  Draw light position as sphere (still no lighting here)
  glColor3f(1,1,1);
  glTranslated(Position[0],Position[1],Position[2]);

  //  OpenGL should normalize normal vectors
  glEnable(GL_NORMALIZE);
  //  Enable lighting
  glEnable(GL_LIGHTING);
  //  Enable light 0
  glEnable(GL_LIGHT1);
  //  Set ambient, diffuse, specular components and position of light 0
  glLightfv(GL_LIGHT1,GL_AMBIENT ,Ambient);
  glLightfv(GL_LIGHT1,GL_DIFFUSE ,Diffuse);
  glLightfv(GL_LIGHT1,GL_SPECULAR,Specular);
  glLightfv(GL_LIGHT1,GL_POSITION,Position);
  //  Set materials
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,RGBA);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,RGBA);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

  glPopMatrix();
  glutSwapBuffers();
  glutPostRedisplay();
}

// Display func for sub window 3
void renderScenesw3()
{
  glutSetWindow(subWindow3);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  glLoadIdentity();
  gluLookAt(x-lz*30 , 10, z+lx*30,
      0 ,0 ,0 ,
      0.0f,1.0f,0.0f);

  renderScene2();

  glutSwapBuffers();
  glutPostRedisplay();
}

// Global render func
void renderSceneAll()
{
  // check for keyboard movement
  if (deltaMove) {
    computePos(deltaMove);
    glutSetWindow(mainWindow);
    glutPostRedisplay();
  }

  while (angle+deltaAngle < 0)
    angle+=2*M_PI;
  satCamAz->SetProgress(fmod(angle+deltaAngle+M_PI/2,2*M_PI)/(2*M_PI));
  satCamEl->SetProgress(((float)satElevNum)/300);
  renderScene();
  //renderScenesw1();
  gluiRender();
  renderScenesw2();
  renderScenesw3();
}

void setProjection(int w, int h)
{
  float ratio;
  // Prevent a divide by zero, when window is too short
  // (you cant make a window of zero width).
  ratio = 1.0f * w / h;
  // Reset the coordinate system before modifying
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Set the viewport to be the entire window
        glViewport(0, 0, w, h);

  // Set the clipping volume
  gluPerspective(45,ratio,0.1,1000);
  glMatrixMode(GL_MODELVIEW);
}

void changeSize(int w,int h)
{
  if(h == 0)
    h = 1;

  // we're keeping these values cause we'll need them latter
  width = w;
  height = h;

  // set subwindow 1 as the active window
  glutSetWindow(subWindow1);
  // resize and reposition the sub window
  glutPositionWindow(border,h-177-border);
  glutReshapeWindow(w-2*border, 177);
  glViewport(0, 0, w-2*border, 177);

  // set subwindow 2 as the active window
  glutSetWindow(subWindow2);
  // resize and reposition the sub window
  glutPositionWindow(border, border);
  glutReshapeWindow(w/2-border*3/2, h-177-3*border);
  setProjection(w/2-border*3/2, h-177-3*border);

  // set subwindow 3 as the active window
  glutSetWindow(subWindow3);
  // resize and reposition the sub window
  glutPositionWindow((w+border)/2, border);
  glutReshapeWindow(w/2-border*3/2, h-177-3*border);
  setProjection(w/2-border*3/2, h-177-3*border);

  // Set the position of all the GUI elements
  int contOffset = width/2 -365-2*border;
  QuitButton->SetPos(contOffset+25, 50);
  MorA->SetPos(contOffset+150, 45);
  satCamAz->SetPos(contOffset+25, 90);
  satCamEl->SetPos(contOffset+180, 90);
  manualConfig->SetPos(contOffset+575, 5);
  spinAzTitle->SetPos(contOffset+575, 20);
  spinAz->SetPos(contOffset+575, 35);
  spinElTitle->SetPos(contOffset+645, 20);
  spinEl->SetPos(contOffset+645, 35);
  simulationConfig->SetPos(contOffset+350, 5);
  spinYearTitle->SetPos(contOffset+350, 20);
  spinYear->SetPos(contOffset+350, 35);
  spinMonthTitle->SetPos(contOffset+420, 20);
  spinMonth->SetPos(contOffset+420, 35);
  spinDayTitle->SetPos(contOffset+490, 20);
  spinDay->SetPos(contOffset+490, 35);
  spinHourTitle->SetPos(contOffset+350, 60);
  spinHour->SetPos(contOffset+350, 75);
  spinMinuteTitle->SetPos(contOffset+420, 60);
  spinMinute->SetPos(contOffset+420, 75);
  spinSecondTitle->SetPos(contOffset+490, 60);
  spinSecond->SetPos(contOffset+490, 75);
  spinMultTitle->SetPos(contOffset+460, 100);
  spinMult->SetPos(contOffset+490, 115);
  setTimeButton->SetPos(contOffset+350, 115);
  runButton->SetPos(contOffset+395, 155);
}

// -----------------------------------
//             KEYBOARD
// -----------------------------------

void processNormalKeys(unsigned char key, int xx, int yy)
{
  if (key == 27) {
    glutDestroyWindow(mainWindow);
    exit(0);
  }
}

void pressKey(int key, int xx, int yy)
{
  switch (key) {
    case GLUT_KEY_UP: 
      satElevNum+=5; break;
    case GLUT_KEY_DOWN: 
      satElevNum-=5; break;
  }
  glutSetWindow(mainWindow);
  glutPostRedisplay();
}

void releaseKey(int key, int x, int y)
{
  switch (key) {
    case GLUT_KEY_UP :
    case GLUT_KEY_DOWN : break;
  }
}

// -----------------------------------
//             MOUSE
// -----------------------------------

void mouseMove(int x, int y)
{
  // this will only be true when the left button is down
  if (xOrigin >= 0) {
    // update deltaAngle
    deltaAngle = (x - xOrigin) * 0.001f;

    // update camera's direction
    lx = sin(angle + deltaAngle);
    lz = -cos(angle + deltaAngle);

    glutSetWindow(mainWindow);
    glutPostRedisplay();
  }
}

void mouseButton(int button, int state, int x, int y)
{
  // only start motion if the left button is pressed
  if (button == GLUT_LEFT_BUTTON) {
    // when the button is released
    if (state == GLUT_UP) {
      angle += deltaAngle;
      deltaAngle = 0.0f;
      xOrigin = -1;
    }
    else  {// state = GLUT_DOWN
      xOrigin = x;
    }
  }
}

void idle()
{
  renderSceneAll();
}

// Callback to the application
void Quit(g2Controller* Caller)
{
  exit(0);
}

// -----------------------------------
//             MAIN and INIT
// -----------------------------------

void init()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // register callbacks
  glutIgnoreKeyRepeat(1);
  glutKeyboardFunc(processNormalKeys);
  glutSpecialFunc(pressKey);
  glutSpecialUpFunc(releaseKey);
  glutMouseFunc(mouseButton);
  glutMotionFunc(mouseMove);
  glutIdleFunc(idle);
}

int main(int argc, char **argv)
{
  // init GLUT and create main window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

  // Center the window
  int SystemResWidth = glutGet(GLUT_SCREEN_WIDTH);
  int SystemResHeight = glutGet(GLUT_SCREEN_HEIGHT);
  glutInitWindowPosition(SystemResWidth / 2 - width / 2, SystemResHeight / 2 - height / 2);

  glutInitWindowSize(width,height);
  mainWindow = glutCreateWindow("Seth Miers - Jackalope");

  // Turn on alpha blending for textures
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.01f);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // callbacks for main window
  glutDisplayFunc(renderSceneAll);
  glutReshapeFunc(changeSize);

  // Removing the idle function to save CPU and GPU
  //glutIdleFunc(renderSceneAll);
  init();

  // sub windows
  subWindow1 = glutCreateSubWindow(mainWindow, border, height-177-border, width-2*border, 177);
  // Turn on alpha blending for textures
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.01f);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  InitGlui2();

  subWindow2 = glutCreateSubWindow(mainWindow, border, border,width/2-border*3/2, height-177-3*border);
  glutDisplayFunc(renderScenesw2);
  init();

  // objs to be loaded for window 2
  earth = LoadOBJ("earth/earth.obj");

  subWindow3 = glutCreateSubWindow(mainWindow, (width+border)/2, border, width/2-border*3/2, height-177-3*border);
  glutDisplayFunc(renderScenesw3);
  init();

  // objs to be loaded for window 3
  base = LoadOBJ("dish/base.obj");
  arm = LoadOBJ("dish/arm.obj");
  dish = LoadOBJ("dish/dish.obj");
  plane = LoadOBJ("plane/plane.obj");

  glGenerateMipmap(GL_TEXTURE_2D);

  // enter GLUT event processing cycle
  glutMainLoop();

  return 1;
}
