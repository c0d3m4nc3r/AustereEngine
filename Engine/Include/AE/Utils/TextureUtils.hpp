#pragma once

#include "PCH.hpp"

namespace AE::TextureUtils
{
    inline GLenum GetFormat(int channels)
    {
        switch (channels)
        {
            case 1: return GL_RED;
            case 2: return GL_RG;
            case 3: return GL_RGB;
            case 4: return GL_RGBA;
            default: return 0;
        }
    }

    inline GLenum GetInternalFormat(int channels)
    {
        switch (channels)
        {
            case 1: return GL_R8;
            case 2: return GL_RG8;
            case 3: return GL_RGB8;
            case 4: return GL_RGBA8;
            default: return 0;
        }
    }
}
