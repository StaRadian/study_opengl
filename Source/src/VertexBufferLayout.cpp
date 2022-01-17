#include "VertexBufferLayout.h"

template<typename T>
void VertexBufferLayout::Push(unsigned int count)
{
    //static_assert(false);   //컴파일 에러
}

template<>
void VertexBufferLayout::Push<float>(unsigned int count)     //float
{
    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (const void*)0));
    //index: 0, size: 2d, type: float, normalized(if not float): false, stride: float * 2, pointer
    m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });   //push_back: 변수 추가
    m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT);  // 4byte
}

template<> 
void VertexBufferLayout::Push<unsigned int>(unsigned int count)     //unsigned int
{
    m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);  // 4byte
}

template<> 
void VertexBufferLayout::Push<unsigned char>(unsigned int count)     //unsigned byte
{
    m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);  // 1byte
}