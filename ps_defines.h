/**
 @author Abhikalp Unakal
 @date 15 jan 2022
 @file ps_defines.h
 @brief pepper engine defines
*/

#ifndef PS_DEFINES_H
#define PS_DEFINES_H

// WINDOWS
#if defined _WIN64 || defined _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define random rand
// included Windows.h before glad.h gets included in pch.h/ps_graphics.h to deal with warning C4005: 'APIENTRY': macro redefinition 
#include "Windows.h"
// telling optimus to switch to nvidia graphics card instead of internal 
// _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
#endif


// APPLE
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif


// WINDOWING FRAMEWORK
#ifdef GLFW
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#endif

#endif
