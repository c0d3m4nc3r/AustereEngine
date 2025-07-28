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

        const std::string vertexShaderSrc = ReadFile(vertexPath);
        const std::string fragmentShaderSrc = ReadFile(fragmentPath);

        if (vertexShaderSrc.empty() || fragmentShaderSrc.empty())
            return nullptr;

        GLuint vertexShaderID = CompileShader(GL_VERTEX_SHADER, vertexShaderSrc);
        GLuint fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    
        if (vertexShaderID == 0 || fragmentShaderID == 0)
            return nullptr;

        const GLuint programID = LinkProgram(vertexShaderID, fragmentShaderID);
    
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

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
            
            Logger::Error("{} shader compilation failed: {}", type == GL_VERTEX_SHADER ? "Vertex" : "Fragment", infoLog);
            
            glDeleteShader(shader);
            return 0;
        }
    
        return shader;
    }
    
    static GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader)
    {
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
    
        return program;
    }
}
