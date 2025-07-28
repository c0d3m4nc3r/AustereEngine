#pragma once

#include "Rendering/Mesh.hpp"
#include "Resources/Cubemap.hpp"

const float skyboxVertices[] = {
    -1.0f,  1.0f, -1.0f,  -1.0f, -1.0f, -1.0f,   1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,   1.0f,  1.0f, -1.0f,  -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,  -1.0f, -1.0f, -1.0f,  -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,  -1.0f,  1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,   1.0f, -1.0f,  1.0f,   1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,   1.0f,  1.0f, -1.0f,   1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,   1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,   1.0f, -1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,   1.0f,  1.0f, -1.0f,   1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,  -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,  -1.0f, -1.0f,  1.0f,   1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,  -1.0f, -1.0f,  1.0f,   1.0f, -1.0f,  1.0f
};

namespace AE
{
    class Skybox
    {
    public:

        /* Constructor & Destructor */

        Skybox(std::shared_ptr<Cubemap> cubemap, std::shared_ptr<Mesh> mesh = nullptr) : cubemap(cubemap)
        {
            if (!mesh)
            {
                std::vector<glm::vec3> vertices;
                vertices.reserve(36);

                for (int i = 0; i < 36; i++)
                {
                    vertices.push_back(glm::vec3(
                        skyboxVertices[i * 3], 
                        skyboxVertices[i * 3 + 1], 
                        skyboxVertices[i * 3 + 2]
                    ));
                }
                
                this->mesh = std::make_shared<Mesh>(vertices);
            }
        }

        ~Skybox() = default;

        /* Public Members */
        
        std::shared_ptr<Cubemap> cubemap;
        std::shared_ptr<Mesh> mesh;

    };
}
