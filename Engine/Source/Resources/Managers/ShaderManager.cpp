#include "Resources/Managers.hpp"
#include "Resources/Shader.hpp"

#include <fstream>

namespace AE
{
    ShaderManager::ShaderManager()
        : ResourceManager("Shader") {}

    static std::string ReadFile(const std::string& path);
    static GLuint CompileShader(GLenum type, const std::string& source);
    static GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);

    std::shared_ptr<Shader> ShaderManager::Load(const std::string& name,
        const std::string& vertexPath, const std::string& fragmentPath)
    {
        LoggerContext ctx("ShaderManager", "Load");

        if (Has(name))
        {
            Logger::Error("Shader with name '{}' already exists!", name);
            return Get(name);
        }

        if (vertexPath.empty() || fragmentPath.empty())
        {
            Logger::Error("Invalid params! Paths cannot be empty!");
            return nullptr;
        }

        Logger::Info("Loading shader '{}' from: ", name);
        Logger::Info("\tVertex: '{}'", vertexPath);
        Logger::Info("\tFragment: '{}'", fragmentPath);

        const std::string vertexSrc = ReadFile(vertexPath);
        const std::string fragmentSrc = ReadFile(fragmentPath);

        if (vertexSrc.empty() || fragmentSrc.empty())
            return nullptr;

        GLuint vertexID = CompileShader(GL_VERTEX_SHADER, vertexSrc);
        GLuint fragmentID = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);
    
        if (vertexID == 0 || fragmentID == 0)
            return nullptr;

        const GLuint programID = LinkProgram(vertexID, fragmentID);
    
        glDeleteShader(vertexID);
        glDeleteShader(fragmentID);

        if (!programID) return nullptr;

        auto shader = std::make_shared<Shader>(programID);

        Add(name, shader);

        Logger::Info("Shader '{}' loaded successfully! (ID = {})", name, programID);

        return shader;
    }

    std::shared_ptr<Shader> ShaderManager::LoadFromSource(const std::string& name,
        const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        LoggerContext ctx("ShaderManager", "Load");

        if (Has(name))
        {
            Logger::Error("Shader with name '{}' already exists!", name);
            return Get(name);
        }

        if (vertexSrc.empty() || fragmentSrc.empty())
        {
            Logger::Error("Invalid parameters! Shader source can't be empty!");
            return nullptr;
        }

        Logger::Info("Loading shader '{}' from source...", name);

        GLuint vertexID = CompileShader(GL_VERTEX_SHADER, vertexSrc);
        GLuint fragmentID = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

        if (vertexID == 0 || fragmentID == 0)
            return nullptr;

        const GLuint programID = LinkProgram(vertexID, fragmentID);
    
        glDeleteShader(vertexID);
        glDeleteShader(fragmentID);

        if (!programID) return nullptr;

        auto shader = std::make_shared<Shader>(programID);

        Add(name, shader);

        Logger::Info("Shader '{}' loaded successfully! (ID = {})", name, programID);

        return shader;
    }

    static std::string ReadFile(const std::string& path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file)
        {
            Logger::Error("Failed to open file: '{}'", path);
            return "";
        }
    
        std::ostringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    static GLuint CompileShader(GLenum type, const std::string& source)
    {
        const std::string typeStr = type == GL_VERTEX_SHADER ? "Vertex" : "Fragment";
        
        Logger::Debug("Compiling {} shader...", typeStr);
        
        GLuint shader = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);
    
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            GLint logLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
            std::string infoLog(logLength, '\0');
            glGetShaderInfoLog(shader, logLength, nullptr, infoLog.data());
            
            Logger::Error("{} shader compilation failed: {}", typeStr, infoLog);
            
            glDeleteShader(shader);
            return 0;
        }

        Logger::Debug("{} shader compiled! (ID = {})", typeStr, shader);
    
        return shader;
    }
    
    static GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader)
    {
        Logger::Debug("Linking shader program...");

        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
    
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            GLint logLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
            std::string infoLog(logLength, '\0');
            glGetProgramInfoLog(program, logLength, nullptr, infoLog.data());
    
            Logger::Error("Shader linking failed: {}", infoLog);
            
            glDeleteProgram(program);
            return 0;
        }

        Logger::Debug("Shader program linked! (ID = {})", program);
    
        return program;
    }
}
