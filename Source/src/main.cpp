#include <GL/glew.h>    //GLEW 라이브러리
#include <GLFW/glfw3.h> //GLFW 라이브러리
#include <iostream> //for LOG
#include <filesystem>   //경로 표시

#include "Debug.h"

#include "Renderer.h"

#include "TestClearColor.h"
#include "TestTexture2D.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void)  //main 함수
{
    LOG("study_opengl Version 0.0.24");     //#11 Uniforms in OpenGL
    LOG("Current_Path:" << std::filesystem::current_path()); //파일 주소
    GLFWwindow* window;

    if (!glfwInit())    //GLFW 초기화
        return -1;

    const char* glsl_version = "#version 460";  //130
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  //opengl 메이저 버전 v4.6   //3.0
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

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    LOG(glGetString(GL_VERSION));   //OpenGL 버전 체크
    
    {
        GLCall(glEnable(GL_BLEND));         //Blending
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));  //GL_SRC_ALPHA: 0, GL_ONE_MINUS_SRC_ALPHA: 1 - 0 = 1

        Renderer renderer;
 
        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu -> RegisterTest<test::TestClearColor>("Clear Color");
        testMenu -> RegisterTest<test::TestTexture2D>("2D Texture");

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f))
            /* Render here */
            renderer.Clear();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if(currentTest)
            {
                currentTest -> OnUpdate(0.0f);
                currentTest -> OnRender();
                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest -> OnImGuiRender();
                ImGui::End();
            }

            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
        delete currentTest;
        if(currentTest != testMenu)
            delete testMenu;
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();    //GLFW 종료
    LOG("Close Window");
    return 0;
}