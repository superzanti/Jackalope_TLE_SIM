#ifndef LOADTEXBMP
#define LOADTEXBMP

#include <stdio.h>
#include "fatal.h"
#include <GL/glut.h>

static void Reverse(void* x,const int n);
unsigned int LoadTexBMP(const char* file);

#endif
