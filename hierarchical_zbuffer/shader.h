#pragma once

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

class Shader {
public:
    /*
     * @brief constructor, take string as shader code to create opengl shader
     */
    Shader(const char* vsCode, const char* fsCode);
    
    /*
     * @brief constructor, read shader code from file to create opengl shader
     */
    Shader(const std::string& vsFilepath, const std::string& fsFilepath);

    /*
     * @brief move constructor
     */
    Shader(Shader&& shader) noexcept;

    /*
     * @brief destructor
     */
    ~Shader();

    /*
     * @brief use current shader for object rendering
     */
    void use();

    /*
     * @brief set bool uniform variable to shader
     */
    void setBool(const std::string& name, bool value) const;

    /*
     * @brief set int uniform variable to shader
     */
    void setInt(const std::string& name, int value) const;

    /*
     * @brief set float uniform variable to shader
     */
    void setFloat(const std::string& name, float value) const;

    /*
     * @brief set vec2 uniform variable to shader
     */
    void setVec2(const std::string& name, const glm::vec2& v2) const;

    /*
     * @brief set vec2 uniform variable to shader
     */
    void setVec3(const std::string& name, const glm::vec3& v3) const;

    /*
     * @brief set vec2 uniform variable to shader
     */
    void setVec4(const std::string& name, const glm::vec4& v4) const;

    /*
     * @brief set vec2 uniform variable to shader
     */
    void setMat3(const std::string& name, const glm::mat3& mat3) const;

    /*
     * @brief set vec2 uniform variable to shader
     */
    void setMat4(const std::string& name, const glm::mat4& mat4) const;

private:
    /* shader program handle */
    GLuint _id = 0;

    /*
     * @brief read shader code from file
     */
    std::string _readFile(const std::string& filePath);

    /*
     * @brief create a vertex / fragment shader
     */
    GLuint _createShader(const std::string& code, GLenum shaderType);

    /*
     * @brief create a shader program
     */
    void _createShaderProgram(const std::string& vsCode, const std::string& fsCode);
};