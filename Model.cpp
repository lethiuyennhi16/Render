#include "Model.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
using namespace glm;

Model::Model(const char* path) {
    // Khởi tạo các giá trị mặc định cho OpenGL buffers
    VAO = 0;
    VBO = 0;
    NBO = 0;
    TBO = 0;
    EBO = 0;

    if (!loadModel(path)) {
        cout << "ERROR::MODEL::Failed to load model at path: " << path << endl;
    }
}

Model::~Model() {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
    }
    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
    }
    if (NBO != 0) {
        glDeleteBuffers(1, &NBO);
    }
    if (TBO != 0) {
        glDeleteBuffers(1, &TBO);
    }
    if (EBO != 0) {
        glDeleteBuffers(1, &EBO);
    }

    vertices.clear();
    normals.clear();
    texCoords.clear();
    indices.clear();
}

bool Model::loadModel(const char* path) {
    vector<vec3> temp_vertices;
    vector<vec3> temp_normals;
    vector<vec2> temp_texcoords;
    
    vertices.clear();
    normals.clear();
    texCoords.clear();
    indices.clear();

    ifstream file(path);
    if (!file.is_open()) {
        cout << "ERROR::MODEL::Could not open file: " << path << endl;
        return false;
    }

    cout << "Starting to load model from: " << path << endl;
    string line;
    int face_count = 0;
    //debug
    int normal_count = 0;
    while (getline(file, line)) {
        istringstream iss(line);
        string type;
        iss >> type; 
        if (type == "v") {  
            vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            float r, g, b;
            if (iss >> r >> g >> b) {
            }
            
            temp_vertices.push_back(vertex);
        }
        else if (type == "vn") {  // Vertex normal
            vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
            //debug
            normal_count++;
            //cout << "Vertex Normal #" << normal_count << ": (" 
                 //<< normal.x << ", " << normal.y << ", " << normal.z << ")" << endl;
        }
        else if (type == "vt") {  // Texture coordinate
            vec2 texcoord;
            iss >> texcoord.x >> texcoord.y;

            float z;
            if (iss >> z) {
            }
            
            texcoord.y = 1.0f - texcoord.y;
            
            temp_texcoords.push_back(texcoord);
        }
        else if (type == "f") {  // Face
            face_count++;
            string vertex1, vertex2, vertex3;
            iss >> vertex1 >> vertex2 >> vertex3;

            for (const string& vertex : {vertex1, vertex2, vertex3}) {
                istringstream vertex_iss(vertex);
                string index_str;
                vector<int> indices_data;

                while (getline(vertex_iss, index_str, '/')) {
                    if (!index_str.empty()) {
                        indices_data.push_back(stoi(index_str));
                    }
                }

                if (indices_data.size() >= 1) {
                    int vertex_idx = indices_data[0] - 1;
                    vertices.push_back(temp_vertices[vertex_idx]);
                }
                
                if (indices_data.size() >= 2 && !temp_texcoords.empty()) {
                    int texcoord_idx = indices_data[1] - 1;
                    texCoords.push_back(temp_texcoords[texcoord_idx]);
                }
                
                if (indices_data.size() >= 3 && !temp_normals.empty()) {
                    int normal_idx = indices_data[2] - 1;
                    normals.push_back(temp_normals[normal_idx]);
                }

                indices.push_back(indices.size());
            }
        }
    }

    file.close();

    cout << "Model loading statistics:" << endl;
    cout << "Vertices: " << vertices.size() << endl;
    cout << "Normals: " << normals.size() << endl;
    cout << "TexCoords: " << texCoords.size() << endl;
    cout << "Indices: " << indices.size() << endl;
    cout << "Faces: " << face_count << endl;
    cout << "Final normals vector content:" << endl;
    for (size_t i = 0; i < normals.size(); i++) {
        cout << "Normal #" << i + 1 << ": (" 
             << normals[i].x << ", " << normals[i].y << ", " << normals[i].z << ")" << endl;
    }
    setupMesh();

    return true;
}

void Model::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &NBO);
    glGenBuffers(1, &TBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), 
        &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Normals
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), 
        &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    // Texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(vec2), 
        &texCoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);

    // Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
        &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Model::Draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

