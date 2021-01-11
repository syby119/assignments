#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const std::string& vsCode, const std::string& fsCode)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode = vsCode;
        std::string fragmentCode = fsCode;

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use()
    {
        glUseProgram(ID);
    }

    /*
     * @brief set bool uniform variable to shader
     * @param name name of the variable
     * @param value bool value to be pass to shader
     */
    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
    }


    /*
     * @brief set int uniform variable to shader
     * @param name name of the variable
     * @param value int value to be pass to shader
     */
    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }


    /*
     * @brief set float uniform variable to shader
     * @param name name of the variable
     * @param value float value to be pass to shader
     */
    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }


    /*
     * @brief set vec2 uniform variable to shader
     * @param name name of the variable
     * @param v2 vec2 to be pass to shader
     */
    void setVec2(const std::string& name, const glm::vec2& v2) const {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &v2[0]);
    }


    /*
     * @brief set vec3 uniform variable to shader
     * @param name name of the variable
     * @param v3 vec3 to be pass to shader
     */
    void setVec3(const std::string& name, const glm::vec3& v3) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &v3[0]);
    }


    /*
     * @brief set vec4 uniform variable to shader
     * @param name name of the variable
     * @param v4 vec4 to be pass to shader
     */
    void setVec4(const std::string& name, const glm::vec3& v4) const {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &v4[0]);
    }


    /*
     * @brief set mat3 uniform variable to shader
     * @param name name of the variable
     * @param value mat3 value to be pass to shader
     */
    void setMat3(const std::string& name, const glm::mat3& mat3) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &mat3[0][0]);
    }


    /*
     * @brief set mat4 uniform variable to shader
     * @param name name of the variable
     * @param value mat4 value to be pass to shader
     */
    void setMat4(const std::string& name, const glm::mat4& mat4) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat4[0][0]);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
#endif