#ifndef FATAL
#define FATAL

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <GL/glut.h>

void Fatal(const char* format , ...);
void ErrCheck(const char* where);

#endif
