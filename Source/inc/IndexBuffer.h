#pragma once
class IndexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Count;
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;  //객체 내부 변수 변경 불가
    void Unbind() const;

    inline unsigned int GetCount() const { return m_Count; }
};
