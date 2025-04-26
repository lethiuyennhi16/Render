#version 330 core
out vec3 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
flat in vec3 view_tri[3];
flat in vec2 varying_uv[3];

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform vec3 uniform_l;

void main() {
    vec3 edge1 = view_tri[1] - view_tri[0];
    vec3 edge2 = view_tri[2] - view_tri[0];
    vec2 deltaUV1 = varying_uv[1] - varying_uv[0];
    vec2 deltaUV2 = varying_uv[2] - varying_uv[0];

    float f = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    vec3 tangent = f * (edge1 * deltaUV2.y - edge2 * deltaUV1.y);
    vec3 bitangent = f * (-edge1 * deltaUV2.x + edge2 * deltaUV1.x);

    tangent = normalize(tangent);
    bitangent = normalize(bitangent);
    vec3 n = normalize(Normal);
    tangent = normalize(tangent - n * dot(n, tangent));
    bitangent = cross(n, tangent);

    mat3 TBN = mat3(tangent, bitangent, n);

    vec3 texNormal = texture(normalMap, TexCoords).rgb;
    texNormal = normalize(texNormal * 2.0 - 1.0);
    vec3 finalNormal = normalize(TBN * texNormal);
    float diff = max(dot(finalNormal, uniform_l), 0.0);
    vec3 r = normalize(2.0 * finalNormal * dot(finalNormal, uniform_l) - uniform_l);
    float spec = pow(max(-r.z, 0.0), 5.0 + texture(specularMap, TexCoords).r);

    vec3 c = texture(diffuseMap, TexCoords).rgb;
    vec3 result;
    for(int i = 0; i < 3; i++) {
        int color = int(c[i]*255.0);
        float light = diff + spec;
        result[i] = float(min(10 + int(color * light), 255)) / 255.0;
    }

    FragColor = result;
}

