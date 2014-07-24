#pragma once

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	// #include <OpenGL/glu.h>
#else
	#ifdef _WIN32
		#include <windows.h>
	#endif
	
	#include <GL/glew.h> // must be included before gl.h
	#include <GL/gl.h>
	// #include <GL/glu.h>
	// #include <GL/glext.h>
#endif
