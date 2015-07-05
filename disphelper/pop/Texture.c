/*
GLExcess v1.0 Demo
Copyright (C) 2001-2003 Paolo Martella

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/
#include <stdlib.h>
#include "Texture.h"

bool Texture_Create(TEXTURE * Texture, char* FileName)
{
	Texture->tID = 0;
	Texture_Kill(Texture);
	glGenTextures(1, &Texture->tID);
	Texture->tID++; // Use an offset of +1 to differentiate from non-initialized state.
	glBindTexture(GL_TEXTURE_2D, Texture->tID-1);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	return true;
}

void Texture_Kill(TEXTURE * Texture)
{
	if(Texture->tID)
	{
		Texture->tID--;
		glDeleteTextures(1, &Texture->tID);
		Texture->tID=0;
	}
}

GLuint pow2(GLuint exp)
{
	GLuint result=1;
	if (!exp)
		return result;
	for (GLuint a=0; a<exp; a++)
	{
		result*=2;
	}
	return result;
}

void Texture_Use(TEXTURE * Texture)
{
	glBindTexture(GL_TEXTURE_2D,Texture->tID-1);
}

bool Texture_Load(TEXTURE * Texture, char* Opaque)
{
	if(Opaque==0)
		return false;

	if (!Texture_Create(Texture, Opaque))
		return false;

	GLuint w,h;

	FILE * fhandle=NULL;
	fhandle=fopen(Opaque,"rb");
	if (!fhandle)
		return false;

	char ww,hh;
	fread(&ww,sizeof(char),1,fhandle);

	w=pow2(ww-48);
	fread(&hh,sizeof(char),1,fhandle);

	h=pow2(hh-48);
	GLuint size=w*h*3;
	char * rgbdata=(char*)malloc(sizeof(char)*size);
	GLuint read=fread(rgbdata,sizeof(char),size,fhandle);
	if (read!=size)
		return false;

	fclose(fhandle);
	gluBuild2DMipmaps(GL_TEXTURE_2D,3,w,h,GL_RGB,GL_UNSIGNED_BYTE,rgbdata);
	return true;
}
