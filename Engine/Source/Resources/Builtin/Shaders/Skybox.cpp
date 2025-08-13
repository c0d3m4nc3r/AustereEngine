#include "Resources/Builtin/Shaders.hpp"

namespace AE::Builtin::Shaders
{
    const std::string skyboxVertex = R"(
        #version 330 core

        layout (location = 0) in vec3 aPosition;

        out vec3 TexCoord;

        uniform mat4 u_ProjectionMatrix;
        uniform mat4 u_ViewMatrix;

        void main()
        {
            TexCoord = aPosition;

            vec4 pos = u_ProjectionMatrix * u_ViewMatrix * vec4(aPosition, 1.0);
            gl_Position = pos.xyww;
        }  
    )";
    
    const std::string skyboxFragment = R"(
        #version 330 core

        in vec3 TexCoord;
        out vec4 FragColor;

        uniform samplerCube u_Cubemap;

        void main()
        {
            FragColor = texture(u_Cubemap, TexCoord);
        }
    )";
}
