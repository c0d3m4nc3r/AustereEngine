#include "Resources/Shader.hpp"
#include "Core/Logger.hpp"

namespace AE
{
    Shader::Shader(GLuint id) : _id(id) {}
    
    Shader::~Shader()
    {
        if (_id == 0)
            glDeleteProgram(_id);
    }
    
    void Shader::Bind() const
    {
        glUseProgram(_id);
    }
    
    void Shader::Unbind() const
    {
        glUseProgram(0);
    }
    
    GLuint Shader::GetID() const { return _id; }
    
    GLint Shader::GetUniformLocation(const std::string& name)
    {
        LoggerContext ctx("Shader", "GetUniformLocation");
    
        if (_id == 0)
        {
            Logger::Warning("Attempted to get uniform '{}' from an invalid shader program (ID = 0)", name);
            return -1;
        }
    
        if (_uniforms.find(name) != _uniforms.end())
            return _uniforms[name];
    
        GLint location = glGetUniformLocation(_id, name.c_str());
        if (location == -1)
        {
            Logger::Warning("Uniform '{}' not found in shader program {}", name, _id);
        }
    
        _uniforms[name] = location;
        return location;
    }

    bool Shader::IsValid() const { return _id != 0; }
    
    void Shader::SetInt(const std::string& name, int value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }
    
    void Shader::SetFloat(const std::string& name, float value)
    {
        glUniform1f(GetUniformLocation(name), value);
    }
    
    void Shader::SetBool(const std::string& name, bool value)
    {
        glUniform1i(GetUniformLocation(name), value ? 1 : 0);
    }
    
    void Shader::SetVec2(const std::string& name, const glm::vec2& value)
    {
        glUniform2fv(GetUniformLocation(name), 1, &value[0]);
    }
    
    void Shader::SetVec3(const std::string& name, const glm::vec3& value)
    {
        glUniform3fv(GetUniformLocation(name), 1, &value[0]);
    }
    
    void Shader::SetVec4(const std::string& name, const glm::vec4& value)
    {
        glUniform4fv(GetUniformLocation(name), 1, &value[0]);
    }
    
    void Shader::SetMat3(const std::string& name, const glm::mat3& value)
    {
        glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
    }
    
    void Shader::SetMat4(const std::string& name, const glm::mat4& value)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
    }
}