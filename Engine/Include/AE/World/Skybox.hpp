#pragma once

namespace AE
{
    class Cubemap;
    class Mesh;
    class Skybox
    {
    public:

        /* Constructor & Destructor */

        Skybox(std::shared_ptr<Cubemap> cubemap, std::shared_ptr<Mesh> mesh = nullptr);
        ~Skybox();

        /* Public Members */
        
        std::shared_ptr<Cubemap> cubemap;
        std::shared_ptr<Mesh> mesh;

    };
}
