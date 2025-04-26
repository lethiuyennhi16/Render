
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 vFragPos;
out vec3 vNormal;
out vec2 vTexCoords;

uniform mat4 ModelView;
uniform mat4 Projection;

void main() {
    
    vec4 viewPos = ModelView * vec4(aPos, 1.0);
    vFragPos = vec3(viewPos);
    

    vNormal = mat3(transpose(inverse(ModelView))) * aNormal;
    
    vTexCoords = aTexCoords;
    
    // Final position
    gl_Position = Projection * viewPos;
}
