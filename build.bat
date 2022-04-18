@echo off

cl /nologo /EHsc mapgen.cpp ^
/link /nologo /SUBSYSTEM:windows /opt:ref user32.lib winmm.lib
