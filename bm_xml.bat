c:\lcc\bin\lcc -IC:\LCC\include -O -g2 -S main_menu.c
REM c:\lcc\bin\lcc -IC:\LCC\include -O -g2 -S disphelper.c

REM c:\lcc\bin\lrc -IC:\LCC\include main_menu.rc
c:\lcc\bin\lcclnk  -subsystem windows -o main.exe main_menu.obj disphelper.obj main_menu.res tcconio.lib uuid.lib ole32.lib oleaut32.lib shell32.lib 