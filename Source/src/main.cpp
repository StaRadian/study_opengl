#include <GL/glew.h>    //GLEW 라이브러리
#include <GLFW/glfw3.h> //GLFW 라이브러리
#include <iostream> //for LOG
#include <filesystem>   //경로 표시

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void)  //main 함수
{
    LOG("study_opengl Version 0.0.24");     //#11 Uniforms in OpenGL
    LOG("Current_Path:" << std::filesystem::current_path()); //파일 주소
    GLFWwindow* window;

    if (!glfwInit())    //GLFW 초기화
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  //opengl 메이저 버전 v4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);  //opengl 마이너 버전
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)    //window == 0, 종료
    {
        glfwTerminate();    //GLFW 종료
        LOG("Failed CreateWindow");
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);    //60fps

    if (glewInit() != GLEW_OK)     //GLEW 초기화
    {
        glfwTerminate();    //GLEW 종료
        LOG("GLEW_ERROR");
        return -1;
    }

    LOG(glGetString(GL_VERSION));   //OpenGL 버전 체크
    
    {
        float positions[] = {   //x,y
            0.5f,  0.5f,       //0
            0.5f, -0.5f,       //1
            -0.5f, -0.5f,       //2
            -0.5f,  0.5f        //3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));     //vertex array object 이름 생성
        GLCall(glBindVertexArray(vao));         //bind a vertex array object

        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        Shader shader(
            #ifdef DEBUG
                "../res/shader/Basic.shader"
            #else
                "./Source/res/shader/Basic.shader"
            #endif
            );
            
        shader.Bind();
        shader.SetUniform4f("u_Color",  0.8f, 0.3f, 0.8f, 1.0f);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        float r = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            shader.Bind();
            shader.SetUniform4f("u_Color",  r, 0.3f, 0.8f, 1.0f);

            va.Bind();
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));    // mode: 삼각형, count: Indices 수, type: Indices type

            if(r > 1.0f)
                increment = -0.05f;
            else if(r < 0.0f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }

    }

    glfwTerminate();    //GLFW 종료
    LOG("Close Window");
    return 0;
}