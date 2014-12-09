#ifndef OBJECT
#define OBJECT

#include <ctype.h>
#include <cstring>
#include <stdio.h>
#include <GL/glut.h>

typedef struct
{
   char* name;                 //  Material name
   float Ka[4],Kd[4],Ks[4],Ns; //  Colors and shininess
   float d;                    //  Transparency
   int map;                    //  Texture
} mtl_t;

static int Nmtl=0;
static mtl_t* mtl=NULL;
static int linelen=0;    //  Length of line
static char* line=NULL;  //  Internal storage for line

static int CRLF(char ch);
static char* readline(FILE* f);
static char* getword(char** line);
static void readfloat(char* line,int n,float x[]);
static void readcoord(char* line,int n,float* x[],int* N,int* M);
static char* readstr(char* line,const char* skip);
static void LoadMaterial(const char* file);
static void SetMaterial(const char* name);
int LoadOBJ(const char* file);

#endif
