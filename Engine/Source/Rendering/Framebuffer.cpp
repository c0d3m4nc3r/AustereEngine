#include "Rendering/Framebuffer.hpp"
#include "Resources/Texture.hpp"
#include "Resources/TextureArray.hpp"
#include "Core/Logger.hpp"

namespace AE
{
    Framebuffer::Framebuffer(GLuint id, int width, int height)
        : _id(id), _width(width), _height(height) 
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    Framebuffer::~Framebuffer()
    {
        if (_depthRenderbuffer != 0)
            glDeleteRenderbuffers(1, &_depthRenderbuffer);

        if (_id != 0)
            glDeleteFramebuffers(1, &_id);
    }

    std::shared_ptr<Framebuffer> Framebuffer::Create(int width, int height)
    {
        GLuint id;
        glGenFramebuffers(1, &id);
        return std::make_shared<Framebuffer>(id, width, height);
    }

    void Framebuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _id);
    }

    void Framebuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::Resize(int width, int height)
    {
        if (width == _width && height == _height)
            return;

        _width = width;
        _height = height;

        glBindFramebuffer(GL_FRAMEBUFFER, _id);

        for (auto& attachment : _colorAttachments)
        {
            attachment.texture->Resize(width, height);
            glFramebufferTexture2D(
                GL_FRAMEBUFFER, 
                GL_COLOR_ATTACHMENT0 + attachment.slot, 
                GL_TEXTURE_2D, 
                attachment.texture->GetID(), 
                0
            );
        }

        for (auto& attachment : _colorArrayAttachments)
        {
            attachment.textureArray->Resize(width, height);
            glFramebufferTextureLayer(
                GL_FRAMEBUFFER, 
                GL_COLOR_ATTACHMENT0 + attachment.slot, 
                attachment.textureArray->GetID(), 
                0, 
                attachment.layer
            );
        }

        if (_depthAttachment)
        {
            _depthAttachment->texture->Resize(width, height);
            glFramebufferTexture2D(
                GL_FRAMEBUFFER, 
                GL_DEPTH_ATTACHMENT, 
                GL_TEXTURE_2D, 
                _depthAttachment->texture->GetID(), 
                0
            );
        }
        else if (_depthArrayAttachment)
        {
            _depthArrayAttachment->textureArray->Resize(width, height);
            glFramebufferTextureLayer(
                GL_FRAMEBUFFER, 
                GL_DEPTH_ATTACHMENT, 
                _depthArrayAttachment->textureArray->GetID(), 
                0, 
                _depthArrayAttachment->layer
            );
        }
        else if (_depthMode == DepthMode::Renderbuffer && _depthRenderbuffer != 0)
        {
            glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderbuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }

        _UpdateDrawBuffers();
        _CheckCompleteness();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::AttachColorTexture(std::shared_ptr<Texture> texture, GLenum slot)
    {
        if (!texture) return;

        LoggerContext ctx("Framebuffer", "AttachColorTexture");

        if (texture->GetWidth() != _width || texture->GetHeight() != _height)
        {
            Logger::Warning("Texture size does not match framebuffer dimensions!");
        }

        _colorAttachments.erase(std::remove_if(_colorAttachments.begin(), _colorAttachments.end(),
            [slot](const auto& a) { return a.slot == slot; }), _colorAttachments.end());

        _colorArrayAttachments.erase(std::remove_if(_colorArrayAttachments.begin(), _colorArrayAttachments.end(),
            [slot](const auto& a) { return a.slot == slot; }), _colorArrayAttachments.end());

        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, 
            GL_COLOR_ATTACHMENT0 + slot, 
            GL_TEXTURE_2D, 
            texture->GetID(), 
            0
        );

        _colorAttachments.push_back({texture, slot});

        _UpdateDrawBuffers();
        _CheckCompleteness();
  
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::AttachColorTextureLayer(std::shared_ptr<TextureArray> textureArray, int layer, GLenum slot)
    {
        if (!textureArray) return;

        LoggerContext ctx("Framebuffer", "AttachColorTextureLayer");

        if (textureArray->GetWidth() != _width || textureArray->GetHeight() != _height)
        {
            Logger::Warning("TextureArray size does not match framebuffer dimensions!");
        }

        _colorAttachments.erase(std::remove_if(_colorAttachments.begin(), _colorAttachments.end(),
            [slot](const auto& a) { return a.slot == slot; }), _colorAttachments.end());

        _colorArrayAttachments.erase(std::remove_if(_colorArrayAttachments.begin(), _colorArrayAttachments.end(),
            [slot](const auto& a) { return a.slot == slot; }), _colorArrayAttachments.end());

        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        glFramebufferTextureLayer(
            GL_FRAMEBUFFER, 
            GL_COLOR_ATTACHMENT0 + slot, 
            textureArray->GetID(), 
            0, 
            layer
        );

        _colorArrayAttachments.push_back({textureArray, slot, layer});

        _UpdateDrawBuffers();
        _CheckCompleteness();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::AttachDepthTexture(std::shared_ptr<Texture> texture)
    {
        if (!texture) return;

        LoggerContext ctx("Framebuffer", "AttachDepthTexture");

        if (texture->GetWidth() != _width || texture->GetHeight() != _height)
        {
            Logger::Warning("Texture size does not match framebuffer dimensions!");
        }

        _depthAttachment.reset();
        _depthArrayAttachment.reset();

        if (_depthRenderbuffer != 0)
        {
            glDeleteRenderbuffers(1, &_depthRenderbuffer);
            _depthRenderbuffer = 0;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, 
            GL_DEPTH_ATTACHMENT, 
            GL_TEXTURE_2D, 
            texture->GetID(), 
            0
        );

        _depthAttachment = DepthAttachment{texture};
        _depthMode = DepthMode::Texture;
        _CheckCompleteness();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::AttachDepthTextureLayer(std::shared_ptr<TextureArray> textureArray, int layer)
    {
        if (!textureArray) return;

        LoggerContext ctx("Framebuffer", "AttachDepthTextureLayer");

        if (textureArray->GetWidth() != _width || textureArray->GetHeight() != _height)
        {
            Logger::Warning("TextureArray size does not match framebuffer dimensions!");
        }

        _depthAttachment.reset();
        _depthArrayAttachment.reset();

        if (_depthRenderbuffer != 0)
        {
            glDeleteRenderbuffers(1, &_depthRenderbuffer);
            _depthRenderbuffer = 0;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        glFramebufferTextureLayer(GL_FRAMEBUFFER, 
                                GL_DEPTH_ATTACHMENT, 
                                textureArray->GetID(), 
                                0, 
                                layer);

        _depthArrayAttachment = DepthArrayAttachment{textureArray, layer};
        _depthMode = DepthMode::Texture;
        _CheckCompleteness();
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::AttachDepthRenderbuffer()
    {
        _depthAttachment.reset();
        _depthArrayAttachment.reset();

        if (_depthRenderbuffer == 0)
            glGenRenderbuffers(1, &_depthRenderbuffer);

        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _width, _height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        _depthMode = DepthMode::Renderbuffer;
        _CheckCompleteness();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::DetachColor(GLenum slot)
    {
        bool changed = false;

        auto colorIt = std::remove_if(_colorAttachments.begin(), _colorAttachments.end(),
            [slot](const auto& a) { return a.slot == slot; });
        
        if (colorIt != _colorAttachments.end())
        {
            _colorAttachments.erase(colorIt, _colorAttachments.end());
            changed = true;
        }

        auto arrayIt = std::remove_if(_colorArrayAttachments.begin(), _colorArrayAttachments.end(),
            [slot](const auto& a) { return a.slot == slot; });
        
        if (arrayIt != _colorArrayAttachments.end())
        {
            _colorArrayAttachments.erase(arrayIt, _colorArrayAttachments.end());
            changed = true;
        }

        if (changed)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, _id);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, 0, 0);
            _UpdateDrawBuffers();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }

    void Framebuffer::DetachDepth()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _id);

        if (_depthAttachment || _depthArrayAttachment)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
            _depthAttachment.reset();
            _depthArrayAttachment.reset();
        }
        else if (_depthRenderbuffer != 0)
        {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
            glDeleteRenderbuffers(1, &_depthRenderbuffer);
            _depthRenderbuffer = 0;
        }

        _depthMode = DepthMode::None;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint Framebuffer::GetID() const { return _id; }
    int Framebuffer::GetWidth() const { return _width; }
    int Framebuffer::GetHeight() const { return _height; }

    std::shared_ptr<Texture> Framebuffer::GetColorTexture(GLenum slot) const
    {
        for (const auto& attachment : _colorAttachments)
        {
            if (attachment.slot == slot)
                return attachment.texture;
        }
        return nullptr;
    }

    std::shared_ptr<TextureArray> Framebuffer::GetColorTextureArray(GLenum slot) const
    {
        for (const auto& attachment : _colorArrayAttachments)
        {
            if (attachment.slot == slot)
                return attachment.textureArray;
        }
        return nullptr;
    }

    int Framebuffer::GetColorTextureLayer(GLenum slot) const
    {
        for (const auto& attachment : _colorArrayAttachments)
        {
            if (attachment.slot == slot)
                return attachment.layer;
        }
        return -1;
    }

    std::shared_ptr<Texture> Framebuffer::GetDepthTexture() const
    {
        return _depthAttachment ? _depthAttachment->texture : nullptr;
    }

    std::shared_ptr<TextureArray> Framebuffer::GetDepthTextureArray() const
    {
        return _depthArrayAttachment ? _depthArrayAttachment->textureArray : nullptr;
    }

    int Framebuffer::GetDepthTextureLayer() const
    {
        return _depthArrayAttachment ? _depthArrayAttachment->layer : -1;
    }

    bool Framebuffer::IsComplete() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return status == GL_FRAMEBUFFER_COMPLETE;
    }

    void Framebuffer::SetDrawBuffer(GLenum buffer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        glDrawBuffer(buffer);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::SetReadBuffer(GLenum buffer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        glReadBuffer(buffer);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::_UpdateDrawBuffers()
    {
        if (_colorAttachments.empty() && _colorArrayAttachments.empty())
        {
            glDrawBuffer(GL_NONE);
            return;
        }

        std::vector<GLenum> drawBuffers;
        
        for (const auto& a : _colorAttachments)
            drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + a.slot);
        
        for (const auto& a : _colorArrayAttachments)
            drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + a.slot);

        std::sort(drawBuffers.begin(), drawBuffers.end());
        
        glDrawBuffers(static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data());
    }

    void Framebuffer::_CheckCompleteness()
    {
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            Logger::Error("Framebuffer is incomplete! Status: " + std::to_string(status));
        }
    }
}