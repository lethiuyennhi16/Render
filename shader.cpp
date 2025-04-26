#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "./glm/glm/gtc/type_ptr.hpp"

using namespace std;
using namespace glm;

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {

    string vertexCode;
    string fragmentCode;
    string geometryCode;
    ifstream vShaderFile;
    ifstream fShaderFile;
    ifstream gShaderFile;

    try {
        // Mở file vertex shader
        vShaderFile.open(vertexPath);
        stringstream vShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        vShaderFile.close();
        vertexCode = vShaderStream.str();

        // Mở file fragment shader
        fShaderFile.open(fragmentPath);
        stringstream fShaderStream;
        fShaderStream << fShaderFile.rdbuf();
        fShaderFile.close();
        fragmentCode = fShaderStream.str();

        // Mở file geometry shader 
        if (geometryPath != nullptr) {
            gShaderFile.open(geometryPath);
            stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (ifstream::failure& e) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryPath != nullptr ? geometryCode.c_str() : nullptr;

   
    unsigned int vertex, fragment, geometry;

    // Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // Fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // Geometry shader (nếu có)
    if (geometryPath != nullptr) {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }

    // Shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    if (geometryPath != nullptr) {
        glAttachShader(ID, geometry);
    }
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath != nullptr) {
        glDeleteShader(geometry);
    }
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const string &name, int value) const {
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        cout << "Warning: Uniform '" << name << "' not found in shader" << endl;
    }
    glUniform1i(location, value);
}

void Shader::setFloat(const string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const string &name, const vec3 &value) const {
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        cout << "Warning: Uniform '" << name << "' not found in shader" << endl;
    }
    glUniform3fv(location, 1, &value[0]);
}

void Shader::setVec3(const string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setMat4(const string &name, const mat4 &mat) const {
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        cout << "Warning: Uniform '" << name << "' not found in shader" << endl;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(mat));
}

void Shader::checkCompileErrors(unsigned int shader, string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog 
                 << "\n -- --------------------------------------------------- -- " << endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog 
                 << "\n -- --------------------------------------------------- -- " << endl;
        }
    }
}
