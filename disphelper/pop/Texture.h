#ifndef TEXTURE_H
#define TEXTURE_H
#define MAX_TEXTURE_NAME_LENGTH 64

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <GL\gl.h>
#include <GL\glu.h>

typedef int bool;
#define true 1
#define false 0
typedef struct _Texture
{
	char Name[MAX_TEXTURE_NAME_LENGTH];
	GLuint tID;
	GLuint Width;
	GLuint Height;
}TEXTURE;

bool Texture_Load(TEXTURE * Texture, char* Opaque);
void Texture_Kill(TEXTURE * Texture);
void Texture_Use(TEXTURE * Texture);

#endif
