#version 330 core
out vec4 FragColor;

in vec3 FragPos;    // Fragment position in world space
in vec3 Normal;     // Normal in world space
in vec2 TexCoords;  // Texture coordinates

// Material textures
uniform sampler2D texture_diffuse;   // Diffuse map
uniform sampler2D texture_specular;  // Specular map
uniform sampler2D texture_normal;    // Normal map

// Light properties
uniform vec3 lightPos;     // Light position
uniform vec3 viewPos;      // Camera position
uniform vec3 lightColor;   // Light color

void main() {
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Sample textures
    vec3 diffuseColor = texture(texture_diffuse, TexCoords).rgb;
    vec3 specularColor = texture(texture_specular, TexCoords).rgb;

    // Combine results
    vec3 result = (ambient + diffuse) * diffuseColor + specular * specularColor;
    FragColor = vec4(result, 1.0);
} 