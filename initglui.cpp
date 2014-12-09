#include "initglui.h"

// Initialize Glui2 library
void InitGlui2()
{
    // Create a glui instance and register necessary handles
    GluiHandle = new Glui2("g2Default.cfg");
    glutDisplayFunc(gluiRender);

    // Place a single button in the middle (Small offset to center)
    QuitButton = GluiHandle->AddButton(25, 50, "  Quit the simulation ", Quit);//" Open Config File ", Quit);

    //Manual or TLE
    const char* Options[2];
    Options[0] = "1. Automated TLE Tracking";
    Options[1] = "2. Manual Dish Control";
    MorA = GluiHandle->AddRadioGroup(150, 45, Options, 2);

    //Screen Progress Bars
    satCamAz = GluiHandle->AddProgressBar(25, 90);
    satCamAz->SetWidth(150);
    satCamAz->SetProgress(0.5f);
    satCamAz->SetText("Camera Azimuth");

    satCamEl = GluiHandle->AddProgressBar(180, 90);
    satCamEl->SetWidth(150);
    satCamEl->SetProgress(0.5f);
    satCamEl->SetText("Satellite Elevation");

    //Manual config
    manualConfig = GluiHandle->AddLabel(575, 5, "Manual Configuration:");
    manualConfig->SetColor(1, 0, 0);

    spinAzTitle = GluiHandle->AddLabel(575, 20, "Azimuth:");
    spinAzTitle->SetColor(0, 0, 0);
    spinAz = GluiHandle->AddSpinner(575, 35, g2SpinnerType_Float);
    spinAz->SetBounds(0, 360);
    spinAz->SetIncrement(0.1f);
    spinAz->SetWidth(50);
    spinAz->SetInt(0);

    spinElTitle = GluiHandle->AddLabel(645, 20, "Elevation:");
    spinElTitle->SetColor(0, 0, 0);
    spinEl = GluiHandle->AddSpinner(645, 35, g2SpinnerType_Float);
    spinEl->SetBounds(0, 90);
    spinEl->SetIncrement(0.1f);
    spinEl->SetWidth(50);
    spinEl->SetInt(0);

    //Simulation config
    simulationConfig = GluiHandle->AddLabel(350, 5, "Simmulation Configuration:");
    simulationConfig->SetColor(1, 0, 0);

    spinYearTitle = GluiHandle->AddLabel(350, 20, "Year:");
    spinYearTitle->SetColor(0, 0, 0);
    spinYear = GluiHandle->AddSpinner(350, 35, g2SpinnerType_Int);
    spinYear->SetBounds(1000, 5000);
    spinYear->SetIncrement(1);
    spinYear->SetWidth(50);
    spinYear->SetInt(2014);

    spinMonthTitle = GluiHandle->AddLabel(420, 20, "Month:");
    spinMonthTitle->SetColor(0, 0, 0);
    spinMonth = GluiHandle->AddSpinner(420, 35, g2SpinnerType_Int);
    spinMonth->SetBounds(1, 12);
    spinMonth->SetIncrement(1);
    spinMonth->SetWidth(50);
    spinMonth->SetInt(1);

    spinDayTitle = GluiHandle->AddLabel(490, 20, "Day:");
    spinDayTitle->SetColor(0, 0, 0);
    spinDay = GluiHandle->AddSpinner(490, 35, g2SpinnerType_Int);
    spinDay->SetBounds(1, 31);
    spinDay->SetIncrement(1);
    spinDay->SetWidth(50);
    spinDay->SetInt(1);

    spinHourTitle = GluiHandle->AddLabel(350, 60, "Hour:");
    spinHourTitle->SetColor(0, 0, 0);
    spinHour = GluiHandle->AddSpinner(350, 75, g2SpinnerType_Int);
    spinHour->SetBounds(0, 23);
    spinHour->SetIncrement(1);
    spinHour->SetWidth(50);
    spinHour->SetInt(0);

    spinMinuteTitle = GluiHandle->AddLabel(420, 60, "Minute:");
    spinMinuteTitle->SetColor(0, 0, 0);
    spinMinute = GluiHandle->AddSpinner(420, 75, g2SpinnerType_Int);
    spinMinute->SetBounds(1, 60);
    spinMinute->SetIncrement(1);
    spinMinute->SetWidth(50);
    spinMinute->SetInt(1);

    spinSecondTitle = GluiHandle->AddLabel(490, 60, "Second:");
    spinSecondTitle->SetColor(0, 0, 0);
    spinSecond = GluiHandle->AddSpinner(490, 75, g2SpinnerType_Int);
    spinSecond->SetBounds(1, 60);
    spinSecond->SetIncrement(1);
    spinSecond->SetWidth(50);
    spinSecond->SetInt(1);

    spinMultTitle = GluiHandle->AddLabel(460, 100, "Time Multiplier:");
    spinMultTitle->SetColor(0, 0, 0);
    spinMult = GluiHandle->AddSpinner(490, 115, g2SpinnerType_Float);
    spinMult->SetBounds(-10, 10);
    spinMult->SetIncrement(0.1f);
    spinMult->SetWidth(50);
    spinMult->SetInt(1);

    setTimeButton = GluiHandle->AddButton(350, 115, " Set Time to Current ", setToCurrentTime);

    runButton = GluiHandle->AddButton(395, 155, " Start Simulation ", startTimer);
}
