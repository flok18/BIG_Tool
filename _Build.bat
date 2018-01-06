@echo off

Set INCLUDE=
Set INCLUDE=%INCLUDE%;C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Tools\MSVC\14.10.25017\include
Set INCLUDE=%INCLUDE%;C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\um
Set INCLUDE=%INCLUDE%;C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\shared
Set INCLUDE=%INCLUDE%;C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\ucrt

Set LIB=
Set LIB=%LIB%;C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Tools\MSVC\14.10.25017\lib\x64
Set LIB=%LIB%;C:\Program Files (x86)\Windows Kits\10\Lib\10.0.14393.0\um\x64
Set LIB=%LIB%;C:\Program Files (x86)\Windows Kits\10\Lib\10.0.14393.0\ucrt\x64

Set Path=%Path%;C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Tools\MSVC\14.10.25017\bin\HostX64\x64
Set Path=%Path%;C:\Program Files (x86)\Windows Kits\10\bin\x64;


cl.exe /nologo /O2 /W4 /Fe"BIG_Tool.exe" Main.c

del *.obj

Pause
Exit

