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
#include <stdio.h>
#include <math.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <GL\glut.h>
#include "Texture.h"
#include "scene03.h"
TEXTURE b_Text[18];
static GLuint numtexs=17;
static GLuint width=800;
static GLuint height=600;
static bool init=true;
GLfloat b_time=0;

GLuint facesize=128;
float ***b_points;
GLfloat b_zeta=-10.2f;
GLfloat b_count=0;


bool b_switch=true;
bool flag=true;
bool b_switch2=true;
GLuint face;


void dolist(void)
{

	b_points=(float***)malloc(sizeof(float**)*facesize);
	for (int a=0; a<facesize; a++)
	{
		b_points[a]=(float**)malloc(sizeof(float*)*facesize);
		for (int b=0; b<facesize; b++)
		{
			b_points[a][b]=(float*)malloc(sizeof(float) * 3);
			b_points[a][b][0]=0;
			b_points[a][b][1]=0;
			b_points[a][b][2]=0;
		}
	}
	FILE * fhandle=NULL;
	fhandle=fopen("data/face.dat","rb");
	if (!fhandle) 
		return;

	GLuint size = facesize*facesize;
	char * data=(char*)malloc(sizeof(char)*size);
	GLuint read=fread(data,sizeof(char),size,fhandle);
	if (read!=size) 
		return;
	fclose(fhandle);

	for (int x=0; x<facesize; x++)
	{
		for (int y=0; y<facesize; y++)
		{
			b_points[x][y][0]= ((x/5.0f)-12.8f);
			b_points[x][y][1]= ((y/5.0f)-12.8f);
			int temp=data[x*facesize+y];
			if (temp<0) 
				temp+=255;
			b_points[facesize-1-x][facesize-1-y][2]= (temp)/(25.6f*2.5f);
		}
	}

	face=glGenLists(1);
	glNewList(face,GL_COMPILE);
	glBegin(GL_QUADS);
	for(int x = 0; x < 127; x++ )
	{
		for( int y = 0; y < 127; y++ )
		{
			GLfloat float_x = (x)/127.0f;
			GLfloat float_y = (y)/127.0f;
			GLfloat float_xb = (x+1)/127.0f;
			GLfloat float_yb = (y+1)/127.0f;

			glTexCoord2f( float_x, float_y);
			glVertex3f( b_points[x][y][0], b_points[x][y][1], b_points[x][y][2]);

			glTexCoord2f( float_x, float_yb );
			glVertex3f( b_points[x][y+1][0], b_points[x][y+1][1], b_points[x][y+1][2] );

			glTexCoord2f( float_xb, float_yb );
			glVertex3f( b_points[x+1][y+1][0], b_points[x+1][y+1][1], b_points[x+1][y+1][2] );

			glTexCoord2f( float_xb, float_y );
			glVertex3f( b_points[x+1][y][0], b_points[x+1][y][1], b_points[x+1][y][2]);
		}
	}
	glEnd();
	glEndList();
}

void b_InitGL(void)
{
	glutSetWindowTitle("Scene 3");
	b_zeta=-10.2f;b_count=0;
	b_switch=true;
	flag=true;
	b_switch2=true;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Texture_Load(&b_Text[1],"data/face.raw");
	Texture_Load(&b_Text[2],"data/glglow.raw");
	Texture_Load(&b_Text[3],"data/xs1.raw");
	Texture_Load(&b_Text[4],"data/logocol.raw");
	Texture_Load(&b_Text[5],"data/art.raw");
	Texture_Load(&b_Text[6],"data/g1.raw");
	Texture_Load(&b_Text[7],"data/trilogy1.raw");
	Texture_Load(&b_Text[8],"data/s.raw");
	Texture_Load(&b_Text[9],"data/t.raw");
	Texture_Load(&b_Text[10],"data/y.raw");
	Texture_Load(&b_Text[11],"data/l.raw");
	Texture_Load(&b_Text[12],"data/e.raw");
	Texture_Load(&b_Text[13],"data/design.raw");
	Texture_Load(&b_Text[14],"data/designs.raw");
	Texture_Load(&b_Text[15],"data/technique.raw");
	Texture_Load(&b_Text[16],"data/techniques.raw");

	glFogf(GL_FOG_MODE,GL_LINEAR);
	glFogf(GL_FOG_START,40.0f);
	glFogf(GL_FOG_END,55.0f);
	glFogf(GL_FOG_DENSITY,0.175f);
	glShadeModel(GL_FLAT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable (GL_CULL_FACE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_TEXTURE_2D);
	glFrontFace(GL_CW);

	FILE *file=NULL;

	glDisable(GL_LIGHTING);
	dolist();
}

void b_Clean(void)
{
	for(int i = 1; i<numtexs; i++)
	{
		Texture_Kill(&b_Text[i]);
	}
	init=true;
}

void b_drawrect(GLfloat b,GLfloat h)
{
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-b/2,-h/2,0.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(b/2,-h/2,0.0f);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(b/2,h/2,0.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(-b/2,h/2,0.0f);
	glEnd();
}

void b_clear(GLfloat zi)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadIdentity();
	glTranslatef(0,0,-1.0f);
	glColor4f(0,0,0,.1f);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

}

bool b_DrawGLScene(GLfloat globtime)
{
	if (init)
	{
		b_InitGL();
		init=false;
	}
	b_time=4*globtime;
	int x, y;
	float float_x, float_y, float_xb, float_yb;
	if ((b_zeta>92.5f)&&(b_zeta<100.0f))
	{
		GLfloat canc;
		if (b_zeta<94.5f) 
			canc=.375f*(1.0f-cos((b_zeta-92.5f)*3.1415f/2.0f));
		else if (b_zeta>98.0f) 
			canc=.375f*(1.0f+cos((b_zeta-98.0f)*3.1415f/2.0f));
		else 
			canc=.75;
		glClear(GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glLoadIdentity();
		glTranslatef(0,0,-1.0f);
		glColor4f(0,0,0,1.0f-canc);
		b_drawrect(1.2f,1.2f);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	}
	else
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	glLoadIdentity();
	if (b_zeta<6.0f)
	{
		gluLookAt(13*sin(b_zeta/10),2-(b_zeta+2)*(b_zeta+2)/50,-9+10*cos(b_zeta/10),
				13*sin(.5+b_zeta/10),1.5,-9+10*cos(.5+b_zeta/10),
				0,1,0);
		glRotatef(-90,1,0,0);
		glRotatef(-90,0,0,1);
	}
	else if (b_zeta<37.0f)
	{
		if (b_switch)
		{
			glFrontFace(GL_CCW);
			b_switch=false;
		}
		gluLookAt(3*cos((b_zeta-7.5f)/4.0f),1+cos((b_zeta-8.5f)/6.0f)*cos((b_zeta-8.5f)/6.0f),-16+(b_zeta-6.5f),
			-1+3*cos(b_zeta/4.8f),-2,3,	0,1,0);
		glRotatef(180,0,0,1);
		glRotatef(10*cos(b_zeta/2.0f),0,0,1);
		glRotatef(-90,1,0,0);
		glRotatef(-90,0,0,1);
	}
	else if (b_zeta<80.0f)
	{
		glTranslatef(0,1,-13.0f+3*cos(b_zeta/6));
		glRotatef(100,1,0,0);
		glRotatef(b_zeta*5.0f,0,0,1);
	}
	else
	{
		if ((b_zeta-81.8>0)&&(b_zeta-81.8<1)) 
			glTranslatef(0,1,-13.0f+(3.0f+.3*(b_zeta-80)*(b_zeta-80))*cos(b_zeta/6)+.5*(1.0f-cos((b_zeta-81.8)*3.1415*2.0f)));
		else if ((b_zeta-84.4>0)&&(b_zeta-84.4<.35)) 
			glTranslatef(0,1,-13.0f+(3.0f+.3*(b_zeta-80)*(b_zeta-80))*cos(b_zeta/6)+.25*(1.0f-cos((b_zeta-84.4)*3.1415*5.714f)));
		else if ((b_zeta-84.75>0)&&(b_zeta-84.75<1.5)) 
			glTranslatef(0,1,-13.0f+(3.0f+.3*(b_zeta-80)*(b_zeta-80))*cos(b_zeta/6)+1.5*(1.0f-cos((b_zeta-84.75)*3.1415*1.33f)));
		else 
			glTranslatef(0,1,-13.0f+(3.0f+.3*(b_zeta-80)*(b_zeta-80))*cos(b_zeta/6));
		glRotatef(100.0f+sin((b_zeta-80.0f)/7.0f)*sin((b_zeta-80.0f)/7.0f)*80.0f,1.0f,0.0f,0.0f);
		if (b_zeta<88.4)
		{
			glRotatef(80.0f*5.0f+(b_zeta-80.0f)*8.9f*cos((b_zeta-80)/10),0,0,1);
		}
		else
		{
			glRotatef(90,0,0,1);
		}
	}

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	/////////////////		ART			////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////

	GLfloat offset=5.0f;
	if ((b_zeta>-offset)&&(b_zeta<6))
	{
		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glDisable(GL_DEPTH_TEST);
		Texture_Use(&b_Text[5]);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
		glLoadIdentity();
		glTranslatef(-.25,.25+(b_zeta+offset)/30.0f,-2.0f);
		glRotatef(180,1,0,0);
		if ((b_zeta+offset)<2)
		{
			glColor4f(1,1,1,(b_zeta+offset)/4.0f);
			b_drawrect(.5,(4.5f-4.0*sin((b_zeta+offset)*3.1415/4.0f)));
		}
		else
		if ((b_zeta+offset)>8)
		{
			glColor4f(1,1,1,1-(b_zeta+offset-8)/2.5f);
			b_drawrect(.5,(.5f+1.0*(1.0-cos((b_zeta+offset-8)*3.1415/4.0f))));
		}
		else
		{
			glColor4f(1,1,1,(b_zeta+offset)/4.0f);
			b_drawrect(.5,.5);
		}
		glLoadIdentity();
		glTranslatef(.5-(b_zeta+offset)/10.0f,.25+(b_zeta+offset)/30.0f,-2.0f);
		Texture_Use(&b_Text[6]);
		glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_COLOR);
		glColor4f((b_zeta+offset)/4,(b_zeta+offset)/4,(b_zeta+offset)/4,1);
		glRotatef(180,1,0,0);
		if ((b_zeta+offset)>8) 
			b_drawrect(1.5-(b_zeta+offset)/20.0f,(.5f+1.0*(1.0-cos((b_zeta+offset-8)*3.1415/4.0f))));
		else 
			b_drawrect(1.5-(b_zeta+offset)/20.0f,.5);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glPopMatrix();
	}

	//////////////////////////////////////////////////////////

	if (b_zeta<98.0f)
	{
		glColor4f(b_count/90.0f,b_count/90.0f,b_count/90.0f,0);
		Texture_Use(&b_Text[1]);
		if (b_zeta<92.5f)
		{
			if (!flag)
			{
				glCallList(face);
			}
			else
			{
				glBegin(GL_LINES);
				for( x = 0; x < 127; x++ )
				{
					for( y = 0; y < 127; y++ )
					{
						float_x = (x)/127.0f;
						float_y = (y)/127.0f;
						float_xb = (x+1)/127.0f;
						float_yb = (y+1)/127.0f;
						GLfloat raiser=sin(b_count*2*3.14/360.0);
						glTexCoord2f( float_x, float_y);
						glVertex3f( b_points[x][y][0], b_points[x][y][1], b_points[x][y][2]*raiser);
						glTexCoord2f( float_x, float_yb );
						glVertex3f( b_points[x][y+1][0], b_points[x][y+1][1], b_points[x][y+1][2]*raiser);
						glTexCoord2f( float_xb, float_yb );
						glVertex3f( b_points[x+1][y+1][0], b_points[x+1][y+1][1], b_points[x+1][y+1][2]*raiser);
						glTexCoord2f( float_xb, float_y );
						glVertex3f( b_points[x+1][y][0], b_points[x+1][y][1], b_points[x+1][y][2]*raiser);
					}
				}
			glEnd();
			}
		}
		else
		{
			glLoadIdentity();
			glFrontFace(GL_CW);
			glScalef(-1,1,1);
			glDisable(GL_DEPTH_TEST);
			glTranslatef(-0.001,.028,-1.2f-(b_zeta-91.0f)/3.5f);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			glColor4f(1.0f,1.0f,1.0f,1.0f-(b_zeta-92.5f)/5.5f);
			glRotatef(-90,0,0,1);
			b_drawrect(.66f,.649f);
			glEnable(GL_DEPTH_TEST);
			glFrontFace(GL_CCW);
			glDisable(GL_BLEND);
		}
	}
	if (-b_zeta>5.2f) 
		b_count=-(-b_zeta-10.2f)*18.0f;
	if ((b_zeta<-9.0f)||((b_zeta>-3.5f)&&(b_zeta<-2.5f))||((b_zeta>5.5f)&&(b_zeta<6.5f))||((b_zeta>36.0f)&&(b_zeta<38.0f))||
		((b_zeta>47.35f)&&(b_zeta<48.35f))||((b_zeta>58.35f)&&(b_zeta<60.35f))||((b_zeta>86.35)&&(b_zeta<87.35)))
	{
		glLoadIdentity();
		glDisable(GL_DEPTH_TEST);
		glTranslatef(0,0,-0.5f);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		if (b_zeta<-9) 
			glColor4f(1.0f,1.0f,1.0f,-b_zeta-9.0f);
		else if (b_zeta<-2.5f) 
			glColor4f(1.0f,1.0f,1.0f,sin((b_zeta-2.5)*3.1415f));
		else if (b_zeta<6.5f) 
			glColor4f(1.0f,1.0f,1.0f,sin((b_zeta-5.5f)*3.1415f));
		else if (b_zeta<38.0f) 
			glColor4f(1.0f,1.0f,1.0f,.5*(1.0-cos((b_zeta-36.0f)*3.1415f)));
		else if (b_zeta<48.35f) 
			glColor4f(1.0f,1.0f,1.0f,.85*sin((b_zeta-47.35f)*3.1415f));
		else if (b_zeta<60.35f) 
			glColor4f(1.0f,1.0f,1.0f,sin((b_zeta-58.35f)*3.1415f/2));
		else 
			glColor4f(1.0f,1.0f,1.0f,1.0f-(b_zeta-86.35));
		if (b_switch) 
			glScalef(1,-1,1);
		b_drawrect(.6f,.45f);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	//////////////			GL				/////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////

	if (b_zeta>92.5f)
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glLoadIdentity();
		if (b_zeta<97.5f)
		{
			Texture_Use(&b_Text[2]);
			for (int az=0; az<6; az++)
			{
				GLfloat arg=(b_zeta-92.5f-(GLfloat)az/10.0f)*3.1415/10.0f;
				glPushMatrix();
				glTranslatef(3.0f-4.65f*sin(arg), -0.075f*sin(arg),
							 -15.0f*sin(arg));
				if (az==0) 
					glColor4f(1,1,1,1); 
				else 
					glColor4f(1,1,1,(.6-(GLfloat)az*.1)*(1.0f-(b_zeta-92.5f)/5.0f));
				glRotatef(60-60*sin((b_zeta-92.5-(GLfloat)az/10.0f)*3.1415/10),0,1,0);
				b_drawrect(4.9,2.5);
				glPopMatrix();
			}
			if (b_zeta-92.5f<1.0f)
			{
				glDisable(GL_TEXTURE_2D);
				glColor4f(1.0f,1.0f,1.0f,1.0f-(b_zeta-92.5f));
				glLoadIdentity();
				glTranslatef(0,0,-.75f);
				b_drawrect(1.0f,.75f);
			}
		}
		else
		{
			glTranslatef(-1.65f,-0.075f,-15.0f);
			Texture_Use(&b_Text[2]);
			if (b_zeta<105.0f) 
				glColor4f(1.0f,1.0f,1.0f,1.0f);
			else 
				glColor4f(1.0f,1.0f,1.0f,1.0f-(b_zeta-105.0f)/5.0f);
			b_drawrect(4.9,2.5);
		}
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	//////////////			EXCESS			////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////

	if (b_zeta>95.0f)
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glLoadIdentity();
		if (b_zeta<100.0f)
		{

			Texture_Use(&b_Text[3]);
			for (int az=0; az<6; az++)
			{
				GLfloat arg=(b_zeta-95.0f-(GLfloat)az/10.0f)*3.1415/10.0f;
				glPushMatrix();
				glTranslatef(-4.0f+6.17f*sin(arg), 0.05f*sin(arg),
						 -15.0f*sin(arg));
				if (az==0) 
					glColor4f(1,1,1,1); 
				else 
					glColor4f(1,1,1,(.6-(GLfloat)az*.1)*(1.0f-(b_zeta-95.0f)/5.0f));
				glRotatef(-60+60*sin((b_zeta-95.0-(GLfloat)az/10.0f)*3.1415/10),0,1,0);
				b_drawrect(5.6,5.35);
				glPopMatrix();
			}
		}
		else
		{
			glTranslatef(2.17f,0.05f,-15.0f);
			Texture_Use(&b_Text[3]);
			if (b_zeta<105.0f) 
				glColor4f(1.0f,1.0f,1.0f,1.0f);
			else 
				glColor4f(1.0f,1.0f,1.0f,1.0f-(b_zeta-105.0f)/5.0f);
			b_drawrect(5.6,5.35);
		}
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}


	if (b_zeta>100.0f)
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glLoadIdentity();
		glTranslatef(0,0,-15.0f);
		Texture_Use(&b_Text[4]);
		glColor4f(1.0f,1.0f,1.0f,(b_zeta-100.0f)/10.0f);
		b_drawrect(10,5);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}

	b_zeta=-10.2f+(b_time)/400.0f;
	if (b_zeta>110.0f)
	{
		//b_Clean();
		return false;
		//***************************FINISH
	}

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	/////////////////		TRILOGY		///////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////

	offset=59.35f;
	if ((b_zeta-offset>0)&&(b_zeta-offset<20))
	{
		GLfloat factor=1.0f;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glDisable(GL_DEPTH_TEST);
		glLoadIdentity();
		glTranslatef(-.25,0,-1.5f);
		Texture_Use(&b_Text[7]);
		if ((b_zeta-offset)>15) 
			factor=1.0f-(b_zeta-offset-15)/5.0f;
		for (int i=0; i<5; i++)
		{
			glPushMatrix();
			glRotatef(-((b_zeta-offset)-(GLfloat)i/20.0f)*((b_zeta-offset)-(GLfloat)i/20.0f)*5,0,0,1);
			if (i!=0) 
				glColor4f(1,1,1,.35*factor*(1.0f-(GLfloat)i/5.0f));
			else 
				glColor4f(1,1,1,factor);
			b_drawrect(.5,.5);
			glPopMatrix();
		}
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	/////////////////		DESIGN		///////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	offset=16.0f;
	if ((b_zeta-offset>0)&&(b_zeta-offset<12))
	{
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glLoadIdentity();
		glTranslatef(.35-(b_zeta-offset)/20.0f,-.2,-1.0f);
		glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_COLOR);
		
		if ((b_zeta-offset)<4) 
			glColor4f((b_zeta-offset)/10,(b_zeta-offset)/10,(b_zeta-offset)/10,1);
		else if ((b_zeta-offset)>8) 
			glColor4f(.4-(b_zeta-offset-8)*.1,.4-(b_zeta-offset-8)*.1,.4-(b_zeta-offset-8)*.1,1);
		else 
			glColor4f(.4,.4,.4,1);
			
		Texture_Use(&b_Text[14]);
		
		if ((b_zeta-offset)<2) 
			b_drawrect((4.55f-4.0*sin((b_zeta-offset)*3.1415/4.0f)),.25);
		else if ((b_zeta-offset)>10) 
			b_drawrect((.55f+1.0*(1.0-cos((b_zeta-offset-10)*3.1415/4.0f))),.25);
		else 
			b_drawrect(.55,.25);
			
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		Texture_Use(&b_Text[13]);
		
		if ((b_zeta-offset)<4) 
			glColor4f(1,1,1,(b_zeta-offset)/6);
		else if ((b_zeta-offset)>8) 
			glColor4f(1,1,1,.66-(b_zeta-offset-8)*.66/4);
		else 
			glColor4f(1,1,1,.66);
			
		if ((b_zeta-offset)<2) 
			b_drawrect((4.5f-4.0*sin((b_zeta-offset)*3.1415/4.0f)),.25);
		else if ((b_zeta-offset)>10) 
			b_drawrect((.5f+1.0*(1.0-cos((b_zeta-offset-10)*3.1415/4.0f))),.25);
		else 
			b_drawrect(.5,.25);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	/////////////////		STYLE		///////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	offset=38.0f;
	if ((b_zeta-offset>0)&&(b_zeta-offset<21.0f))
	{
		GLfloat zoomer;
		//if (b_zeta-offset<10) zoomer=1-.1*(1.0f-cos((b_zeta-offset)*3.1415/5));
		if (b_zeta-offset<10) 
			zoomer=.8+.1*(1.0f-cos((b_zeta-offset)*3.1415/10));
		else 
			zoomer=1;
			
		b_zeta=b_zeta-offset-10.0;
		if (b_zeta<6)
		{
			GLfloat fall=0.0f;
			if (b_zeta>0) 
				fall=b_zeta*b_zeta*b_zeta/5.0f;
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			glDisable(GL_DEPTH_TEST);
			glLoadIdentity();
			glTranslatef(0,1.25-fall,-5.0f*zoomer);
			Texture_Use(&b_Text[8]);
			GLfloat sizer;
			if (-b_zeta>8.2)
			{
				sizer=sin((b_zeta+10.2)*3.1415f/4.0f);
				glColor4f(1,1,1,sizer);
			}
			else
			{
				sizer=1.0f;
				glColor4f(1,1,1,1-fall/2.0f);
			}

			glRotatef(90.0f-90.0f*sizer,0,1,0);
			glRotatef(-15.0f*fall,0,0,1);
			glRotatef(-15.0f*fall,1,0,0);
			b_drawrect(.5,.5);
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		}
		if (b_zeta>-9.2f)
		{
			GLfloat fall=0.0f;
			if (b_zeta>.5) 
				fall=(b_zeta-.5)*(b_zeta-.5)*(b_zeta-.5)/5.0f;
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			glDisable(GL_DEPTH_TEST);
			glLoadIdentity();
			glTranslatef(.5,1.25-fall,-5.0f*zoomer);
			Texture_Use(&b_Text[9]);
			GLfloat sizer;
			if (-b_zeta>7.2)
			{
				sizer=sin((b_zeta+9.2)*3.1415f/4.0f);
				glColor4f(1,1,1,sizer);
			}
			else
			{
				sizer=1.0f;
				glColor4f(1,1,1,1-fall/2.0f);
			}
			glRotatef(90.0f-90.0f*sizer,0,1,0);
			glRotatef(25.0f*fall,0,0,1);
			glRotatef(25.0f*fall,1,0,0);
			b_drawrect(.5,.5);
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		}
		if (b_zeta>-8.2f)
		{
			GLfloat fall=0.0f;
			if (b_zeta>1.0) 
				fall=(b_zeta-1)*(b_zeta-1)*(b_zeta-1)/5.0f;
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			glDisable(GL_DEPTH_TEST);
			glLoadIdentity();
			glTranslatef(1,1.25-fall,-5.0f*zoomer);
			Texture_Use(&b_Text[10]);
			GLfloat sizer;
			if (-b_zeta>6.2)
			{
				sizer=sin((b_zeta+8.2)*3.1415f/4.0f);
				glColor4f(1,1,1,sizer);
			}
			else
			{
				sizer=1.0f;
				glColor4f(1,1,1,1-fall/2.0f);
			}
			glRotatef(90.0f-90.0f*sizer,0,1,0);
			glRotatef(10.0f*fall,0,0,1);
			glRotatef(10.0f*fall,1,0,0);
			b_drawrect(.5,.5);
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		}
		if (b_zeta>-7.2f)
		{
			GLfloat fall=0.0f;
			if (b_zeta>1.5) 
				fall=(b_zeta-1.5)*(b_zeta-1.5)*(b_zeta-1.5)/5.0f;
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			glDisable(GL_DEPTH_TEST);
			glLoadIdentity();
			glTranslatef(1.5,1.25-fall,-5.0f*zoomer);
			Texture_Use(&b_Text[11]);
			GLfloat sizer;
			if (-b_zeta>5.2)
			{
				sizer=sin((b_zeta+7.2)*3.1415f/4.0f);
				glColor4f(1,1,1,sizer);
			}
			else
			{
				sizer=1.0f;
				glColor4f(1,1,1,1-fall/2.0f);
			}
			glRotatef(90.0f-90.0f*sizer,0,1,0);
			glRotatef(-30.0f*fall,0,0,1);
			glRotatef(-30.0f*fall,1,0,0);
			b_drawrect(.5,.5);
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		}
		if (b_zeta>-6.2f)
		{
			GLfloat fall=0.0f;
			if (b_zeta>2.0) 
				fall=(b_zeta-2)*(b_zeta-2)*(b_zeta-2)/5.0f;
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			glDisable(GL_DEPTH_TEST);
			glLoadIdentity();
			glTranslatef(2.0,1.25-fall,-5.0f*zoomer);
			Texture_Use(&b_Text[12]);
			GLfloat sizer;
			if (-b_zeta>4.2)
			{
				sizer=sin((b_zeta+6.2)*3.1415f/4.0f);
				glColor4f(1,1,1,sizer);
			}
			else
			{
				sizer=1.0f;
				glColor4f(1,1,1,1-fall/2.0f);
			}
			glRotatef(90.0f-90.0f*sizer,0,1,0);
			glRotatef(20.0f*fall,0,0,1);
			glRotatef(20.0f*fall,1,0,0);
			b_drawrect(.5,.5);
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		}
		b_zeta=b_zeta+offset+10;
	}

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	/////////////////	  TECHNIQUE		////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	offset=62.5f;
	if ((b_zeta-offset>0)&&(b_zeta-offset<15))
	{
		b_zeta=b_zeta-offset-10;
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glLoadIdentity();
		glTranslatef(0,0,-1.25);
		GLfloat rot;
		
		if (-b_zeta>6.2) 
			rot=1.0-sin((b_zeta+10.2)*3.1415/8.0f);
		else if ((b_zeta+10)>10.0f) 
			rot=-.5*(1.0-cos((b_zeta)*3.1415f/10.0f));
		else 
			rot=0.0;
			
		glRotatef(90.0f*rot,1,0,0);
		glTranslatef(0,0,.25f);
		glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_COLOR);
		Texture_Use(&b_Text[16]);
		
		if (-b_zeta>6.2) 
			glColor4f(1-rot,1-rot,1-rot,1);
		else 
			glColor4f(1+2*rot,1+2*rot,1+2*rot,1);
			
		b_drawrect(.55,.11);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		Texture_Use(&b_Text[15]);
		
		if (-b_zeta>6.2) 
			glColor4f(1,1,1,1-rot);
		else 
			glColor4f(1,1,1,1+2*rot);
			
		b_drawrect(.5,.0625);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		b_zeta=b_zeta+offset+10;
	}
	if ((flag)&&(b_zeta>-3.0f)) 
		flag=false;
	if (!b_switch2) 
		b_zeta=-8;
	glutSwapBuffers();
	return true;
}
