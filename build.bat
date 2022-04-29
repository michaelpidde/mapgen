@echo off

cl /nologo /EHsc /Zi /MT /EHa- mapgen.cpp ^
/link /nologo /SUBSYSTEM:windows /opt:ref user32.lib winmm.lib gdi32.lib
