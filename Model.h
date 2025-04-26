#ifndef MODEL_H
#define MODEL_H

#include "glad.h"
#include "./glm/glm/glm.hpp"
#include <string>
#include <vector>
using namespace std;
using namespace glm;

class Model {
private:
    vector<vec3> vertices;     
    vector<vec3> normals;     
    vector<vec2> texCoords;    
    vector<unsigned int> indices;   

    unsigned int VAO;   
    unsigned int VBO;    
    unsigned int NBO;    
    unsigned int TBO;    
    unsigned int EBO;    

public:

    Model(const char* path);
    ~Model();
    bool loadModel(const char* path);
    void Draw();
private:
    void setupMesh();
};

#endif 