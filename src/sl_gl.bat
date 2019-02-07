REM @ECHO off

SET arch=x64
SET glfw_dir=c:\users\scott\documents\src\external\glfw-3.2.1
SET glew_dir=c:\users\scott\documents\src\external\glew-2.1.0

IF NOT DEFINED VisualStudioVersion (
	IF EXIST "c:\program files (x86)\Microsoft Visual Studio\2017" (
		PUSHD "c:\program files (x86)\Microsoft Visual Studio\2017"
		PWD
		IF EXIST ".\Community" (
			PUSHD ".\Community" 
			PWD
		) ELSE (
			IF EXIST  ".\Professional" (
				PUSHD ".\Professional"
				PWD
			) ELSE (
				ECHO Cannot Determine Visual Studio Version;
				POPD
				EXIT 1				
			)
		)
	) ELSE (
		ECHO Cannot Determine Visual Studio Version; 
		EXIT 1
	)
	PWD
	CALL ".\VC\Auxiliary\Build\vcvarsall.bat" %arch%
	POPD
)

IF NOT EXIST glfw_build (
	MKDIR glfw_build
)

PUSHD glfw_build

SET glfw_headers=%glfw_dir%\src\internal.h			 ^
				 %glfw_dir%\include\GLFW\glfw3.h  	 ^
 				%glfw_dir%\include\GLFW\glfw3native.h ^
				 %glfw_dir%\src\win32_platform.h	   ^
				 %glfw_dir%\src\win32_joystick.h	   ^
                 %glfw_dir%\src\wgl_context.h		  ^
				 %glfw_dir%\src\egl_context.h

FOR %%f in (%glfw_headers%) DO (CP %%f .)
PAUSE

SET glfw_sources=%glfw_dir%\src\context.c     	^
			 	%glfw_dir%\src\init.c 	   	^
				 %glfw_dir%\src\input.c 	 	 ^
				 %glfw_dir%\src\monitor.c 		^
				 %glfw_dir%\src\vulkan.c     	 ^
				 %glfw_dir%\src\window.c    	  ^
				 %glfw_dir%\src\win32_init.c  	^
				 %glfw_dir%\src\win32_joystick.c  ^
                 %glfw_dir%\src\win32_monitor.c   ^
				 %glfw_dir%\src\win32_time.c	  ^
				 %glfw_dir%\src\win32_tls.c	   ^
				 %glfw_dir%\src\win32_window.c	^
                 %glfw_dir%\src\wgl_context.c	 ^
				 %glfw_dir%\src\egl_context.c	
PAUSE
FOR %%f in (%glfw_sources%) DO (CP %%f .)
PAUSE

SET glfw_compiler_options=-MT -c -nologo -D_GLFW_WIN32


CL %glfw_compiler_options% %glfw_sources%


POPD