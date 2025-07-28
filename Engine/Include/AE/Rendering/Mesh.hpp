#pragma once

#include "PCH.hpp"

#include "Math/AABB.hpp"

namespace AE
{
    class Mesh
    {
    public:
    
        Mesh(
            const std::vector<glm::vec3>& vertices,
            const std::vector<GLuint>& indices = {},
            const std::vector<glm::vec3>& normals = {},
            const std::vector<glm::vec2>& texCoords = {},
            const std::vector<glm::vec3>& tangents = {},
            const std::vector<glm::vec3>& bitangents = {}
        );
        
        ~Mesh();
    
        void Bind() const;
        void Unbind() const;
    
        void Setup();
        void Draw(GLenum mode = GL_TRIANGLES);

        // AABB
        const AABB& GetAABB() const;
        void SetAABB(const AABB& aabb);

        // void CalculateAABB() const;
        
        // Vertices
        const std::vector<glm::vec3>& GetVertices() const;
        void SetVertices(const std::vector<glm::vec3>& vertices);
        std::size_t GetVerticesCount() const;
        bool HasVertices() const;
        
        // Normals
        const std::vector<glm::vec3>& GetNormals() const;
        void SetNormals(const std::vector<glm::vec3>& normals);
        std::size_t GetNormalsCount() const;
        bool HasNormals() const;
        
        // TexCoords
        const std::vector<glm::vec2>& GetTexCoords() const;
        void SetTexCoords(const std::vector<glm::vec2>& texCoords);
        std::size_t GetTexCoordsCount() const;
        bool HasTexCoords() const;
        
        // Tangents
        const std::vector<glm::vec3>& GetTangents() const;
        void SetTangents(const std::vector<glm::vec3>& tangents);
        std::size_t GetTangentsCount() const;
        bool HasTangents() const;
        
        // Bitangents
        const std::vector<glm::vec3>& GetBitangents() const;
        void SetBitangents(const std::vector<glm::vec3>& bitangents);
        std::size_t GetBitangentsCount() const;
        bool HasBitangents() const;
        
        // Indices
        const std::vector<GLuint>& GetIndices() const;
        void SetIndices(const std::vector<GLuint>& indices);
        std::size_t GetIndicesCount() const;
        bool HasIndices() const;
    
    private:
    
        enum VBOs {
            VERTICES,
            NORMALS,
            TEXCOORDS,
            TANGENTS,
            BITANGENTS,
            VBO_COUNT
        };
    
        GLuint _vao, _ebo;
        GLuint _vbos[VBO_COUNT];

        AABB _aabb;
        
        std::vector<glm::vec3> _vertices;
        std::vector<glm::vec3> _normals;
        std::vector<glm::vec2> _texCoords;
        std::vector<glm::vec3> _tangents;
        std::vector<glm::vec3> _bitangents;
        std::vector<GLuint> _indices;
    };
}
