#include "Resources/Managers.hpp"
#include "Resources/Texture.hpp"
#include "Core/EngineSettings.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace AE
{
    TextureManager::TextureManager()
        : ResourceManager("Texture") {}

    std::shared_ptr<Texture> TextureManager::Load(const std::string& name,
        const std::string& path)
    {
        LoggerContext ctx("TextureManager", "Load<file>");

        if (Has(name))
        {
            Logger::Error("Texture with name '{}' already exists!", name);
            return Get(name);
        }

        if (path.empty())
        {
            Logger::Error("Invalid params! Path cannot be empty!");
            return Texture::GetDefault();
        }

        Logger::Info("Loading texture '{}' from: '{}'", name, path);

        int width, height, channels;
        unsigned char* pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!pixels)
        {
            Logger::Error("Failed to load texture data: {}", stbi_failure_reason());
            return Texture::GetDefault();
        }

        auto& settings = EngineSettings::Get();

        auto texture = Texture::Create({pixels, width, height, channels}, settings.graphics.generateMipmaps);
        if (!texture)
        {
            Logger::Error("Failed to create texture!");
            stbi_image_free(pixels);
            return Texture::GetDefault();
        }

        stbi_image_free(pixels);

        Add(name, texture);

        Logger::Info("Texture '{}' loaded successfully! (ID = {}, Size = ({}x{}), Channels = {})",
            name, texture->GetID(), width, height, channels);

        return texture;
    }

    std::shared_ptr<Texture> TextureManager::Load(const std::string& name,
        unsigned char* data, size_t size, bool compressed,
        int width, int height, int channels)
    {
        LoggerContext ctx("TextureManager", "Load<memory>");

        if (Has(name))
        {
            Logger::Error("Texture with name '{}' already exists!", name);
            return Get(name);
        }

        if (!data || !size)
        {
            Logger::Error("Invalid params! Texture data cannot be null or empty!");
            return Texture::GetDefault();
        }

        Logger::Info("Loading texture '{}' from: (Address: 0x{:X}, Size: {} bytes)",
            name, reinterpret_cast<std::uintptr_t>(data), size);

        unsigned char* pixels = nullptr;
        bool shouldFree = false;

        if (compressed)
        {
            pixels = stbi_load_from_memory(
                static_cast<const stbi_uc*>(data),
                static_cast<int>(size),
                &width, &height, &channels, 0
            );
            if (!pixels)
            {
                Logger::Error("Failed to load texture data: {}", stbi_failure_reason());
                return Texture::GetDefault();
            }
            shouldFree = true;
        } else {
            if (width <= 0 || height <= 0 || channels < 1 || channels > 4)
            {
                Logger::Error("Invalid texture dimensions or channels!");
                return Texture::GetDefault();
            }
            pixels = data;
        }

        auto& settings = EngineSettings::Get();

        auto texture = Texture::Create({pixels, width, height, channels}, settings.graphics.generateMipmaps);
        if (!texture)
        {
            Logger::Error("Failed to create texture!");
            if (shouldFree) stbi_image_free(pixels);
            return Texture::GetDefault();
        }

        if (shouldFree) stbi_image_free(pixels);

        Add(name, texture);

        Logger::Info("Texture '{}' loaded successfully! (ID = {}, Size = ({}x{}), Channels = {})",
            name, texture->GetID(), width, height, channels);

        return texture;
    }
}
