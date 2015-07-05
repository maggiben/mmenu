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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include "Texture.h"

void getErr(void)
{
	GLenum errCode;
	const GLubyte *errString;
	if ((errCode = glGetError()) != GL_NO_ERROR)
	{
		errString = gluErrorString(errCode);
		MessageBox(0, (char*)errString, "Error!", MB_OK);
	}
}

char tochar(int cipher)
{
	switch(cipher)
	{
	case 0: return '0'; break;
	case 1: return '1'; break;
	case 2: return '2'; break;
	case 3: return '3'; break;
	case 4: return '4'; break;
	case 5: return '5'; break;
	case 6: return '6'; break;
	case 7: return '7'; break;
	case 8: return '8'; break;
	case 9: return '9'; break;
	default: return -1;
	}
}

char * tostr(int val)
{
	bool minzero=false;
	if (val<0)
	{
		val=-val;
		minzero=true;
	}
	int num = log10(val)+1;
	if (minzero)
		num++;
	if (val==0)
		return "0";
	
	int * ciphers=(int*)malloc(sizeof(int)*(num+1));
	char * str =(char*)malloc(sizeof(char)*(num+1));
	int d=1;
	for (int i=1; i<=num; i++)
	{
		d=1;
		for (int c=1; c<=i; c++) d*=10;
		ciphers[i]=(val%d)/(d/10);
		if (minzero) 
			str[num-1-i]=tochar(ciphers[i]);
		else 
			str[num-i]=tochar(ciphers[i]);
	}
	if (minzero)
	{
		str[num-1]='-';
		str[num]=-1;
	}
	else
		str[num]=-1;
	return str;
}

int power(int base,int exp)
{
	int res=1;
	for (int i=0; i<exp; i++) res*=base;
	return res;
}

int toint(char * str)
{
	int res=0;
	for (int i=strlen(str)-1; i>=0; i--)
	{
		char ch=str[strlen(str)-1-i];
		res+=power(10,i)*(ch-48);
	}
	return res;
}

char * buildgamestring(int* params)
{
	char * res;
	res=strcat(tostr(params[1]),"x");
	res=strcat(res,tostr(params[2]));
	res=strcat(res,":");
	res=strcat(res,tostr(params[3]));
	res=strcat(res,"@");
	res=strcat(res,tostr(params[4]));
	return res;
}

