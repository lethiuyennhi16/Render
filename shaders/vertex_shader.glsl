#version 330 core
layout (location = 0) in vec3 aPos;       // Vertex position
layout (location = 1) in vec3 aNormal;    // Vertex normal
layout (location = 2) in vec2 aTexCoords; // Texture coordinates

out vec3 FragPos;    // Fragment position in world space
out vec3 Normal;     // Normal in world space
out vec2 TexCoords;  // Texture coordinates to fragment shader

uniform mat4 model;      // Model matrix
uniform mat4 view;       // View matrix
uniform mat4 projection; // Projection matrix
uniform sampler2D normalMap;  // Đã định nghĩa là uniform

void main() {
    // Calculate fragment position in world space
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Transform normal to world space (excluding translation)
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Pass texture coordinates to fragment shader
    TexCoords = aTexCoords;
    
    // Calculate final position
    gl_Position = projection * view * vec4(FragPos, 1.0);
} 