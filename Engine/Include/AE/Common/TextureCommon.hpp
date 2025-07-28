#pragma once

#include "PCH.hpp"

namespace AE
{
    enum class TextureFilter
    {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,
        NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
        LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
        NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
        LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
    };

    enum class TextureWrap
    {
        Repeat = GL_REPEAT,
        MirroredRepeat = GL_MIRRORED_REPEAT,
        ClampToEdge = GL_CLAMP_TO_EDGE,
        ClampToBorder = GL_CLAMP_TO_BORDER
    };

    enum class TextureFormat
    {
        R = GL_RED,
        RG = GL_RG,
        RGB = GL_RGB,
        RGBA = GL_RGBA,
        Depth = GL_DEPTH_COMPONENT,
        Stencil = GL_STENCIL_INDEX,

        R8 = GL_R8,
        RG8 = GL_RG8,
        RGB8 = GL_RGB8,
        RGBA8 = GL_RGBA8,

        R16 = GL_R16,
        RG16 = GL_RG16,
        RGBA16 = GL_RGBA16,
        R16F = GL_R16F,
        RG16F = GL_RG16F,
        RGB16F = GL_RGB16F,
        RGBA16F = GL_RGBA16F,

        R32F = GL_R32F,
        RG32F = GL_RG32F,
        RGB32F = GL_RGB32F,
        RGBA32F = GL_RGBA32F,

        RGB10_A2 = GL_RGB10_A2,
        R11F_G11F_B10F = GL_R11F_G11F_B10F,
        Depth24 = GL_DEPTH_COMPONENT24,
        Depth32F = GL_DEPTH_COMPONENT32F,
        Depth24_Stencil8 = GL_DEPTH24_STENCIL8
    };

    struct TextureData
    {
        unsigned char* pixels = nullptr;
        int width = 0;
        int height = 0;
        int channels = 4;
        GLenum type = GL_UNSIGNED_BYTE;
    };

    struct TextureDesc
    {
        int width = 0;
        int height = 0;
        int channels = 4;
        GLenum type = GL_UNSIGNED_BYTE;
        TextureFormat internalFormat = TextureFormat::RGBA;
        TextureFormat format = TextureFormat::RGBA;
        TextureFilter minFilter = TextureFilter::Linear;
        TextureFilter magFilter = TextureFilter::Linear;
        TextureWrap wrapS = TextureWrap::Repeat;
        TextureWrap wrapT = TextureWrap::Repeat;
        TextureWrap wrapR = TextureWrap::Repeat;
    };
}
