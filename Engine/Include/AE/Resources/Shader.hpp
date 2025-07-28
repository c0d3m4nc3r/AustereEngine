#pragma once

#include "PCH.hpp"

namespace AE
{
    class Shader
    {
    public:
    
        Shader(GLuint id = 0);
        ~Shader();
    
        void Bind() const;
        void Unbind() const;
    
        GLuint GetID() const;
    
        GLint GetUniformLocation(const std::string& name);
   
        bool IsValid() const;

        void SetInt(const std::string& name, int value);
        void SetFloat(const std::string& name, float value);
        void SetBool(const std::string& name, bool value);
        void SetVec2(const std::string& name, const glm::vec2& value);
        void SetVec3(const std::string& name, const glm::vec3& value);
        void SetVec4(const std::string& name, const glm::vec4& value);
        void SetMat3(const std::string& name, const glm::mat3& value);
        void SetMat4(const std::string& name, const glm::mat4& value);
    
    private:
    
        GLuint _id;
    
        std::unordered_map<std::string, GLint> _uniforms;
    
    };
}