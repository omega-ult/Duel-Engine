//  [4/7/2013 OMEGA] created

#ifndef _DUELGLCOMMON_H_
#define _DUELGLCOMMON_H_

#include "DuelConfig.h"
#include "DuelGLConfig.h"
#include <GL/glew.h>
#ifdef DUEL_PLATFORM_WINDOWS
#include <GL/wglew.h>
#pragma comment( lib, "glew32.lib")
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#endif
#include "DuelGLPredeclare.h"



#endif