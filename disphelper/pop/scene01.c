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
#include "scene01.h"
static GLuint width=800;
static GLuint height=600;
GLint cnt=0;
GLint checker=1;
GLfloat rundom;

TEXTURE z_Text[19];
static GLuint numtexs=18;

GLfloat z_time=0;

static bool init=true;

void z_InitGL(void)
{
	glutSetWindowTitle("Scene 1");
	rundom=.001*((GLfloat)(rand()%1000));
	checker=1;
	cnt=0;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Texture_Load(&z_Text[1], "data/introducing.raw");
	Texture_Load(&z_Text[2], "data/introducings.raw");
	Texture_Load(&z_Text[3], "data/opengl.raw");
	Texture_Load(&z_Text[4], "data/openglb.raw");
	Texture_Load(&z_Text[5], "data/glxcess.raw");
	Texture_Load(&z_Text[6], "data/glxcesss.raw");
	Texture_Load(&z_Text[7], "data/wholenew.raw");
	Texture_Load(&z_Text[8], "data/wholenews.raw");
	Texture_Load(&z_Text[9], "data/experience.raw");
	Texture_Load(&z_Text[10], "data/experiences.raw");
	Texture_Load(&z_Text[11], "data/featuring.raw");
	Texture_Load(&z_Text[12], "data/featurings.raw");
	Texture_Load(&z_Text[13], "data/back.raw");
	Texture_Load(&z_Text[14], "data/linenoise.raw");
	Texture_Load(&z_Text[15], "data/dust1.raw");
	Texture_Load(&z_Text[16], "data/dust2.raw");
	Texture_Load(&z_Text[17], "data/sh1.raw");

	glShadeModel(GL_SMOOTH);
	glClearColor(0,0,0,0);
	glClearDepth(1.0f);
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable (GL_CULL_FACE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glPolygonMode(GL_FRONT, GL_FILL);
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
}

void z_Clean(void)
{
	for(int i = 0; i<numtexs; i++)
	{
		Texture_Kill(&z_Text[i]);
	}
	init=true;
}

void z_drawrect(GLfloat b,GLfloat h)
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

void z_drawrectf(GLfloat b,GLfloat h)
{
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-b/2,-h/2,0.0f);
		glTexCoord2f(b,0.0f);
		glVertex3f(b/2,-h/2,0.0f);
		glTexCoord2f(b,1.0f);
		glVertex3f(b/2,h/2,0.0f);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(-b/2,h/2,0.0f);
	glEnd();
}

void z_drawrecta(GLfloat b,GLfloat h,GLfloat sht,GLfloat shs)
{
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f+sht,0.0f);
		glVertex3f(-b/2,-h/2,0.0f);
		glTexCoord2f(1.0f*shs+sht,0.0f);
		glVertex3f(b/2,-h/2,0.0f);
		glTexCoord2f(1.0f*shs+sht,1.0f);
		glVertex3f(b/2,h/2,0.0f);
		glTexCoord2f(0.0f+sht,1.0f);
		glVertex3f(-b/2,h/2,0.0f);
	glEnd();
}

void z_drawrectb(GLfloat b,GLfloat h,GLfloat shs,GLfloat sht)
{
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f+shs,0.0f+sht);
		glVertex3f(-b/2,-h/2,0.0f);
		glTexCoord2f(1.0f+shs,0.0f+sht);
		glVertex3f(b/2,-h/2,0.0f);
		glTexCoord2f(1.0f+shs,1.0f+sht);
		glVertex3f(b/2,h/2,0.0f);
		glTexCoord2f(0.0f+shs,1.0f+sht);
		glVertex3f(-b/2,h/2,0.0f);
	glEnd();
}

void z_drawrectc(GLfloat b,GLfloat h)
{
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,.9f);
		glVertex3f(-b/2,0,0.0f);
		glTexCoord2f(1.0f,0.9f);
		glVertex3f(b/2,0,0.0f);
		glColor4f(0,0,0,0);
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(b/2,h,0.0f);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-b/2,h,0.0f);
	glEnd();
}

void z_draw(GLfloat z_w, GLfloat z_h, GLfloat z_fact, GLfloat z_tlt)
{
	glPushMatrix();
	glTranslatef(-z_tlt, -z_tlt*z_h/z_w,0.0f);
	glBegin(GL_TRIANGLE_FAN);

	glTexCoord2f(1.0f,1.0f);
	glVertex3f(1.0f*z_w,1.0f*z_h,0.0f);

	glTexCoord2f(0.0f,1.0f);
	glVertex3f(-1.0f*z_w,1.0f*z_h,0.0f);

	glTexCoord2f(0.0f,.125f);
	glVertex3f(-1.0f*z_w,-.75f*z_h,0.0f);

	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-1.0f*z_w-z_fact,-1.0f*z_h-z_fact*z_h/z_w,0.0f);

	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(z_tlt, z_tlt*z_h/z_w,0.0f);
	glBegin(GL_TRIANGLE_FAN);

	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-1.0f*z_w,-1.0f*z_h,0.0f);

	glTexCoord2f(1.0f,0.0f);
	glVertex3f(1.0f*z_w,-1.0f*z_h,0.0f);

	glTexCoord2f(1.0f,0.825f);
	glVertex3f(1.0f*z_w,.75f*z_h,0.0f);

	glTexCoord2f(1.0f,1.0f);
	glVertex3f(1.0f*z_w+z_fact,1.0f*z_h+z_fact*z_h/z_w,0.0f);

	glEnd();
	glPopMatrix();
}

bool z_DrawGLScene(GLfloat globtime)
{
	if (init)
	{
		z_InitGL();
		init=false;
	}
	z_time=globtime*.0001;
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_BLEND);
	glColor4f(1,1,1,1);
	Texture_Use(&z_Text[13]);
	glLoadIdentity();
	glTranslatef(0,0,-1+.1*sin(z_time*10));
	glRotatef(z_time*50,0,0,1);
	z_drawrect(1,1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1,1,1,.25);
	glRotatef(-z_time*100,0,0,1);
	z_drawrect(1,1);

	GLfloat z_offset;

	/////////////////////// INTRODUCING
	z_offset=0.1f;
	if (((z_time - z_offset)>0.0f) && ((z_time - z_offset) * 25.0f < 6.283f))
	{
		glLoadIdentity();
		glTranslatef(.75f-(z_time-z_offset)*2.0f,-.5f,-3.0f);
		glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_COLOR);
		Texture_Use(&z_Text[2]);
		GLfloat menne=.375*(1.0f-cos((z_time-z_offset)*25.0f));
		glColor4f(menne,menne,menne,1);
		glPushMatrix();
		glTranslatef(.05,0,0);
		z_drawrect(1.3,.4);
		glPopMatrix();
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		Texture_Use(&z_Text[1]);
		for (int i=1; i<=5; i++)
		{
			if (i!=1) 
				glColor4f(1,1,1,(1.0f-cos((z_time-z_offset)*25.0f))/(5+i*5));
			else 
				glColor4f(1,1,1,(1.0f-cos((z_time-z_offset)*25.0f))*.5f);
			glPushMatrix();
			glTranslatef((GLfloat)i/(50.0f-(z_time-z_offset)*135.0f),0,0);
			if ((z_time-z_offset)<.1f) 
				z_draw(.5,.1,0,0); 
			else
				z_draw(.5,.1,.05f*(1.0f-cos(((z_time-z_offset)-.1f)*12.5f)),.05f*(1.0f-cos(((z_time-z_offset)-.1f)*12.5f)));
			glPopMatrix();
		}
	}

	//////////////////  WHOLE NEW

	z_offset=0.2f;
	if (((z_time-z_offset)>0.0f)&&((z_time-z_offset)*25.0f<6.283f))
	{
		glLoadIdentity();
		glTranslatef(-.35f,.5f-(z_time-z_offset),-2.0f);
		glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_COLOR);
		Texture_Use(&z_Text[8]);

		GLfloat menne=.5*(1.0f-cos((z_time-z_offset)*25.0f));
		glColor4f(menne,menne,menne,1);
		glPushMatrix();
		glTranslatef(-.05,-.025,0);
		z_drawrect(1.1,.35);
		glPopMatrix();
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		Texture_Use(&z_Text[7]);

		glRotatef(180,1,0,0);
		glTranslatef(-(z_time-z_offset)*.5f,0,0);
		for (int i=1; i<=5; i++)
		{
			if (i!=1) 
				glColor4f(1,1,1,(1.0f-cos((z_time-z_offset)*25.0f))/(5+i*5));
			else 
				glColor4f(1,1,1,(1.0f-cos((z_time-z_offset)*25.0f))*.5f);
			glPushMatrix();
			glTranslatef(0,(GLfloat)i/(50.0f-(z_time-z_offset)*150.0f),0);
			if ((z_time-z_offset)<.1f) 
				z_draw(.5,.09,0,0); 
			else
				z_draw(.5,.09,.05f*(1.0f-cos(((z_time-z_offset)-.1f)*12.5f)),.05f*(1.0f-cos(((z_time-z_offset)-.1f)*12.5f)));
			glPopMatrix();
		}
	}

	////////////////// EXPERIENCE

	z_offset=0.3f;

	if (((z_time-z_offset)>0.0f)&&((z_time-z_offset)*25.0f<6.283f))
	{
		glLoadIdentity();
		glScalef(1,-1,1);
		glTranslatef(-.5f+(z_time-z_offset)*1.5f,0.25f,-2.5f);
		glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_COLOR);
		Texture_Use(&z_Text[10]);

		GLfloat menne=.5*(1.0f-cos((z_time-z_offset)*25.0f));
		glColor4f(menne,menne,menne,1);
		glPushMatrix();
		glTranslatef(.05,.0,0);
		z_drawrect(1.2,.35);
		glPopMatrix();
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		Texture_Use(&z_Text[9]);

		glRotatef(180,1,0,0);
		for (int i=1; i<=5; i++)
		{
			if (i!=1) 
				glColor4f(1,1,1,(1.0f-cos((z_time-z_offset)*25.0f))/(5+i*5));
			else 
				glColor4f(1,1,1,(1.0f-cos((z_time-z_offset)*25.0f))*.5f);
		glPushMatrix();
		glTranslatef((GLfloat)i/(50.0f-(z_time-z_offset)*135.0f),0,0);
		if ((z_time-z_offset)<.1f) 
			z_draw(.5,.09,0,0); 
		else
			z_draw(.5,.09,.05f*(1.0f-cos(((z_time-z_offset)-.1f)*12.5f)),.05f*(1.0f-cos(((z_time-z_offset)-.1f)*12.5f)));
		glPopMatrix();
		}
	}

	//////////////////// FEATURING

	z_offset=0.4f;
	if (((z_time-z_offset)>0.0f)&&((z_time-z_offset)*25.0f<6.283f))
	{
		glLoadIdentity();
		glScalef(1,-1,1);
		glTranslatef(.25f-(z_time-z_offset)*2.0f,-.5f,-3.0f);
		glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_COLOR);
		Texture_Use(&z_Text[12]);

		GLfloat menne=.45*(1.0f-cos((z_time-z_offset)*25.0f));
		glColor4f(menne,menne,menne,1);
		glPushMatrix();
		glTranslatef(.05,.01,0);
		z_drawrect(1.3,.4);
		glPopMatrix();
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		Texture_Use(&z_Text[11]);

		for (int i=1; i<=5; i++)
		{
			if (i!=1) 
				glColor4f(1,1,1,(1.0f-cos((z_time-z_offset)*25.0f))/(5+i*5));
			else 
				glColor4f(1,1,1,(1.0f-cos((z_time-z_offset)*25.0f))*.5f);
			glPushMatrix();
			glTranslatef((GLfloat)i/(50.0f-(z_time-z_offset)*135.0f),0,0);
			if ((z_time-z_offset)<.1f) 
				z_draw(.5,.1,0,0); 
			else
				z_draw(.5,.1,.05f*(1.0f-cos(((z_time-z_offset)-.1f)*12.5f)),.05f*(1.0f-cos(((z_time-z_offset)-.1f)*12.5f)));
			glPopMatrix();
		}
	}

	///////////////////// OPENGL

	z_offset=0.5f;
	if (((z_time-z_offset)>0.0f)&&((z_time-z_offset)*25.0f<6.283f))
	{
		glLoadIdentity();
		glTranslatef(0,0,-.9f);
		Texture_Use(&z_Text[4]);

		glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_COLOR);
		GLfloat menne=(1.0f-cos((z_time-z_offset)*25.0f))*.5f;
		glColor4f(menne,menne,menne,1);
		z_drawrect(.5,.25);
		//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
		Texture_Use(&z_Text[3]);

		//menne=(1.0f-cos((z_time-z_offset)*25.0f))*.5f;
		glColor4f(menne,menne,menne,menne);
		z_drawrect(.5,.25);
	}

	////////////////////// GL EXCESS

	z_offset=.7f;
	if (((z_time-z_offset)>0.0f)&&((z_time-z_offset)*25.0f<6.283f))
	{
		glLoadIdentity();
		glTranslatef(.05f-(z_time-z_offset)/2.0f,0.0f,-1.5f);
		glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_COLOR);
		Texture_Use(&z_Text[6]);

		GLfloat menne=.45*(1.0f-cos((z_time-z_offset)*25.0f));
		glColor4f(menne,menne,menne,1);
		glPushMatrix();
		//glTranslatef(0,0,0);
		z_drawrect(1.2,.6);
		glPopMatrix();
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		Texture_Use(&z_Text[5]);

		for (int i=1; i<=5; i++)
		{
			if (i!=1) 
				glColor4f(1,1,1,(1.0f-cos((z_time-z_offset)*25.0f))/(5+i*5));
			else 
				glColor4f(1,1,1,(1.0f-cos((z_time-z_offset)*25.0f))*.5f);
			glPushMatrix();
			glTranslatef((GLfloat)i/(100.0f-(z_time-z_offset)*350.0f),0,0);
			if ((z_time-z_offset)<.1f) 
				z_draw(.5,.15,0,0); 
			else
				z_draw(.5,.15,.05f*(1.0f-cos(((z_time-z_offset)-.1f)*12.5f)),.05f*(1.0f-cos(((z_time-z_offset)-.1f)*12.5f)));
			glPopMatrix();
		}
	}

	if (z_time<.1f)
	{
		glDisable(GL_TEXTURE_2D);
		glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_COLOR);
		glLoadIdentity();
		glTranslatef(0,0,-1.0f);
		GLfloat fader=.5f+.5f*cos(z_time*31.415f);
		glColor4f(fader,fader,fader,1);
		z_drawrect(1.33f,1.0f);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	}

	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glTranslatef(0,0,-1.5);
	Texture_Use(&z_Text[14]);

	GLfloat ran=.00025*((GLfloat)(rand()%1000));
	glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_COLOR);
	GLfloat shaderf=.16+.16*sin(z_time*100.0f);
	glColor4f(.33+shaderf+ran,.33+shaderf+ran,.33+shaderf+ran,1);
	//z_drawrecta(2,2,2.0f*sin(z_time*10.0f),1.0f+.5f*cos(z_time*50.0f));
	//z_drawrecta(1.5,1.5,-1.5f*cos(z_time*22.0f),1.0f+.75f*cos(z_time*13.0f));
	glPushMatrix();
	glTranslatef(0,0,.25+.25*cos(z_time*20.0f));
	z_drawrectb(1.5,4.5,0,z_time*100);
	glPopMatrix();
	glColor4f(.33-shaderf+ran,.33-shaderf+ran,.33-shaderf+ran,1);
	glScalef(-1,1,1);
	glTranslatef(1,0,-.75-.75*sin(z_time*40.0f));
	z_drawrectb(3,3,0,-z_time*100);
	//glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	if ((int)(z_time*50)==checker)
	{
		if (cnt>=2) 
		{
			checker++; 
			cnt=0; 
			rundom=.001*((GLfloat)(rand()%1000));
		}
		else if (rundom>.2) 
			cnt++; 
		else 
			cnt+=2;
		glRotatef(360.0*rundom,0,0,1);
		glTranslatef(.25*rundom,0,-1);
		if (rundom>.5)
			Texture_Use(&z_Text[15]);
		else
			Texture_Use(&z_Text[16]);

		glRotatef(360.0*rundom*rundom,0,0,1);
		z_drawrect(.1+rundom/3,.1+rundom/3);

		glDisable(GL_TEXTURE_2D);
		glLoadIdentity();
		glTranslatef(0,0,-1.0f);
		glColor4f(1,1,1,.00005*((GLfloat)(rand()%1000)));
		glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_ALPHA);
		z_drawrect(1.33f,1.0f);
		glEnable(GL_TEXTURE_2D);
	}

	if (z_time>.92f)
	{
		GLfloat fader=(z_time-.92f)*75.0f;
		if (fader>1.0f)
		{
			//FINISH*****************************************************************
//			return TRUE;
			//z_Clean();
			return false;
		}
		glDisable(GL_TEXTURE_2D);
		glLoadIdentity();
		glTranslatef(0,0,-1.0f);
		glColor4f(1,1,1,fader);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		z_drawrect(1.33f,1.0f);
		glEnable(GL_TEXTURE_2D);
	}

	z_offset=.8;
	if (z_time>z_offset)
	{
		//if (swi) {FSOUND_PlaySound(FSOUND_FREE, cut);swi=false;}
		glLoadIdentity();
		glDisable(GL_DEPTH_TEST);
		Texture_Use(&z_Text[17]);

		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glTranslatef(-.05,-.0175,-1);
		glRotatef(16,0,0,1);
		glRotatef(85,1,0,0);
		glColor4f(1,1,1,.95+.00005*((GLfloat)(rand()%1000)));
		if (z_time-z_offset<.0125f) 
			z_drawrectc(.75f*sin((z_time-z_offset)*80*3.1415f/2.0f),1.5f);
		else 
			z_drawrectc(.75f,1.5f);
		glRotatef(8.5,1,0,0);
		glColor4f(1,1,1,.95+.00005*((GLfloat)(rand()%1000)));
		if (z_time-z_offset<.0125f) 
			z_drawrectc(.75f*sin((z_time-z_offset)*80*3.1415f/2.0f),1.5f);
		else 
			z_drawrectc(.75f,1.5f);
	}
//	if (benchmode) z_time=thatime.query()/20000.0f;///z_time=((GLfloat)FSOUND_Stream_GetTime(stream))/20000.0f;
//	else z_time=((GLfloat)FSOUND_Stream_GetTime(stream))/50000.0f;
	//return TRUE;
	glutSwapBuffers();
	return true;
}

