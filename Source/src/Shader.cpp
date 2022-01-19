#include "Shader.h"

#include <iostream> //for LOG
#include <fstream>  //for ParseShader
#include <string>   //for getline

#include <GL/glew.h>    //GLEW 라이브러리

#include <filesystem>   //경로 표시
#include "Debug.h"

Shader::Shader(const std::string& filepath)
    : m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSources source = ParseShader(filepath);

    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSources Shader::ParseShader(const std::string& filepath)
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

unsigned int Shader::CompileShader(unsigned int type, const std::string source)
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

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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


void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
    
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if(location == -1)   //Uniform 탐색 실패
        LOG("Warring: Uniform '" << name << "' doesn't exist!");
    
    m_UniformLocationCache[name] = location;
    return location;
}