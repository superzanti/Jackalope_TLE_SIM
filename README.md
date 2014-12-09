# Jackalope's TLE Simulation Program

### Seth Miers

Jackalope is a capstone team I'm working with for my senior design project. We are developing and building a base station for the polar cube satellites. Most orbits can be described in the form of a Two-Line Element (TLE) using this TLE and the location that we want to sample data from it is possible to calculate the azimuth and elevation that the satellite dish needs to be pointing.

The program also features an acurate sun in both windows. The sun has the same azimuth and elevation to the user as it would in real life.

### How to run

    make && ./jackalope

I only used external libraries that were packaged with my operating system (Arch Linux). So it should work for any linux distro given that you have g++ installed.

### Notes

The time is in UTC+0. The current location is in Boulder, Colorado which is UTC-7. Just be careful when thinking about time.

I was going to finish up my other obj file reader from a month ago, but after working on it and seeing my deadline approach fast, I decided just to use the library provided in class.

Glui is an old (2006) library that I had to modify to make work with 'normal' opengl coding.

### Current controlls
The program features 3 windows that can be manipulated.

up/down arrows will change the field of view of the satellite in the left-hand pane.

Dragging the mouse across one of the top two screens will pan the camera around the satellite dish.

Pressing 'esc' in one of the top windows will exit the program.

### Screenshots!

Initial program startup (dark side of the earth)
![Startup](https://raw.githubusercontent.com/superzanti/Jackalope_TLE_SIM/master/screenshots/2014-12-08-162601_1920x1080_scrot.png)

Program tracking the newest (2014) TLE of the ISS
![ISS tracking](https://raw.githubusercontent.com/superzanti/Jackalope_TLE_SIM/master/screenshots/2014-12-08-162627_1920x1080_scrot.png)

Elevation of the dish finally increasing as the ISS passes over Colorado
![Elevation increasing](https://raw.githubusercontent.com/superzanti/Jackalope_TLE_SIM/master/screenshots/2014-12-08-162723_1920x1080_scrot.png)

A more exciting elevation of the dish as the ISS passes over Colorado
![Elevation increasing](https://raw.githubusercontent.com/superzanti/Jackalope_TLE_SIM/master/screenshots/2014-12-08-162749_1920x1080_scrot.png)

Decided to make sure manual control still works
![Manual control](https://raw.githubusercontent.com/superzanti/Jackalope_TLE_SIM/master/screenshots/2014-12-08-162749_1920x1080_scrot.png)

### To-do

 - [ ] Fix comments and A more exciting elevation of the dish as the ISS passes over Colorado
 - [ ] Clean up unused variables
 - [ ] Cleanly separate each header file and make sure it's properly associated with it's cpp/c file
 - [ ] Implement a function to automatically pull values from a TLE insetad of having to manually put them in
 - [ ] More beautiful graphics (hey, I like it to look nice)
 - [ ] Add a skybox with dynamic textures and lighting
 - [ ] 'Tell me which satellite I'm pointing at' function
 - [ ] Sweep to location given 2 points (don't jerk and hop)
 - [ ] Reimplement plan13 to better support milli/nanoseconds
 - [ ] HUD to show the line of horizon for the satellite
 - [ ] Tail for the satellite to show where it's been
 - [ ] Create a standard for all variable and function names (I know I'm sorry)
 - [ ] etc.
