#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vs_path, const std::string& fs_path)
{
    std::string vs_source = ReadFile(vs_path);
    std::string fs_source = ReadFile(fs_path);

    GLuint vs = CompileShader(vs_source.c_str(), GL_VERTEX_SHADER);
    GLuint fs = CompileShader(fs_source.c_str(), GL_FRAGMENT_SHADER);

    m_program = glCreateProgram();
    glAttachShader(m_program, vs);
    glAttachShader(m_program, fs);
    glLinkProgram(m_program);

    GLint did_link;
    glGetProgramiv(m_program, GL_LINK_STATUS, &did_link);
    if (did_link == GL_FALSE)
    {
        GLint len = 0;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &len);

        std::vector<GLchar> infoLog(len);
        glGetProgramInfoLog(m_program, len, &len, &infoLog[0]);

        std::cerr << "Shader program linking failed:\n" << &infoLog[0] << std::endl;

        glDeleteShader(vs);
        glDeleteShader(fs);
        glDeleteProgram(m_program);
        throw std::runtime_error("Shader program linking failed");
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader()
{
    glDeleteProgram(m_program);
}

Shader::Shader(Shader&& other)
{
    m_program = other.m_program;
    other.m_program = 0;
}

Shader& Shader::operator=(Shader&& other)
{
    if (this != &other)
    {
        glDeleteProgram(m_program);
        m_program = other.m_program;
        other.m_program = 0;
    }
    return *this;
}

void Shader::Bind() const
{
    glUseProgram(m_program);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform(const std::string& name, const glm::mat4& matrix)
{
    GLuint loc = glGetUniformLocation(m_program, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetUniform(const std::string& name, const int value)
{
    GLuint loc = glGetUniformLocation(m_program, name.c_str());
    glUniform1i(loc, value);
}

GLuint Shader::CompileShader(const char* src, GLenum type) const
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GLint did_compile;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &did_compile);
    if (did_compile == GL_FALSE)
    {
        GLint len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

        std::vector<GLchar> infoLog(len);
        glGetShaderInfoLog(shader, len, &len, &infoLog[0]);

        std::cerr << "Shader compilation failed:\n" << &infoLog[0] << std::endl;

        glDeleteShader(shader);
        throw std::runtime_error("Shader compilation failed");
    }

    return shader;
}

std::string Shader::ReadFile(const std::string& path) const
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}