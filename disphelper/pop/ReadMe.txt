10/09/2003
----------

This is the source code for the demo part only, benchmarking code is not included.
All references to Win32-specific code have been removed, in order to make the
whole thing easily portable. Unfortunately, this also implies that there is no
realiable timing anymore (glut timing just could not do the work), though you can
adjust the timing step with 'a' and 'z' keys. Space bar will jump to the next scene.

Command line arguments:
-----------------------
Arg1: Windowed/Fullscreen (0/1)
Arg2: Width
Arg3: Height
Arg4: Bpp
Arg5: VFreq

Notice about textures:
----------------------
All textures have been converted to a simple R8G8B8 format, where the first two bytes
contain width and height as powers of 2. This makes it possible to load and edit
textures inside Photoshop, just remember to set 3 (interleaved) color channels and
a header size of 2 bytes, most times the program will automatically guess the right
dimensions.

----------------------------------------------------------
As usual, feel free to drop comments: bustard@glexcess.com
----------------------------------------------------------

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