#include "Shader.h"
#include <iostream>
#include <fstream>
#include "../glm/gtc/type_ptr.hpp"

bool Shader::compileShader(const char *filepath, unsigned int shaderType)
{
    std::ifstream *file = new std::ifstream(filepath, std::ios::ate | std::ios::binary);

    if (!file->is_open())
        return false;

    size_t shaderSize = (size_t)file->tellg();
    char *shaderCode = new char[shaderSize + 1];
    shaderCode[shaderSize] = 0;
    file->seekg(0);
    file->read(shaderCode, shaderSize);
    file->close();

    delete file;

    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        return false;
    }

    delete[] shaderCode;

    if (shaderType == GL_VERTEX_SHADER)
        vertexShader = shader;
    else
        fragmentShader = shader;

    return true;
}

bool Shader::linkShaders()
{
    program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    int success;
    char infoLog[512];

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << infoLog << '\n';
        return false;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

bool Shader::autoCompileAndLink(const char *vertexShaderFilepath, const char *fragmentShaderFilepath)
{
    return compileShader(vertexShaderFilepath, GL_VERTEX_SHADER) &&
           compileShader(fragmentShaderFilepath, GL_FRAGMENT_SHADER) &&
           linkShaders();
}

void Shader::use()
{
    glUseProgram(program);
}

void Shader::setInt(const char *name, int t)
{
    use();
    glUniform1i(glGetUniformLocation(program, name), t);
}

void Shader::setFloat(const char* name, float t)
{
    glUniform1f(glGetUniformLocation(program, name),t);
}

void Shader::setVec3(const char* name, const glm::vec3& vec3)
{
    use();
    glUniform3fv(glGetUniformLocation(program, name),1, glm::value_ptr(vec3));
}

void Shader::setMat4(const char *name, const glm::mat4 &mat4)
{
    use();
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, glm::value_ptr(mat4));
}

Shader::~Shader()
{
    glDeleteProgram(program);
}

Shader::Shader()
{
    vertexShader = fragmentShader = program = -1;
}
