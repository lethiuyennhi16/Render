#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad.h"
#include <string>

using namespace std;

class Texture {
public:
    // ID của texture trong OpenGL
    unsigned int ID;
    
    // Constructor
    Texture(const char* path);
    
    // Bind texture vào active texture unit
    void bind(unsigned int unit = 0) const;

private:
    // Load image từ file và tạo texture
    bool loadFromFile(const char* path);
};

#endif 