#include "Resources/Texture.hpp"
#include "Core/EngineSettings.hpp"
#include "Core/Logger.hpp"
#include "Utils/TextureUtils.hpp"

#include "PCH.hpp"

namespace AE
{
    Texture::Texture(
        GLuint id,
        const TextureDesc& descriptor,
        GLenum target,
        bool generateMipmaps
    ) : _id(id), desc(descriptor), target(target)
    {
        if (_id != 0 && generateMipmaps)
        {
            GenerateMipmaps();
        }
    }

    Texture::~Texture()
    {
        if (_id != 0)
        {
            glDeleteTextures(1, &_id);
        }
    }

    std::shared_ptr<Texture> Texture::Create(const TextureData& data, bool generateMipmaps)
    {
        LoggerContext ctx("Texture", "Create");

        GLenum format = TextureUtils::GetFormat(data.channels);
        GLenum internalFormat = TextureUtils::GetInternalFormat(data.channels);

        if (!format || !internalFormat)
        {
            Logger::Error("Unsupported number of channels!");
            return nullptr;
        }

        bool hasTransparency = false;
        if (data.channels == 4 && data.pixels)
        {
            size_t pixelCount = static_cast<size_t>(data.width) * data.height;
            for (size_t i = 0; i < pixelCount; ++i)
            {
                unsigned char alpha = data.pixels[i * 4 + 3];
                if (alpha < 255)
                {
                    hasTransparency = true;
                    break;
                }
            }
        }

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, data.width, data.height, 0, format, data.type, data.pixels);

        EngineSettings& settings = EngineSettings::Get();

        TextureDesc descriptor = TextureDesc(
            data.width, data.height, data.channels,
            data.type,
            static_cast<TextureFormat>(internalFormat),
            static_cast<TextureFormat>(format),
            settings.graphics.defaultMinFilter,
            settings.graphics.defaultMagFilter
        );

        auto texture = std::make_shared<Texture>(textureID, descriptor, GL_TEXTURE_2D, generateMipmaps);

        texture->_hasTransparency = hasTransparency;

        if (settings.graphics.anisotropyLevel != AnisotropicLevel::None)
        {
            GLfloat maxAniso = 0.0f;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAniso);
            float desiredAniso = std::min(static_cast<GLfloat>(settings.graphics.anisotropyLevel), maxAniso);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, desiredAniso);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(descriptor.minFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(descriptor.magFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(descriptor.wrapS));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(descriptor.wrapT));

        glBindTexture(GL_TEXTURE_2D, 0);

        return texture;
    }

    void Texture::Bind(int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(target, _id);
    }

    void Texture::Unbind(int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(target, 0);
    }

    void Texture::Resize(int width, int height)
    {
        if (_id != 0)
        glDeleteTextures(1, &_id);

        glGenTextures(1, &_id);
        glBindTexture(target, _id);

        const GLenum internalFormat = TextureUtils::GetInternalFormat(desc.channels);
        const GLenum format = TextureUtils::GetFormat(desc.channels);

        AllocateStorage(width, height, internalFormat, format);

        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(desc.minFilter));
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(desc.magFilter));
        glTexParameteri(target, GL_TEXTURE_WRAP_S, static_cast<GLint>(desc.wrapS));
        glTexParameteri(target, GL_TEXTURE_WRAP_T, static_cast<GLint>(desc.wrapT));

        desc.width = width;
        desc.height = height;
    }

    void Texture::GenerateMipmaps()
    {
        if (_id == 0) return;

        Bind();
        
        glGenerateMipmap(target);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            Logger::Error("OpenGL error: {}", error);
        }

        _hasMipmaps = true;
    }

    std::shared_ptr<Texture> Texture::GetDefault()
    {
        static std::shared_ptr<Texture> defaultTexture = []() -> std::shared_ptr<Texture>
        {
            const int width = 16;
            const int height = 16;
            const int channels = 3;
            const int tileSize = 4;

            std::vector<unsigned char> pixels(width * height * channels);

            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    int i = (y * width + x) * channels;
                    bool isBlack = ((x / tileSize) + (y / tileSize)) % 2 == 0;

                    if (isBlack)
                    {
                        pixels[i + 0] = 0;
                        pixels[i + 1] = 0;
                        pixels[i + 2] = 0;
                    }
                    else
                    {
                        pixels[i + 0] = 255;
                        pixels[i + 1] = 0;
                        pixels[i + 2] = 255;
                    }
                }
            }

            auto& settings = EngineSettings::Get();
            return Texture::Create({pixels.data(), width, height, channels}, settings.graphics.generateMipmaps);
        }();

        return defaultTexture;
    }
    const TextureDesc& Texture::GetDescriptor() const { return desc; }

    GLuint Texture::GetID() const { return _id; }
    int Texture::GetWidth() const { return desc.width; }
    int Texture::GetHeight() const { return desc.height; }
    int Texture::GetChannels() const { return desc.channels; }
    TextureFilter Texture::GetMinFilter() const { return desc.minFilter; }
    TextureFilter Texture::GetMagFilter() const { return desc.magFilter; }
    TextureWrap Texture::GetWrapS() const { return desc.wrapS; }
    TextureWrap Texture::GetWrapT() const { return desc.wrapT; }

    bool Texture::HasTransparency() const { return _hasTransparency; }
    bool Texture::HasMipmaps() const { return _hasMipmaps; }
    
    bool Texture::IsValid() const { return _id != 0; }

    void Texture::SetMinFilter(TextureFilter filter)
    {
        desc.minFilter = filter;
        glBindTexture(target, _id);
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter));
    }

    void Texture::SetMagFilter(TextureFilter filter)
    {
        desc.magFilter = filter;
        glBindTexture(target, _id);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter));
    }

    void Texture::SetWrapS(TextureWrap wrap)
    {
        desc.wrapS = wrap;
        glBindTexture(target, _id);
        glTexParameteri(target, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap));
    }

    void Texture::SetWrapT(TextureWrap wrap)
    {
        desc.wrapT = wrap;
        glBindTexture(target, _id);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap));
    }

    void Texture::AllocateStorage(int width, int height, GLenum internalFormat, GLenum format)
    {
        glTexImage2D(target, 0, internalFormat, width, height, 0, format, desc.type, nullptr);
    }
}