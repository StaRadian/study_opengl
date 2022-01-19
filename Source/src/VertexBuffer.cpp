#include "VertexBuffer.h"

#include <GL/glew.h>    //GLEW 라이브러리
#include "Debug.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1,&m_RendererID));    //버퍼 생성
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));     //array buffer 탐색 bind = selecting
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));   //buffer data 생성 or 초기화 
    //STATIC: 데이터 저장소 한번 수정 여러번 사용

}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));   //1개의 버퍼를 삭제
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));    //Bind m_RendererID Buffer
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));   //unbind buffer
}