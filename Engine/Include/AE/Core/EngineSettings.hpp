#pragma once

#include "PCH.hpp"

#include "Rendering/Color.hpp"

#include "Resources/Texture.hpp"

namespace AE
{
    enum class MSAALevel : int
    {
        None = 0,
        X2 = 2,
        X4 = 4,
        X8 = 8,
        X16 = 16
    };

    enum class AnisotropicLevel : int
    {
        None = 1,
        X2 = 2,
        X4 = 4,
        X8 = 8,
        X16 = 16
    };

    class EngineSettings
    {
    public:
        
        EngineSettings(const EngineSettings&) = delete;
        EngineSettings& operator=(const EngineSettings&) = delete;

        struct WindowSettings {
            std::string title = "Austere Engine";
            int width = 800;
            int height = 600;
            bool fullscreen = false;
            bool resizable = true;
            bool borderless = false;
            bool vsync = true;
        } window;

        struct GraphicsSettings {
            TextureFilter defaultMinFilter = TextureFilter::LinearMipmapLinear;
            TextureFilter defaultMagFilter = TextureFilter::Linear;
            bool generateMipmaps = true;

            MSAALevel msaaLevel = MSAALevel::None;
            AnisotropicLevel anisotropyLevel = AnisotropicLevel::None;
        } graphics;

        struct RendererSettings {
            AE::Color clearColor = AE::Color(0.1f, 0.1f, 0.1f, 1.0f);
            bool enableDepthTest = true;
            bool enableFaceCulling = true;
        } renderer;

        static EngineSettings& Get()
        {
            static EngineSettings settings;
            return settings;
        }

        // void Load(const std::string& filePath);
        // void Save(const std::string& filePath);
    private:
        
        EngineSettings() = default;
    };
};
