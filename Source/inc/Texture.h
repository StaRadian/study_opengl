#pragma once

#include <string>

class Texture
{
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Wight, m_Height, m_BPP;   //x, y, bit per pixel
public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;     //window have 32 texture slot
    void Unbind() const;

    inline int GetWidth() const { return m_Wight; }
    inline int GetHeight() const { return m_Height; }
};