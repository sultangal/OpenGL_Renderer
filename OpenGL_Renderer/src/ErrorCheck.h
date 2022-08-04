#pragma once
#include <GL/glew.h>


#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

#define FRAME_WIDTH 1920
#define FRAME_HEIGHT 1080