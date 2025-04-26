#ifndef SHADER_H
#define SHADER_H

#include "glad.h"
#include "./glm/glm/glm.hpp"
#include "./glm/glm/gtc/matrix_transform.hpp"
#include "./glm/glm/gtc/type_ptr.hpp"
#include <string>

using namespace std;
using namespace glm;

class Shader {
public:
    // ID của shader program
    unsigned int ID;

    // Constructor
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);

    // Kích hoạt shader
    void use();

    // Các hàm utility để set uniform variables
    void setBool(const string &name, bool value) const;
    void setInt(const string &name, int value) const;
    void setFloat(const string &name, float value) const;
    void setVec3(const string &name, const vec3 &value) const;
    void setVec3(const string &name, float x, float y, float z) const;
    void setMat4(const string &name, const mat4 &mat) const;

private:
    // Hàm utility để kiểm tra lỗi compile/linking shader
    void checkCompileErrors(unsigned int shader, string type);
};

#endif 
