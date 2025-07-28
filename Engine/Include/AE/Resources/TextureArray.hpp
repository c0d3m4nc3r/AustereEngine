#pragma once

#include "PCH.hpp"

#include "Resources/Texture.hpp"

namespace AE
{
    class TextureArray : public Texture
    {
    public:
        
        TextureArray(GLuint id, const TextureDesc& desc, int layers);

        static std::shared_ptr<TextureArray> Create(const TextureDesc& desc, int layers);

        int GetLayerCount() const;
        
    protected:

        void AllocateStorage(int width, int height, GLenum internalFormat, GLenum format) override;

    private:

        int _layers;
        
    };
}
