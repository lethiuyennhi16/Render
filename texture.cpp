#include "texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* path) {
    // Khởi tạo ID texture
    glGenTextures(1, &ID);
    
    // Load và tạo texture
    if (!loadFromFile(path)) {
        cout << "ERROR::TEXTURE::Failed to load texture from: " << path << endl;
    }
}

void Texture::bind(unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    // Bind texture
    glBindTexture(GL_TEXTURE_2D, ID);
    //debug
    GLint currentTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
    if (currentTexture != ID) {
        std::cout << "Warning: Texture " << ID << " not bound correctly!" << std::endl;
    }
}

bool Texture::loadFromFile(const char* path) {
    // Load image bằng stb_image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    
    if (!data) {
        cout << "ERROR::TEXTURE::Failed to load image: " << path << endl;
        return false;
    }

    GLenum format;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
    else {
        cout << "ERROR::TEXTURE::Unsupported number of channels: " << nrChannels << endl;
        stbi_image_free(data);
        return false;
    }

    // Bind texture
    glBindTexture(GL_TEXTURE_2D, ID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Giải phóng data
    stbi_image_free(data);

    return true;
} 
