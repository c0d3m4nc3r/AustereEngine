#include "Resources/TextureArray.hpp"
#include "Utils/TextureUtils.hpp"

namespace AE
{
    TextureArray::TextureArray(GLuint id, const TextureDesc& desc, int layers)
        : Texture(id, desc, GL_TEXTURE_2D_ARRAY) {}

    std::shared_ptr<TextureArray> TextureArray::Create(const TextureDesc& desc, int layers)
    {
        GLuint id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D_ARRAY, id);

        GLenum internalFormat = static_cast<GLenum>(desc.internalFormat);
        GLenum format = static_cast<GLenum>(desc.format);

        glTexImage3D(
            GL_TEXTURE_2D_ARRAY,
            0,
            internalFormat,
            desc.width,
            desc.height,
            layers,
            0,
            format,
            desc.type,
            nullptr
        );

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(desc.minFilter));
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(desc.magFilter));
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, static_cast<GLint>(desc.wrapS));
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, static_cast<GLint>(desc.wrapT));

        // NOTE: ЭТО ПИЗДЕЦ!!!

        if (1)
        {
            float borderColor[] = {1.f, 1.f, 1.f, 1.f};
            glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        } else {
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        }

        // NOTE: ЭТО КОНЕЦ ПИЗДЕЦА!!!

        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

        return std::make_shared<TextureArray>(id, desc, layers);
    }

    int TextureArray::GetLayerCount() const { return _layers; }

    void TextureArray::AllocateStorage(int width, int height, GLenum internalFormat, GLenum format)
    {
        glTexImage3D(target, 0, internalFormat, width, height, _layers, 0, format, desc.type, nullptr);
    }

}
