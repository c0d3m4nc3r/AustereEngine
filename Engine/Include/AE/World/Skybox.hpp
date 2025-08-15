#pragma once

namespace AE
{
    class Cubemap;
    class Mesh;
    class Skybox
    {
    public:

        /* Constructor & Destructor */

        Skybox(SPtr<Cubemap> cubemap, SPtr<Mesh> mesh = nullptr);
        ~Skybox();

        /* Public Members */
        
        SPtr<Cubemap> cubemap;
        SPtr<Mesh> mesh;

    };
}
