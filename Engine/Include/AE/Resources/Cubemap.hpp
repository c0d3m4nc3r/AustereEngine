#pragma once

#include "PCH.hpp"

#include "Common/TextureCommon.hpp"

namespace AE
{
    class Cubemap
    {
    public:
        
        Cubemap(
            GLuint id = 0,
            const TextureDesc& descriptor = TextureDesc()
        );

        ~Cubemap();

        static std::shared_ptr<Cubemap> Create(const std::array<TextureData, 6> facesData);

        void Bind() const;
        void Unbind() const;

        const TextureDesc& GetDescriptor() const;

        GLuint GetID() const;
        int GetWidth() const;
        int GetHeight() const;
        int GetChannels() const;
        TextureFilter GetMinFilter() const;
        TextureFilter GetMagFilter() const;
        TextureWrap GetWrapS() const;
        TextureWrap GetWrapT() const;
        TextureWrap GetWrapR() const;

        bool IsValid() const;

        void SetMinFilter(TextureFilter filter);
        void SetMagFilter(TextureFilter filter);
        void SetWrapS(TextureWrap wrap);
        void SetWrapT(TextureWrap wrap);
        void SetWrapR(TextureWrap wrap);

    private:

        GLuint _id;
        TextureDesc _desc;

    };
}
