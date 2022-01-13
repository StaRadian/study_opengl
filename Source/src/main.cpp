#include <GL/glew.h>    //GLEW 라이브러리
#include <GLFW/glfw3.h> //GLFW 라이브러리
#include <iostream> //for LOG
#include <fstream>  //for ParseShader
#include <string>   //for getline
#include <sstream>  

#include <filesystem>   //경로 표시


#define LOG(x) std::cout << x << std::endl

static void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}

static void GLCheakError()
{
    while(GLenum error = glGetError())
    {
        LOG("[OpenGL Error] (" << std::hex << error << ")");
    }
}

struct ShaderProgramSources
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSources ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    LOG("Shader Path:" << std::filesystem::absolute(filepath));
    while(getline(stream, line))
    {
        if(line.find("#shader") != std::string::npos)   //npos: 찾지 못하였을때 line.find 반환값
        {
            if(line.find("vertex") != std::string::npos)    //set mode vertex
                type = ShaderType::VERTEX;
            else if(line.find("fragment") != std::string::npos) //set mode fragment
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return  { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string source)
{
    unsigned int id = glCreateShader(type); //create empty shader object
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);    //Shader compile

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);  //
    if(result == GL_FALSE)  //failed compiled shader
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetShaderInfoLog(id, length, &length, message);
        LOG("failed to compiled "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader!");
        LOG(message);
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();   //create empty program object
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs); //program에 vs shader를 연결
    glAttachShader(program, fs); //program에 fs shader를 연결
    glLinkProgram(program);     //link program
    glValidateProgram(program); //유효성 검사 program 지정, 개발용으로만 유용

    glDeleteShader(vs); //delete shader vs 
    glDeleteShader(fs); //delete shader fs

    return program;
}

int main(void)  //main 함수
{
    LOG("study_opengl Version 0.0.11");
    LOG("Shader Path:" << std::filesystem::current_path());
    GLFWwindow* window;
    

    if (!glfwInit())    //GLFW 초기화
        return -1;

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)    //window == 0, 종료
    {
        glfwTerminate();    //GLFW 종료
        LOG("Failed CreateWindow");
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)     //GLEW 초기화
    {
        glfwTerminate();    //GLEW 종료
        LOG("GLEW_ERROR");
        return -1;
    }

    LOG(glGetString(GL_VERSION));   //OpenGL 버전 체크
    
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

    unsigned int buffer;    //버퍼 ID
    glGenBuffers(1,&buffer);    //버퍼 생성
    glBindBuffer(GL_ARRAY_BUFFER, buffer);     //array buffer 탐색 bind = selecting
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);   //buffer data 생성 or 초기화 
    //STATIC: 데이터 저장소 한번 수정 여러번 사용, DRAW 데이터 저장소 application에 의해 수정됨 그리기와 이미지 지정 명령에

    glEnableVertexAttribArray(0);   //VertexAttribArray Index:0 Enable
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (const void*)0);    //index = 0, size: 2d, type, 

    unsigned int ibo;    //Index buffer array ID
    glGenBuffers(1,&ibo);    //IBO 버퍼 생성
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);     //target: Vertex array indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);   //buffer data 생성 or 초기화 
    //STATIC: 데이터 저장소 한번 수정 여러번 사용, DRAW 데이터 저장소 application에 의해 수정됨 그리기와 이미지 지정 명령에


    ShaderProgramSources source = ParseShader("../res/shader/Basic.shader");

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);   //Installs a program object as part of current rendering state

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        GLClearError();
        glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr);    // mode: 삼각형, count: Indices 수, type: Indices type
        GLCheakError();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();    //GLFW 종료
    LOG("Close Window");
    return 0;
}