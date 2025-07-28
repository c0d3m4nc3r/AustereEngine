#include "Resources/Managers.hpp"
#include "Resources/Cubemap.hpp"

#include "stb_image.h"

namespace AE
{
    CubemapManager::CubemapManager()
        : ResourceManager("Cubemap") {}

    std::shared_ptr<Cubemap> CubemapManager::Load(const std::string& name,
        const std::array<std::string, 6>& paths)
    {
        LoggerContext ctx("CubemapManager", "Load");

        if (Has(name))
        {
            Logger::Error("Cubemap with name '{}' already exists!", name);
            return Get(name);
        }

        constexpr const char* cubemapFaces[6] = {
            "X+ (Right)",
            "X- (Left)",
            "Y+ (Top)",
            "Y- (Bottom)",
            "Z+ (Front)",
            "Z- (Back)"
        };

        Logger::Info("Loading cubemap '{}' from: ", name);
        for (size_t i = 0; i < paths.size(); ++i)
        {
            const auto& path = paths[i];
            if (path.empty())
            {
                Logger::Error("Empty path found for cubemap face: {}", cubemapFaces[i]);
                return nullptr;
            }

            Logger::Info("\t{}: '{}'", cubemapFaces[i], path);
        }

        std::array<TextureData, 6> facesData;

        int width, height, channels;
        unsigned char* pixels;

        for (int i = 0; i < 6; i++)
        {
            pixels = stbi_load(paths[i].c_str(), &width, &height, &channels, 0);
            if (!pixels)
            {
                Logger::Error("Failed to load data for face {}: {}", cubemapFaces[i], stbi_failure_reason());
                for (int j = 0; j < i; j++)
                {
                    stbi_image_free(facesData[j].pixels);
                    facesData[j].pixels = nullptr;
                }
                return nullptr;
            };

            facesData[i] = {pixels, width, height, channels};
        }

        auto cubemap = Cubemap::Create(facesData);
        if (!cubemap)
        {
            Logger::Error("Failed to create cubemap!");
            return nullptr;
        }

        for (int i = 0; i < 6; i++)
        {
            stbi_image_free(facesData[i].pixels);
            facesData[i].pixels = nullptr;
        }

        Add(name, cubemap);

        Logger::Info("Cubemap '{}' loaded successfully! (ID = {}, Size = ({}x{}), Channels = {})",
            name, cubemap->GetID(), width, height, channels);

        return cubemap;
    }
}
