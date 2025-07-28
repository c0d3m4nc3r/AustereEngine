#include "Resources/Cubemap.hpp"
#include "Core/EngineSettings.hpp"
#include "Utils/TextureUtils.hpp"
#include "Core/Logger.hpp"

namespace AE
{
    Cubemap::Cubemap(
        GLuint id,
        const TextureDesc& descriptor
    ) : _id(id), _desc(descriptor) {}

    Cubemap::~Cubemap()
    {
        if (_id != 0)
        {
            glDeleteTextures(1, &_id);
        }
    }

    std::shared_ptr<Cubemap> Cubemap::Create(const std::array<TextureData, 6> facesData)
    {
        LoggerContext ctx("Cubemap", "Create");

        GLuint cubemapID;
        glGenTextures(0, &cubemapID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

        auto& firstFaceData = facesData[0];

        int width = firstFaceData.width;
        int height = firstFaceData.height;
        int channels = firstFaceData.channels;
        GLenum type = firstFaceData.type;
        GLenum format = TextureUtils::GetFormat(channels);
        GLenum internalFormat = TextureUtils::GetInternalFormat(channels);

        for (int i = 0; i < 6; i++)
        {
            auto& data = facesData[i];

            if (!data.pixels)
            {
                Logger::Error("Side {} data is NULL!", i);
                glDeleteTextures(1, &cubemapID);
                return nullptr;
            }

            if (!format || !internalFormat)
            {
                Logger::Error("Unsupported number of channels in {} texture!", i);
                glDeleteTextures(1, &cubemapID);                
                return nullptr;
            }

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, data.width, data.height, 0, format, GL_UNSIGNED_BYTE, data.pixels);
        }

        EngineSettings& settings = EngineSettings::Get();

        auto descriptor = TextureDesc(
            width, height, channels, type,
            static_cast<TextureFormat>(internalFormat),
            static_cast<TextureFormat>(format),
            TextureFilter::Linear,
            TextureFilter::Linear,
            TextureWrap::ClampToEdge,
            TextureWrap::ClampToEdge,
            TextureWrap::ClampToEdge
        );

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, static_cast<int>(descriptor.minFilter));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, static_cast<int>(descriptor.magFilter));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, static_cast<int>(descriptor.wrapS));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, static_cast<int>(descriptor.wrapT));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, static_cast<int>(descriptor.wrapR));

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            Logger::Error("OpenGL error: {}", error);
        }

        return std::make_shared<Cubemap>(cubemapID, descriptor);
    }

    void Cubemap::Bind() const
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
    }

    void Cubemap::Unbind() const
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    const TextureDesc& Cubemap::GetDescriptor() const { return _desc; }

    GLuint Cubemap::GetID() const { return _id; }
    int Cubemap::GetWidth() const { return _desc.width; }
    int Cubemap::GetHeight() const { return _desc.height; }
    int Cubemap::GetChannels() const { return _desc.channels; }
    TextureFilter Cubemap::GetMinFilter() const { return _desc.minFilter; }
    TextureFilter Cubemap::GetMagFilter() const { return _desc.magFilter; }
    TextureWrap Cubemap::GetWrapS() const { return _desc.wrapS; }
    TextureWrap Cubemap::GetWrapT() const { return _desc.wrapT; }
    TextureWrap Cubemap::GetWrapR() const { return _desc.wrapR; }

    bool Cubemap::IsValid() const { return _id != 0; }

    void Cubemap::SetMinFilter(TextureFilter filter)
    {
        _desc.minFilter = filter;
        glBindTexture(GL_TEXTURE_2D, _id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter));
    }

    void Cubemap::SetMagFilter(TextureFilter filter)
    {
        _desc.magFilter = filter;
        glBindTexture(GL_TEXTURE_2D, _id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter));
    }

    void Cubemap::SetWrapS(TextureWrap wrap)
    {
        _desc.wrapS = wrap;
        glBindTexture(GL_TEXTURE_2D, _id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap));
    }

    void Cubemap::SetWrapT(TextureWrap wrap)
    {
        _desc.wrapT = wrap;
        glBindTexture(GL_TEXTURE_2D, _id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap));
    }

    void Cubemap::SetWrapR(TextureWrap wrap)
    {
        _desc.wrapR = wrap;
        glBindTexture(GL_TEXTURE_2D, _id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, static_cast<GLint>(wrap));
    }

}
