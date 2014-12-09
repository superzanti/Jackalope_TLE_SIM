#ifndef INITGLUI
#define INITGLUI

#include "Glui2/glui2.h"


// Everything that's contained in initglui.cpp
extern Glui2* GluiHandle;
extern g2Button* QuitButton;
extern g2ProgressBar* satCamAz;
extern g2ProgressBar* satCamEl;
extern g2RadioGroup* MorA;
extern g2Label* manualConfig;
extern g2Label* spinAzTitle;
extern g2Spinner* spinAz;
extern g2Label* spinElTitle;
extern g2Spinner* spinEl;
extern g2Label* simulationConfig;
extern g2Label* spinYearTitle;
extern g2Spinner* spinYear;
extern g2Label* spinMonthTitle;
extern g2Spinner* spinMonth;
extern g2Label* spinDayTitle;
extern g2Spinner* spinDay;
extern g2Label* spinHourTitle;
extern g2Spinner* spinHour;
extern g2Label* spinMinuteTitle;
extern g2Spinner* spinMinute;
extern g2Label* spinSecondTitle;
extern g2Spinner* spinSecond;
extern g2Label* spinMultTitle;
extern g2Spinner* spinMult;
extern g2Button* setTimeButton;
extern g2Button* runButton;

void InitGlui2();

// Everything that initglui.cpp needs to know about
void Quit(g2Controller* Caller);
void startTimer(g2Controller* Caller);
void setToCurrentTime(g2Controller* Caller);
void gluiRender();


#endif
