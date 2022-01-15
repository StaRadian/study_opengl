#include "Renderer.h"

#include <iostream>


void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while(GLenum error = glGetError())
    {
        LOG("[OpenGL Error] (" << std::hex << error << "): "     //에러 코드 출력
            << function << " " << file << ":" << std::dec << line);                                                 
        return false;
    }
    return true;
}