#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <glad/gl.h>

#include <glm/fwd.hpp>

class Shader
{
public:
    Shader(const std::string& vs_path, const std::string& fs_path);
    ~Shader();

    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;

    Shader(Shader&& other);
    Shader& operator=(Shader&& other);

    void Bind() const;
    void Unbind() const;

    void SetUniform(const std::string& name, const glm::mat4& matrix);
    void SetUniform(const std::string& name, const int value);

private:
    GLuint CompileShader(const char* src, GLenum type) const;
    std::string ReadFile(const std::string& path) const;
private:
    GLuint m_program;
};

#endif // SHADER_HPP