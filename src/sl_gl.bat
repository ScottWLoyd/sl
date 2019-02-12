@ECHO off

SET arch=x64
IF NOT DEFINED VisualStudioVersion (
	IF EXIST "c:\program files (x86)\Microsoft Visual Studio\2017" (
		PUSHD "c:\program files (x86)\Microsoft Visual Studio\2017"		
		IF EXIST ".\Community" (
			PUSHD ".\Community" 
			CALL ".\VC\Auxiliary\Build\vcvarsall.bat" %arch%
			POPD
		) ELSE (
			IF EXIST  ".\Professional" (
				PUSHD ".\Professional"
				CALL ".\VC\Auxiliary\Build\vcvarsall.bat" %arch%
				POPD
			) ELSE (
				ECHO Cannot Determine Visual Studio Version;
				POPD
				EXIT 1				
			)
		)
		POPD
	) ELSE (
		ECHO Cannot Determine Visual Studio Version; 
		EXIT 1
	)	
)


IF [%1]==[] (
    GOTO :build_sdl
) ELSE (
	IF [%1]=="glfw" (
		GOTO :build_glfw
	) ELSE (
		@ECHO Unrecognized target
        @ECHO Usage: %0 ^<TARGET^>
		EXIT /B 1
	)
)


REM ***********************************************************************
REM GLFW
REM ***********************************************************************


:build_glfw
SET glfw_dir=c:\users\sloyd3\documents\src\external\glfw-3.2.1\src
SET glew_dir=c:\users\sloyd3\documents\src\external\glew-2.1.0

SET glfw_build_dir=glfw_build

IF NOT EXIST %glfw_build_dir% (
	MKDIR %glfw_build_dir%\include\GLFW
	MKDIR %glfw_build_dir%\src
	ICACLS %glfw_build_dir% /T /C /grant:r Everyone:F
)

PUSHD %glfw_build_dir%

REM NOTE(scott): these files need to be in a separate directory
REM due to the way the source code imports them
CP %glfw_dir%\include\GLFW\glfw3.h       .\include\GLFW
CP %glfw_dir%\include\GLFW\glfw3native.h .\include\GLFW

SET glfw_headers=%glfw_dir%\src\internal.h			 ^
				 %glfw_dir%\src\osmesa_context.h	   ^
				 %glfw_dir%\src\win32_platform.h	   ^
				 %glfw_dir%\src\win32_joystick.h	   ^
                 %glfw_dir%\src\wgl_context.h		  ^
				 %glfw_dir%\src\egl_context.h

FOR %%f in (%glfw_headers%) DO (CP %%f .\src)

SET glfw_sources=%glfw_dir%\src\context.c     	^
			 	%glfw_dir%\src\init.c 	   	^
				 %glfw_dir%\src\input.c 	 	 ^
				 %glfw_dir%\src\monitor.c 		^
				 %glfw_dir%\src\osmesa_context.c  ^
				 %glfw_dir%\src\vulkan.c     	 ^
				 %glfw_dir%\src\window.c    	  ^
				 %glfw_dir%\src\win32_init.c  	^
				 %glfw_dir%\src\win32_joystick.c  ^
                 %glfw_dir%\src\win32_monitor.c   ^
				 %glfw_dir%\src\win32_time.c	  ^
				 %glfw_dir%\src\win32_thread.c    ^
				 %glfw_dir%\src\win32_window.c	^
                 %glfw_dir%\src\wgl_context.c	 ^
				 %glfw_dir%\src\egl_context.c	
FOR %%f in (%glfw_sources%) DO (CP %%f .\src)

PUSHD src

SET glfw_compiler_options=-MT -c -nologo -D_GLFW_WIN32

CL %glfw_compiler_options% %glfw_sources%
LIB /out:glfw3.lib *.obj
CP glfw3.lib ..
POPD

POPD

GOTO :eof

REM ***********************************************************************
REM SDL
REM ***********************************************************************


:build_sdl

SET sdl_build_dir=build_sdl

IF NOT EXIST %sdl_build_dir% (
	MKDIR %sdl_build_dir%
	ICACLS %sdl_build_dir% /T /C /grant:r Everyone:F
)

PUSHD %sdl_build_dir%
hg clone http://hg.libsdl.org/SDL

mkdir build
PUSHD build

cmake -G "Visual Studio 15 2017 Win64" ..\\SDL
msbuild -p:Configuration=Release SDL2-static.vcxproj


POPD
POPD
