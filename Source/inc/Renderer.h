#ifndef RENDERER
    #define RENDERER

    #include <GL/glew.h>    //GLEW 라이브러리

    #if DEBUG
        #define LOG(x) std::cout << x << std::endl
        #define ASSERT(x) if (!(x)) __debugbreak();
        #define GLCall(x) GLClearError();\
            x;\
            ASSERT(GLLogCall(#x, __FILE__, __LINE__))
    #else
        #define LOG(x) 
        #define GLCall(x) x
        #define ASSERT(x)
    #endif

    void GLClearError();

    bool GLLogCall(const char* function, const char* file, int line);

#endif