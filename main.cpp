#include "glad.h"
#include <GLFW/glfw3.h>
#include "./glm/glm/glm.hpp"
#include "./glm/glm/gtc/matrix_transform.hpp"
#include "./glm/glm/gtc/type_ptr.hpp"
#include "shader.h"
#include "Model.h"
#include "texture.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace glm;

const int width = 1920;
const int height = 1080;
const vec3 light_dir(1.0f, 1.0f, 1.0f);
const vec3 eye(1.0f, 1.0f, 3.0f);
const vec3 center(0.0f, 0.0f, 0.0f);
const vec3 up(0.0f, 1.0f, 0.0f);

struct ModelInstance {
    Model* model;
    vector<Texture*> textures;
    vec3 position;
    float angle;
};

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " obj/model.obj" << endl;
        return 1;
    }
    cout << argv[0] << " " << argv[1] << endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Model Viewer", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    cout << "Loading shaders..." << endl;
    Shader shader("vertex_shader.glsl", "fragment_shader.glsl", "geometry_shader.glsl");
    shader.use();

    shader.setInt("diffuseMap", 0);
    shader.setInt("specularMap", 1);
    shader.setInt("normalMap", 2);

    vector<ModelInstance> modelInstances;

    for (int i = 1; i < argc; i += 4) {
        if (i + 3 >= argc) {
            cout << "Warning: Incomplete position data for " << argv[i] << endl;
            break;
        }

        string modelPath = argv[i];
        string basePath = modelPath.substr(0, modelPath.find_last_of('.'));

        ModelInstance instance;
        instance.model = new Model(modelPath.c_str());
        instance.position = vec3(
            stof(argv[i + 1]),
            stof(argv[i + 2]),
            stof(argv[i + 3])
        );
        instance.angle = 0.0f;

        instance.textures.push_back(new Texture((basePath + "_diffuse.tga").c_str()));
        instance.textures.push_back(new Texture((basePath + "_spec.tga").c_str()));
        instance.textures.push_back(new Texture((basePath + "_nm_tangent.tga").c_str()));

        modelInstances.push_back(instance);

        cout << "Loaded model: " << modelPath << " at position: "
             << instance.position.x << ", "
             << instance.position.y << ", "
             << instance.position.z << endl;
    }

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 view = lookAt(eye, center, up);
        mat4 projection = perspective(radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);
        shader.use();
        shader.setMat4("Projection", projection);

        for (int i = 0; i < modelInstances.size(); i++) {
            mat4 model = mat4(1.0f);
            model = translate(model, modelInstances[i].position);
            model = rotate(model, modelInstances[i].angle, vec3(0.0f, 1.0f, 0.0f));

            mat4 ModelView = view * model;
            vec3 uniform_l = normalize(
                vec3(ModelView * vec4(light_dir, 0.0f))
            );

            shader.setMat4("ModelView", ModelView);
            shader.setVec3("uniform_l", uniform_l);

            modelInstances[i].textures[0]->bind(0);
            modelInstances[i].textures[1]->bind(1);
            modelInstances[i].textures[2]->bind(2);

            modelInstances[i].model->Draw();

            //modelInstances[i].angle += 0.01f;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (int i = 0; i < modelInstances.size(); i++) {
        delete modelInstances[i].model;
        for (int j = 0; j < modelInstances[i].textures.size(); j++) {
            delete modelInstances[i].textures[j];
        }
    }

    glfwTerminate();
    return 0;
}

