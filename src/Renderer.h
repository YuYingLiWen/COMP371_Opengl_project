#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>


#ifdef _DEBUG
#define DEBUG_LOG(x) std::cout << "DEBUG: " << x << std::endl
#else
#define DEBUG_LOG(x) //x
#endif // _NDEBUG


#define BLANK_LINE std::cout << std::endl
#define PRINT_LOG(x) std::cout << ">> " << x << std::endl
#define ERROR_LOG(x) std::cout << "ERROR: " << x << std::endl


void APIENTRY glDebugOutput(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam);

void EnableDebug();
