#pragma once

#include "PCH.hpp"

#include <optional>

namespace AE
{
    class Texture;
    class TextureArray;
    class Framebuffer
    {
    public:

        enum class AttachmentType
        {
            Color,
            Depth
            // Stencil,
            // DepthStencil
        };

        enum class DepthMode
        {
            None,
            Renderbuffer,
            Texture
        };

        struct ColorAttachment
        {
            SPtr<Texture> texture;
            GLenum slot = 0;
        };

        struct ColorArrayAttachment
        {
            SPtr<TextureArray> textureArray;
            GLenum slot = 0;
            int layer = 0;
        };

        struct DepthAttachment
        {
            SPtr<Texture> texture;
        };

        struct DepthArrayAttachment
        {
            SPtr<TextureArray> textureArray;
            int layer = 0;
        };

        Framebuffer(GLuint id, int width, int height);
        ~Framebuffer();

        Framebuffer(const Framebuffer&) = delete;
        Framebuffer& operator=(const Framebuffer&) = delete;

        // Framebuffer(Framebuffer&& other) noexcept;
        // Framebuffer& operator=(Framebuffer&& other) noexcept;

        static SPtr<Framebuffer> Create(int width, int height);

        void Bind() const;
        void Unbind() const;

        void Resize(int width, int height);

        void AttachColorTexture(SPtr<Texture> texture, GLenum slot = 0);
        void AttachColorTextureLayer(SPtr<TextureArray> textureArray, int layer, GLenum slot = 0);
        
        void AttachDepthTexture(SPtr<Texture> texture);
        void AttachDepthTextureLayer(SPtr<TextureArray> textureArray, int layer);
        void AttachDepthRenderbuffer();

        void DetachColor(GLenum color);
        void DetachDepth();

        GLuint GetID() const;
        int GetWidth() const;
        int GetHeight() const;

        SPtr<Texture> GetColorTexture(GLenum slot = 0) const;
        SPtr<TextureArray> GetColorTextureArray(GLenum slot = 0) const;
        int GetColorTextureLayer(GLenum slot = 0) const;
        
        SPtr<Texture> GetDepthTexture() const;
        SPtr<TextureArray> GetDepthTextureArray() const;
        int GetDepthTextureLayer() const;

        bool IsComplete() const;

        void SetDrawBuffer(GLenum buffer);
        void SetReadBuffer(GLenum buffer);

    private:

        GLuint _id;
        int _width, _height;

        DepthMode _depthMode = DepthMode::None;
        GLuint _depthRenderbuffer = 0;
        
        std::vector<ColorAttachment> _colorAttachments;
        std::vector<ColorArrayAttachment> _colorArrayAttachments;
        
        std::optional<DepthAttachment> _depthAttachment;
        std::optional<DepthArrayAttachment> _depthArrayAttachment;

        void _UpdateDrawBuffers();
        void _CheckCompleteness();

    };
}
