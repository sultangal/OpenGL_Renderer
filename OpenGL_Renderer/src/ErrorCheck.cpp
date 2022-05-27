#include "pch.h"
#include "ErrorCheck.h"

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        switch (error)
        {
        case 1280:  std::cout << red << "[OpenGL][ERROR]::(" << error << ")(" << "GL_INVALID_ENUM" << "): \n";
            break;                                      
        case 1281:  std::cout << red << "[OpenGL][ERROR]::(" << error << ")(" << "GL_INVALID_VALUE" << "): \n";
            break;                                      
        case 1282:  std::cout << red << "[OpenGL][ERROR]::(" << error << ")(" << "GL_INVALID_OPERATION" << "): \n";
            break;                                      
        case 1283:  std::cout << red << "[OpenGL][ERROR]::(" << error << ")(" << "GL_STACK_OVERFLOW" << "): \n";
            break;                                      
        case 1284:  std::cout << red << "[OpenGL][ERROR]::(" << error << ")(" << "GL_STACK_UNDERFLOW" << "): \n";
            break;                                     
        case 1285:  std::cout << red << "[OpenGL][ERROR]::(" << error << ")(" << "GL_OUT_OF_MEMORY" << "): \n";
            break;                                      
        case 1286:  std::cout << red << "[OpenGL][ERROR]::(" << error << ")(" << "GL_INVALID_FRAMEBUFFER_OPERATION" << "): \n";
            break;                                      
        default:    std::cout << red << "[OpenGL][ERROR]::(" << error << ")(" << error << "): \n";
            break;
        }
        std::cout << "\t Function: " << function << "\n\t File: " << file << ":" << line << white << std::endl;
        return false;
    }
    return true;
}
