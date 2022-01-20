#include <GL/glew.h>    //GLEW 라이브러리
#include <GLFW/glfw3.h> //GLFW 라이브러리
#include <iostream> //for LOG
#include <filesystem>   //경로 표시

#include "Debug.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
        float positions[] = {   //x,y   //s,t
            100.0f,  100.0f,  0.0f,  0.0f,      //0
            200.0f,  100.0f,  1.0f,  0.0f,      //1
            200.0f,  200.0f,  1.0f,  1.0f,      //2
            100.0f,  200.0f,  0.0f,  1.0f       //3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));         //Blending
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));  //GL_SRC_ALPHA: 0, GL_ONE_MINUS_SRC_ALPHA: 1 - 0 = 1

        
        VertexArray va; //VertexArray
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));  //VertexBuffer
        VertexBufferLayout layout;  //VertexBufferLayout
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6); //IndexBuffer

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);    //정사영

        Shader shader(  //Shader
            #ifdef DEBUG
                "../res/shader/Basic.shader"
            #else
                "./Source/res/shader/Basic.shader"
            #endif
            );
            
        shader.Bind();
        //shader.SetUniform4f("u_Color",  0.8f, 0.3f, 0.8f, 1.0f);
        shader.SetUniformMat4f("u_MVP", proj);

        Texture texture(
            #ifdef DEBUG
                "../res/textures/Mistarion_cat.png"
            #else
                "./Source/res/textures/Mistarion_cat.png"
            #endif
            );

        texture.Bind(); //slot: 0
        shader.SetUniform1i("u_Texture", 0);    //slot: 0

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            //shader.Bind();
            //shader.SetUniform4f("u_Color",  r, 0.3f, 0.8f, 1.0f);

            renderer.Draw(va, ib, shader);

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