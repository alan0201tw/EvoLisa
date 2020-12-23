#pragma once

#include <string>

#include "stb_image.h"

namespace ycel
{
    class Image
    {
    public:
        Image(const std::string& path);
        ~Image();

        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }
        u_char* GetData() const { return m_Data; }

        int PixelDifferenceMSE(u_char* reference) const;

    private:
        uint32_t m_Width, m_Height;
        stbi_uc* m_Data;
    };
}