#pragma once

#include "PCH.hpp"

#include "Common/TextureCommon.hpp"

namespace AE
{
    class Texture
    {
    public:
        Texture(
            GLuint id = 0,
            const TextureDesc& descriptor = TextureDesc(),
            GLenum target = GL_TEXTURE_2D,
            bool generateMipmaps = false
        );
        
        virtual ~Texture();

        static std::shared_ptr<Texture> Create(const TextureData& data, bool generateMipmaps = false);

        void Bind(int slot = 0) const;
        void Unbind(int slot = 0) const;

        void Resize(int width, int height);

        void GenerateMipmaps();

        static std::shared_ptr<Texture> GetDefault();

        const TextureDesc& GetDescriptor() const;

        GLuint GetID() const;
        int GetWidth() const;
        int GetHeight() const;
        int GetChannels() const;
        TextureFilter GetMinFilter() const;
        TextureFilter GetMagFilter() const;
        TextureWrap GetWrapS() const;
        TextureWrap GetWrapT() const;

        bool HasTransparency() const;
        bool HasMipmaps() const;
        
        bool IsValid() const;

        void SetMinFilter(TextureFilter filter);
        void SetMagFilter(TextureFilter filter);
        void SetWrapS(TextureWrap wrap);
        void SetWrapT(TextureWrap wrap);

    protected:

        virtual void AllocateStorage(int width, int height, GLenum internalFormat, GLenum format);
        
        GLenum target;
        TextureDesc desc;
        
    private:
        GLuint _id;
        bool _hasMipmaps = false;
        bool _hasTransparency = false;
    };
}