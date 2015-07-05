# ------------------------------------------
# tcplayer.mak
#-------------------------------------------

!IFNDEF SRCDIR
SRCDIR=.
!ENDIF

!IFNDEF COMMONDIR
COMMONDIR=..\common
!ENDIF

EXEFILE=..\out\tcplayer.exe
LANGID=0x411
RCFILE=$(SRCDIR)\tcplayer.rc
RESFILE=tcplayer.res
TDSFILE=..\out\tcplayer.tds
TCLOCKH=$(SRCDIR)\tcplayer.h $(SRCDIR)\resource.h $(COMMONDIR)\common.h
COMMONH=$(COMMONDIR)\common.h

OBJS=playermain.obj playerdlg.obj player.obj\
	tclang.obj langcode.obj playfile.obj\
	utl.obj exec.obj reg.obj font.obj

LIBS=kernel32.lib user32.lib gdi32.lib advapi32.lib\
	shell32.lib winmm.lib

all: $(EXEFILE)

# Visual C++
!IFDEF _NMAKE_VER

CC=cl
LINK=link
RC=rc
RCOPT=/l $(LANGID) /fo 

!IFDEF NODEFAULTLIB

COPT=/c /W3 /O2 /Oi /DNODEFAULTLIB /Fo
LOPT=/SUBSYSTEM:WINDOWS /NODEFAULTLIB /OPT:NOWIN98

$(EXEFILE): $(OBJS) nodeflib.obj $(RESFILE)
	$(LINK) $(LOPT) $(OBJS) nodeflib.obj $(RESFILE) $(LIBS) /OUT:$@

!ELSE

COPT=/c /W3 /O2 /Oi /Fo
LOPT=/SUBSYSTEM:WINDOWS /OPT:NOWIN98

$(EXEFILE): $(OBJS) $(RESFILE)
	$(LINK) $(LOPT) $(OBJS) $(RESFILE) $(LIBS) /OUT:$@

!ENDIF

# Borland C++ Compiler
!ELSE

CC=bcc32
LINK=ilink32
RC=brc32
RCOPT=-r -32 -fo

!IFDEF NODEFAULTLIB
COPT=-c -w -w-8057 -O2 -Oi -d -DNODEFAULTLIB -tW -o
LOPT=/c /C /Gn /x

$(EXEFILE): $(OBJS) nodeflib.obj bccexe.pat $(RESFILE)
	$(LINK) $(LOPT) /Tpe /aa $(OBJS) nodeflib.obj bccexe.pat,$@,,$(LIBS),,$(RESFILE)
	del $(TDSFILE)

bccexe.pat: $(COMMONDIR)\bccexe.nas
	nasmw -f obj -o $@ $(COMMONDIR)\bccexe.nas

!ELSE
COPT=-c -w -w-8057 -O2 -Oi -d -tW -o
LOPT=/c /C /Gn /x

$(EXEFILE): $(OBJS) $(RESFILE)
	$(LINK) $(LOPT) /Tpe /aa $(OBJS) c0w32.obj,$@,,$(LIBS) cw32.lib,,$(RESFILE)
	del $(TDSFILE)

!ENDIF

!ENDIF

# obj files

playermain.obj: $(SRCDIR)\main.c $(TCLOCKH)
	$(CC) $(COPT)$@ $(SRCDIR)\main.c
playerdlg.obj: $(SRCDIR)\dialog.c $(TCLOCKH)
	$(CC) $(COPT)$@ $(SRCDIR)\dialog.c
player.obj: $(SRCDIR)\player.c $(TCLOCKH)
	$(CC) $(COPT)$@ $(SRCDIR)\player.c

# common obj files

tclang.obj: $(COMMONDIR)\tclang.c $(COMMONH)
	$(CC) $(COPT)$@ $(COMMONDIR)\tclang.c
langcode.obj: $(COMMONDIR)\langcode.c $(COMMONH)
	$(CC) $(COPT)$@ $(COMMONDIR)\langcode.c
playfile.obj: $(COMMONDIR)\playfile.c $(COMMONH)
	$(CC) $(COPT)$@ $(COMMONDIR)\playfile.c
utl.obj: $(COMMONDIR)\utl.c $(COMMONH)
	$(CC) $(COPT)$@ $(COMMONDIR)\utl.c
exec.obj: $(COMMONDIR)\exec.c $(COMMONH)
	$(CC) $(COPT)$@ $(COMMONDIR)\exec.c
reg.obj: $(COMMONDIR)\reg.c $(COMMONH)
	$(CC) $(COPT)$@ $(COMMONDIR)\reg.c
font.obj: $(COMMONDIR)\font.c $(COMMONH)
	$(CC) $(COPT)$@ $(COMMONDIR)\font.c
nodeflib.obj: $(COMMONDIR)\nodeflib.c $(COMMONH)
	$(CC) $(COPT)$@ $(COMMONDIR)\nodeflib.c

# res file

$(RESFILE): $(RCFILE)
	$(RC) $(RCOPT)$@ $(RCFILE)
