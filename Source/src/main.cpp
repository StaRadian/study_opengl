#include <GL/glew.h>    //GLEW 라이브러리
#include <GLFW/glfw3.h> //GLFW 라이브러리
#include <iostream> //for LOG
#include <fstream>  //for ParseShader
#include <string>   //for getline
#include <sstream>  

#include <filesystem>   //경로 표시

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

static void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while(GLenum error = glGetError())
    {
        LOG("[OpenGL Error] (0x" << std::setw(4) << std::setfill('0') << std::hex << error << "): "     //에러 코드 출력
            << function << " " << file << ":" << std::dec << line);                                                 
        return false;
    }
    return true;
}

struct ShaderProgramSources
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSources ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    LOG("Shader Path:" << std::filesystem::absolute(filepath)); //Shader 주소
    if(stream.fail())
    {
        LOG("Shader file not found!");
    }
    else
    {
        LOG("Shader file found!");
    }

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
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
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));    //Shader compile

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));  //
    if(result == GL_FALSE)  //failed compiled shader
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char message[length];
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        LOG("failed to compiled "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader!");
        LOG(message);
        GLCall(glDeleteShader(id));
        return 0;
    }
    else
    {
        LOG("succeed to compiled "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader!");
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();   //create empty program object
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs)); //program에 vs shader를 연결
    GLCall(glAttachShader(program, fs)); //program에 fs shader를 연결
    GLCall(glLinkProgram(program));     //link program
    GLCall(glValidateProgram(program)); //유효성 검사 program 지정, 개발용으로만 유용

    GLCall(glDeleteShader(vs)); //delete shader vs 
    GLCall(glDeleteShader(fs)); //delete shader fs

    return program;
}

int main(void)  //main 함수
{
    LOG("study_opengl Version 0.0.24");     //#11 Uniforms in OpenGL
    LOG("Current_Path:" << std::filesystem::current_path()); //파일 주소
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

    glfwSwapInterval(1);    //60fps

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
    GLCall(glGenBuffers(1,&buffer));    //버퍼 생성
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));     //array buffer 탐색 bind = selecting
    GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW));   //buffer data 생성 or 초기화 
    //STATIC: 데이터 저장소 한번 수정 여러번 사용, DRAW 데이터 저장소 application에 의해 수정됨 그리기와 이미지 지정 명령에

    GLCall(glEnableVertexAttribArray(0));   //VertexAttribArray Index:0 Enable
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (const void*)0));    //index = 0, size: 2d, type, 

    unsigned int ibo;    //Index buffer array ID
    GLCall(glGenBuffers(1,&ibo));    //IBO 버퍼 생성
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));     //target: Vertex array indices
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));   //buffer data 생성 or 초기화 
    //STATIC: 데이터 저장소 한번 수정 여러번 사용, DRAW 데이터 저장소 application에 의해 수정됨 그리기와 이미지 지정 명령에

    //ShaderProgramSources source = ParseShader("./Source/res/shader/Basic.shader");
    ShaderProgramSources source = ParseShader("../res/shader/Basic.shader");

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));   //Installs a program object as part of current rendering state

    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);     //Uniform 탐색 실패
    GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
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

    GLCall(glDeleteProgram(shader));

    glfwTerminate();    //GLFW 종료
    LOG("Close Window");
    return 0;
}