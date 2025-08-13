#pragma once

#include "PCH.hpp"

#include "Resources/Texture.hpp"
#include "Common/TextureCommon.hpp"

namespace AE
{
    class Cubemap : public Texture
    {
    public:
        
        Cubemap(
            GLuint id = 0,
            const TextureDesc& descriptor = TextureDesc()
        );

        static std::shared_ptr<Cubemap> Create(const std::array<TextureData, 6> facesData);

    protected:

        void AllocateStorage(int width, int height, GLenum internalFormat, GLenum format) override;

    };
}
