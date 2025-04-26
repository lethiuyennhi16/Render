#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 vFragPos[];
in vec3 vNormal[];
in vec2 vTexCoords[];

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
flat out vec3 view_tri[3];
flat out vec2 varying_uv[3];
flat out vec3 varying_nrm[3];

void main() {
    for (int i = 0; i < 3; i++) {
        FragPos = vFragPos[i];
        Normal = vNormal[i];
        TexCoords = vTexCoords[i];

        view_tri[0] = vFragPos[0];
        view_tri[1] = vFragPos[1];
        view_tri[2] = vFragPos[2];

        varying_uv[0] = vTexCoords[0];
        varying_uv[1] = vTexCoords[1];
        varying_uv[2] = vTexCoords[2];

        varying_nrm[0] = vNormal[0];
        varying_nrm[1] = vNormal[1];
        varying_nrm[2] = vNormal[2];

        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
