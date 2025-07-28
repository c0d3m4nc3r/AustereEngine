#include "Game/Game.hpp"

#include <AE/Core/Engine.hpp>

int main(int argc, char* argv[])
{
    AE::Engine engine;
    
    auto info = AE::ApplicationInfo{
        .name = "Austere Game",
        .version = "0.0.1",
        .author = "c0d3m4nc3r",
        .description = "Game for testing Austere Engine."
    };

    engine.SetApplication(std::make_unique<AustereGame>(info));
    return engine.Run(argc, argv);
}
