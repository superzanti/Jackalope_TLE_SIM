# Example 26
EXE=jackalope

# Main target
all: $(EXE)

#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall
LIBS=-lglut32cu -lglu32 -lopengl32
CLEAN=del *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lX11 -lXi -lXmu -lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Dependencies
g2Button.o: Glui2/g2Button.cpp Glui2/g2Button.h
g2CheckBox.o: Glui2/g2CheckBox.cpp Glui2/g2CheckBox.h
g2Config.o: Glui2/g2Config.cpp Glui2/g2Config.h
g2Console.o: Glui2/g2Console.cpp Glui2/g2Console.h
g2Controller.o: Glui2/g2Controller.cpp Glui2/g2Controller.h
g2Dialog.o: Glui2/g2Dialog.cpp Glui2/g2Dialog.h
g2DropDown.o: Glui2/g2DropDown.cpp Glui2/g2DropDown.h
g2Images.o: Glui2/g2Images.cpp Glui2/g2Images.h
g2Label.o: Glui2/g2Label.cpp Glui2/g2Label.h
g2LabelEdit.o: Glui2/g2LabelEdit.cpp Glui2/g2LabelEdit.h
g2Panel.o: Glui2/g2Panel.cpp Glui2/g2Panel.h
g2ProgressBar.o: Glui2/g2ProgressBar.cpp Glui2/g2ProgressBar.h
g2RadioGroup.o: Glui2/g2RadioGroup.cpp Glui2/g2RadioGroup.h
g2Slider.o: Glui2/g2Slider.cpp Glui2/g2Slider.h
g2Spinner.o: Glui2/g2Spinner.cpp Glui2/g2Spinner.h
g2TextField.o: Glui2/g2TextField.cpp Glui2/g2TextField.h
g2Theme.o: Glui2/g2Theme.cpp Glui2/g2Theme.h
g2Utilities.o: Glui2/g2Utilities.cpp Glui2/g2Utilities.h
glui2.o: Glui2/glui2.cpp Glui2/glui2.h
stb_image.o: Glui2/stb_image.c Glui2/stb_image.h
initglui.o: initglui.cpp initglui.h
object.o: object.cpp object.h
fatal.o: fatal.cpp fatal.h
loadtexbmp.o: loadtexbmp.cpp loadtexbmp.h
plan13.o: plan13.cpp plan13.h
sunpos.o: sunpos.cpp sunpos.h
main.o: main.cpp

#  Create archive
glui2.a:g2Button.o g2CheckBox.o g2Config.o g2Console.o g2Controller.o g2Dialog.o g2DropDown.o\
	g2Images.o g2Label.o g2LabelEdit.o g2Panel.o g2ProgressBar.o g2RadioGroup.o g2Slider.o g2Spinner.o\
	g2TextField.o g2Theme.o g2Utilities.o glui2.o stb_image.o
	ar -rcs $@ $^

# Compile rules
%.o: Glui2/%.c
	gcc -c $(CFLG) $<
%.o: Glui2/%.cpp
	g++ -c $(CFLG) $<
%.o: %.c
	gcc -c $(CFLG) $<
%.o: %.cpp
	g++ -c $(CFLG) $<

#  Link
jackalope: main.o initglui.o fatal.o loadtexbmp.o object.o plan13.o sunpos.o glui2.a
	g++ $(LIBS) -o $@ $^

#  Clean
clean:
	$(CLEAN)
