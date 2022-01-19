#include "IndexBuffer.h"

#include <GL/glew.h>    //GLEW 라이브러리
#include "Debug.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint)); //unsigned int == GLuint?

    GLCall(glGenBuffers(1,&m_RendererID));    //버퍼 생성
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));     //array buffer 탐색 bind = selecting
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));   //buffer data 생성 or 초기화 
    //STATIC: 데이터 저장소 한번 수정 여러번 사용

}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));   //1개의 버퍼를 삭제
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));    //Bind m_RendererID Buffer
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));   //unbind buffer
}