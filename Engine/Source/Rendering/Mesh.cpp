#include "Rendering/Mesh.hpp"
#include "Core/Logger.hpp"

namespace AE
{
    Mesh::Mesh(
        const std::vector<glm::vec3>& vertices,
        const std::vector<GLuint>& indices,
        const std::vector<glm::vec3>& normals,
        const std::vector<glm::vec2>& texCoords,
        const std::vector<glm::vec3>& tangents,
        const std::vector<glm::vec3>& bitangents
    )
        : _vao(0), _ebo(0),
          _vertices(vertices),
          _indices(indices),
          _normals(normals),
          _texCoords(texCoords),
          _tangents(tangents),
          _bitangents(bitangents)
    {
        for (int i = 0; i < VBO_COUNT; ++i)
            _vbos[i] = 0;
    
        Setup();
    }
    
    Mesh::~Mesh()
    {
        if (_vao) glDeleteVertexArrays(1, &_vao);
        if (_ebo) glDeleteBuffers(1, &_ebo);
        for (int i = 0; i < VBO_COUNT; ++i)
        {
            if (_vbos[i]) glDeleteBuffers(1, &_vbos[i]);
        }
    }
    
    void Mesh::Bind() const
    {
        glBindVertexArray(_vao);
    }
    
    void Mesh::Unbind() const
    {
        glBindVertexArray(0);
    }
    
    void Mesh::Setup()
    {
        LoggerContext ctx("Mesh", "Setup");
    
        if (!HasVertices()) {
            Logger::Error("Mesh has no vertices!");
            return;
        }
    
        if (_vao != 0)
        {
            glDeleteVertexArrays(1, &_vao);
            _vao = 0;
    
            for (int i = 0; i < VBO_COUNT; ++i)
            {
                if (_vbos[i] != 0)
                {
                    glDeleteBuffers(1, &_vbos[i]);
                    _vbos[i] = 0;
                }
            }
    
            if (_ebo != 0)
            {
                glDeleteBuffers(1, &_ebo);
                _ebo = 0;
            }
        }
    
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
    
        // VERTICES
        glGenBuffers(1, &_vbos[VERTICES]);
        glBindBuffer(GL_ARRAY_BUFFER, _vbos[VERTICES]);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), _vertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
        // NORMALS
        if (HasNormals()) {
            glGenBuffers(1, &_vbos[NORMALS]);
            glBindBuffer(GL_ARRAY_BUFFER, _vbos[NORMALS]);
            glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(glm::vec3), _normals.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        } else {
            glVertexAttrib3f(1, 0.0f, 0.0f, 1.0f);
        }
    
        // TEXCOORDS
        if (HasTexCoords()) {
            glGenBuffers(1, &_vbos[TEXCOORDS]);
            glBindBuffer(GL_ARRAY_BUFFER, _vbos[TEXCOORDS]);
            glBufferData(GL_ARRAY_BUFFER, _texCoords.size() * sizeof(glm::vec2), _texCoords.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        } else {
            glVertexAttrib2f(2, 0.0f, 0.0f);
        }
    
        // TANGENTS
        if (HasTangents()) {
            glGenBuffers(1, &_vbos[TANGENTS]);
            glBindBuffer(GL_ARRAY_BUFFER, _vbos[TANGENTS]);
            glBufferData(GL_ARRAY_BUFFER, _tangents.size() * sizeof(glm::vec3), _tangents.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        } else {
            glVertexAttrib3f(3, 1.0f, 0.0f, 0.0f);
        }
    
        // BITANGENTS
        if (HasBitangents()) {
            glGenBuffers(1, &_vbos[BITANGENTS]);
            glBindBuffer(GL_ARRAY_BUFFER, _vbos[BITANGENTS]);
            glBufferData(GL_ARRAY_BUFFER, _bitangents.size() * sizeof(glm::vec3), _bitangents.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        } else {
            glVertexAttrib3f(4, 0.0f, 1.0f, 0.0f);
        }
    
        // INDICES
        if (HasIndices()) {
            glGenBuffers(1, &_ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW);
        }
    
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            Logger::Error("OpenGL error: {}", error);
        }
    }
    
    void Mesh::Draw(GLenum mode)
    {
        if (_vao == 0 || _vertices.empty())
            return;
    
        glBindVertexArray(_vao);
    
        if (HasIndices()) {
            glDrawElements(mode, _indices.size(), GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(mode, 0, _vertices.size());
        }
    
        glBindVertexArray(0);
    }

    const AABB& Mesh::GetAABB() const { return _aabb; }
    void Mesh::SetAABB(const AABB& aabb) { _aabb = aabb; }
    
    const std::vector<glm::vec3>& Mesh::GetVertices() const { return _vertices; }
    const std::vector<glm::vec3>& Mesh::GetNormals() const { return _normals; }
    const std::vector<glm::vec2>& Mesh::GetTexCoords() const { return _texCoords; }
    const std::vector<glm::vec3>& Mesh::GetTangents() const { return _tangents; }
    const std::vector<glm::vec3>& Mesh::GetBitangents() const { return _bitangents; }
    const std::vector<GLuint>&    Mesh::GetIndices() const { return _indices; }
    
    std::size_t Mesh::GetVerticesCount() const   { return _vertices.size(); }
    std::size_t Mesh::GetNormalsCount() const    { return _normals.size(); }
    std::size_t Mesh::GetTexCoordsCount() const  { return _texCoords.size(); }
    std::size_t Mesh::GetTangentsCount() const   { return _tangents.size(); }
    std::size_t Mesh::GetBitangentsCount() const { return _bitangents.size(); }
    std::size_t Mesh::GetIndicesCount() const    { return _indices.size(); }
    
    bool Mesh::HasVertices() const   { return !_vertices.empty(); }
    bool Mesh::HasNormals() const    { return !_normals.empty(); }
    bool Mesh::HasTexCoords() const  { return !_texCoords.empty(); }
    bool Mesh::HasTangents() const   { return !_tangents.empty(); }
    bool Mesh::HasBitangents() const { return !_bitangents.empty(); }
    bool Mesh::HasIndices() const    { return !_indices.empty(); }
    
    void Mesh::SetVertices(const std::vector<glm::vec3>& vertices)
    {
        _vertices = vertices;
    
        if (_vbos[VERTICES]) {
            glBindBuffer(GL_ARRAY_BUFFER, _vbos[VERTICES]);
            glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), _vertices.data(), GL_STATIC_DRAW);
        }
    }
    
    void Mesh::SetNormals(const std::vector<glm::vec3>& normals)
    {
        _normals = normals;
    
        if (_vbos[NORMALS]) {
            glBindBuffer(GL_ARRAY_BUFFER, _vbos[NORMALS]);
            glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(glm::vec3), _normals.data(), GL_STATIC_DRAW);
        }
    }
    
    void Mesh::SetTexCoords(const std::vector<glm::vec2>& texCoords)
    {
        _texCoords = texCoords;
    
        if (_vbos[TEXCOORDS]) {
            glBindBuffer(GL_ARRAY_BUFFER, _vbos[TEXCOORDS]);
            glBufferData(GL_ARRAY_BUFFER, _texCoords.size() * sizeof(glm::vec2), _texCoords.data(), GL_STATIC_DRAW);
        }
    }
    
    void Mesh::SetTangents(const std::vector<glm::vec3>& tangents)
    {
        _tangents = tangents;
    
        if (_vbos[TANGENTS]) {
            glBindBuffer(GL_ARRAY_BUFFER, _vbos[TANGENTS]);
            glBufferData(GL_ARRAY_BUFFER, _tangents.size() * sizeof(glm::vec3), _tangents.data(), GL_STATIC_DRAW);
        }
    }
    
    void Mesh::SetBitangents(const std::vector<glm::vec3>& bitangents)
    {
        _bitangents = bitangents;
    
        if (_vbos[BITANGENTS]) {
            glBindBuffer(GL_ARRAY_BUFFER, _vbos[BITANGENTS]);
            glBufferData(GL_ARRAY_BUFFER, _bitangents.size() * sizeof(glm::vec3), _bitangents.data(), GL_STATIC_DRAW);
        }
    }
    
    void Mesh::SetIndices(const std::vector<GLuint>& indices)
    {
        _indices = indices;
    
        if (_ebo) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW);
        }
    }
}
