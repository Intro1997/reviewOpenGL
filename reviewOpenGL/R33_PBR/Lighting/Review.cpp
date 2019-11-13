#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
//value = [tlsSettings objectForKey:(NSString *)kCFStreamSSLAllowsExpiredCertificates];//过期属性


#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ReviewCamera.h"
#include "ReviewShader.h"
#include "Model.h"

#include <vector>
#include <iostream>
#include <random>

#define STB_IMAGE_IMPLEMENTATION
#include <stb-master/stb_image.h>
using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(vector<string> faces);
void renderQuad();
void renderCube();
void renderPlane();
void renderSphere();
float lerp(float a, float b, float scale);

// size settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);

// calculation parameter
const float Pi = 3.14159265359;
const float doublePi = Pi * 2.0;


// camera
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = (float)SCR_WIDTH  / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// texture normal
glm::vec3 normal(0.0, 0.0, 1.0);

// plane normal
glm::vec3 planeNormal = glm::vec3(0.0, 1.0, 0.0);


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);        // tell GLFW to capture our mouse

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Configure the test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    // Create shaders
    ReviewShader sphereShader("shader/sphere.vs", "shader/sphere.fs");
    ReviewShader pbrShader("shader/sphere.vs", "shader/PBR.fs");
    ReviewShader quadShader("shader/quad.vs", "shader/sphere.fs");

    glm::vec3 lightPositions[] = {
        glm::vec3(0.0f,  0.0f, 5.0f),
//        glm::vec3( 5.0f,  5.0f, 5.0f),
//        glm::vec3(-5.0f, -5.0f, 5.0f),
//        glm::vec3( 5.0f, -5.0f, 5.0f),
    };
    glm::vec3 lightColors[] = {
        glm::vec3(100.0f, 100.0f, 100.0f),
//        glm::vec3(100.0f, 100.0f, 100.0f),
//        glm::vec3(100.0f, 100.0f, 100.0f),
//        glm::vec3(100.0f, 100.0f, 100.0f)
    };
    pbrShader.use();
    pbrShader.setInt("albedoTexture", 0);
    pbrShader.setInt("metallicTexture", 1);
    pbrShader.setInt("normalTexture", 2);
    pbrShader.setInt("roughnessTexture", 3);

    unsigned int albedo = loadTexture("Sphere/rustediron2_basecolor.png");
    unsigned int metallic = loadTexture("Sphere/rustediron2_metallic.png");
    unsigned int normal = loadTexture("Sphere/rustediron2_normal.png");
    unsigned int roughness = loadTexture("Sphere/rustediron2_roughness.png");

    pbrShader.setFloat("ao", 1.0f);

    for (int i = 0; i < 1; i++) {
        pbrShader.setVec3("lightPositions[" + to_string(i) + "]", lightPositions[i]);
        pbrShader.setVec3("lightColors[" + to_string(i) + "]", lightColors[i]);
    }


    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!glfwWindowShouldClose(window))
    {
        // avoid delay
        // -----------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // process input
        // -------------
        processInput(window);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        pbrShader.use();
        pbrShader.setMat4("view", view);
        pbrShader.setMat4("projection", projection);
        pbrShader.setVec3("camPos", camera.Position);
        pbrShader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, albedo);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, metallic);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, normal);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, roughness);

        for(int i = 0; i <= 6; i++){
            for (int j = 0; j <= 6; j++) {
                model = glm::mat4(1.0);
                model = glm::translate(model, glm::vec3(i - 3, j - 3, 0.0) * glm::vec3(2.5));
                pbrShader.setMat4("model", model);
                renderSphere();
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

unsigned int sphereVAO = 0;
int indicesCount;
void renderSphere() {
    if(sphereVAO == 0){
        unsigned int sphereVBO;
        unsigned int sphereEBO;
        vector<glm::vec3> position;
        vector<glm::vec2> uv;
        vector<glm::vec3> normal;
        vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;

        // get position, texture coordinates uv and normal
        for(int y = 0; y <= Y_SEGMENTS; ++y){
            for(int x = 0; x <= X_SEGMENTS; ++x){
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;

                float xPos = sin(ySegment * Pi) * cos(xSegment * doublePi);
                float yPos = cos(ySegment * Pi);
                float zPos = sin(ySegment * Pi) * sin(xSegment * doublePi);

                position.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normal.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        // get indices
        bool isOdd = false;
        for(int i = 0; i < Y_SEGMENTS; ++i){
//            if(!isOdd){
//                for (int j = 0; j <= X_SEGMENTS; ++j) {
//                    indices.push_back(i       * (X_SEGMENTS + 1) + j);
//                    indices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
//                }
//            }
//            else{
//                for (int j = X_SEGMENTS; j >= 0; --j) {
//                    indices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
//                    indices.push_back(i       * (X_SEGMENTS + 1) + j);
//                }
//            }
//            isOdd = !isOdd;
            for (int j = 0; j <= X_SEGMENTS; ++j) {
                indices.push_back(i       * (X_SEGMENTS + 1) + j);
                indices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
            }
        }
        indicesCount = indices.size();
        vector<float> data;
        for (int i = 0; i < position.size(); ++i) {
            data.push_back(position[i].x);
            data.push_back(position[i].y);
            data.push_back(position[i].z);
            if(uv.size() > 0) {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
            if(normal.size() > 0) {
                data.push_back(normal[i].x);
                data.push_back(normal[i].y);
                data.push_back(normal[i].z);
            }
        }
        glGenVertexArrays(1, &sphereVAO);
        glGenBuffers(1, &sphereVBO);
        glGenBuffers(1, &sphereEBO);
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        GLsizei stride = 8 * sizeof(float);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 5));
        glEnableVertexAttribArray(2);
    }

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indicesCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad(){
    if(quadVAO == 0){
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
        };

        glGenVertexArrays(1, &quadVAO);
        glBindVertexArray(quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}



unsigned int cubeVAO = 0;
unsigned int cubeVBO;
void renderCube(){
    // Cube vertex
    // -----------
    if(cubeVAO == 0){

        float cubeVertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
            1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
            1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
            1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
        };
        glGenVertexArrays(1, &cubeVAO);
        glBindVertexArray(cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);
    }
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

}

unsigned int planeVAO = 0;
unsigned int planeVBO;
void renderPlane(){
    if(planeVAO == 0){
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0,
            -1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0,
            1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0,
            1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0
        };

        glGenVertexArrays(1, &planeVAO);
        glBindVertexArray(planeVAO);
        glGenBuffers(1, &planeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
    }
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera move
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.processMouseScroll(yoffset);
}
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
    
unsigned int loadCubemap(vector<string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, urChannel;
    for(int i = 0; i < faces.size(); i++){
        unsigned char *data = stbi_load(("skybox/" + faces[i]).c_str(), &width, &height, &urChannel, 0);
        if(data){
            GLenum format;
            if(urChannel == 1)
                format = GL_RED;
            if(urChannel == 3)
                format = GL_RGB;
            if(urChannel == 4)
                format = GL_RGBA;

            //            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else{
            cout << "Read texture: " << faces[i] << " failed\n";
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

float lerp(float a, float b, float f) {
    return a + f * (b - a);
}


//
//
//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wdeprecated-declarations"
////value = [tlsSettings objectForKey:(NSString *)kCFStreamSSLAllowsExpiredCertificates];//过期属性
//
//
//#include <GLFW/glfw3.h>
//#include <glad/glad.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "ReviewCamera.h"
//#include "ReviewShader.h"
//#include "Model.h"
//
//#include <vector>
//#include <iostream>
//#include <random>
//using namespace std;
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow *window);
//unsigned int loadTexture(const char *path);
//void renderSphere();
//
//// settings
//const unsigned int SCR_WIDTH = 1280;
//const unsigned int SCR_HEIGHT = 720;
//
//// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 20.0f));
//float lastX = 800.0f / 2.0;
//float lastY = 600.0 / 2.0;
//bool firstMouse = true;
//
//// timing
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//
//int main()
//{
//    // glfw: initialize and configure
//    // ------------------------------
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_SAMPLES, 4);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
//#endif
//
//    // glfw window creation
//    // --------------------
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//    glfwMakeContextCurrent(window);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//    glfwSetCursorPosCallback(window, mouse_callback);
//    glfwSetScrollCallback(window, scroll_callback);
//
//    // tell GLFW to capture our mouse
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//    // glad: load all OpenGL function pointers
//    // ---------------------------------------
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    // configure global opengl state
//    // -----------------------------
//    glEnable(GL_DEPTH_TEST);
//
//    // build and compile shaders
//    // -------------------------
//    ReviewShader shader("shader/sphere.vs", "shader/PBR.fs");
//
//    shader.use();
//    shader.setInt("albedoTexture", 0);
//    shader.setInt("normalTexture", 1);
//    shader.setInt("metallicTexture", 2);
//    shader.setInt("roughnessTexture", 3);
////    shader.setInt("aoMap", 4);
//
//    // load PBR material textures
//    // --------------------------
//    unsigned int albedo = loadTexture("Sphere/rustediron2_basecolor.png");
//    unsigned int metallic = loadTexture("Sphere/rustediron2_metallic.png");
//    unsigned int normal = loadTexture("Sphere/rustediron2_normal.png");
//    unsigned int roughness = loadTexture("Sphere/rustediron2_roughness.png");
////    unsigned int ao        = loadTexture(FileSystem::getPath("resources/textures/pbr/rusted_iron/ao.png").c_str());
//
//    // lights
//    // ------
//    glm::vec3 lightPositions[] = {
//        glm::vec3(0.0f,  0.0f, 3.0f),
////        glm::vec3( 5.0f,  5.0f, 5.0f),
////        glm::vec3(-5.0f, -5.0f, 5.0f),
////        glm::vec3( 5.0f, -5.0f, 5.0f),
//    };
//    glm::vec3 lightColors[] = {
//        glm::vec3(100.0f, 100.0f, 100.0f),
////        glm::vec3(100.0f, 100.0f, 100.0f),
////        glm::vec3(100.0f, 100.0f, 100.0f),
////        glm::vec3(100.0f, 100.0f, 100.0f)
//    };
//    int nrRows = 7;
//    int nrColumns = 7;
//    float spacing = 2.5;
//
//    // initialize static shader uniforms before rendering
//    // --------------------------------------------------
//    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//    shader.use();
//    shader.setMat4("projection", projection);
//    shader.setVec3("ao", glm::vec3(1.0));
//    // render loop
//    // -----------
//    while (!glfwWindowShouldClose(window))
//    {
//        // per-frame time logic
//        // --------------------
//        float currentFrame = glfwGetTime();
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//
//        // input
//        // -----
//        processInput(window);
//
//        // render
//        // ------
//        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        shader.use();
//        glm::mat4 view = camera.getViewMatrix();
//        shader.setMat4("view", view);
//        shader.setVec3("camPos", camera.Position);
//
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, albedo);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D, normal);
//        glActiveTexture(GL_TEXTURE2);
//        glBindTexture(GL_TEXTURE_2D, metallic);
//        glActiveTexture(GL_TEXTURE3);
//        glBindTexture(GL_TEXTURE_2D, roughness);
////        glActiveTexture(GL_TEXTURE4);
////        glBindTexture(GL_TEXTURE_2D, ao);
//
//        // render rows*column number of spheres with material properties defined by textures (they all have the same material properties)
//        glm::mat4 model = glm::mat4(1.0f);
//        for (int row = 0; row < nrRows; ++row)
//        {
//            for (int col = 0; col < nrColumns; ++col)
//            {
//                model = glm::mat4(1.0f);
//                model = glm::translate(model, glm::vec3(
//                    (float)(col - (nrColumns / 2)) * spacing,
//                    (float)(row - (nrRows / 2)) * spacing,
//                    0.0f
//                ));
//                shader.setMat4("model", model);
//                renderSphere();
//            }
//        }
//
//        // render light source (simply re-render sphere at light positions)
//        // this looks a bit off as we use the same shader, but it'll make their positions obvious and
//        // keeps the codeprint small.
//        for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
//        {
//            glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
//            newPos = lightPositions[i];
//            shader.setVec3("lightPositions[" + std::to_string(i) + "]", lightPositions[i]);
//            shader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
//
//        }
//
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        // -------------------------------------------------------------------------------
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // glfw: terminate, clearing all previously allocated GLFW resources.
//    // ------------------------------------------------------------------
//    glfwTerminate();
//    return 0;
//}
//
//// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//// ---------------------------------------------------------------------------------------------------------
//void processInput(GLFWwindow *window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//    float cameraSpeed = 2.5 * deltaTime;
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        camera.processKeyboard(FORWARD, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        camera.processKeyboard(BACKWARD, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        camera.processKeyboard(LEFT, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        camera.processKeyboard(RIGHT, deltaTime);
//}
//
//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    // make sure the viewport matches the new window dimensions; note that width and
//    // height will be significantly larger than specified on retina displays.
//    glViewport(0, 0, width, height);
//}
//
//
//// glfw: whenever the mouse moves, this callback is called
//// -------------------------------------------------------
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//    lastX = xpos;
//    lastY = ypos;
//
//    camera.processMouseMovement(xoffset, yoffset, true);
//}
//
//// glfw: whenever the mouse scroll wheel scrolls, this callback is called
//// ----------------------------------------------------------------------
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    camera.processMouseScroll(yoffset);
//}
//
//// renders (and builds at first invocation) a sphere
//// -------------------------------------------------
//unsigned int sphereVAO = 0;
//unsigned int indexCount;
//void renderSphere()
//{
//    if (sphereVAO == 0)
//    {
//        glGenVertexArrays(1, &sphereVAO);
//
//        unsigned int vbo, ebo;
//        glGenBuffers(1, &vbo);
//        glGenBuffers(1, &ebo);
//
//        std::vector<glm::vec3> positions;
//        std::vector<glm::vec2> uv;
//        std::vector<glm::vec3> normals;
//        std::vector<unsigned int> indices;
//
//        const unsigned int X_SEGMENTS = 64;
//        const unsigned int Y_SEGMENTS = 64;
//        const float PI = 3.14159265359;
//        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
//        {
//            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
//            {
//                float xSegment = (float)x / (float)X_SEGMENTS;
//                float ySegment = (float)y / (float)Y_SEGMENTS;
//                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
//                float yPos = std::cos(ySegment * PI);
//                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
//
//                positions.push_back(glm::vec3(xPos, yPos, zPos));
//                uv.push_back(glm::vec2(xSegment, ySegment));
//                normals.push_back(glm::vec3(xPos, yPos, zPos));
//            }
//        }
//
//        bool oddRow = false;
//        for (int y = 0; y < Y_SEGMENTS; ++y)
//        {
//            if (!oddRow) // even rows: y == 0, y == 2; and so on
//            {
//                for (int x = 0; x <= X_SEGMENTS; ++x)
//                {
//                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
//                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
//                }
//            }
//            else
//            {
//                for (int x = X_SEGMENTS; x >= 0; --x)
//                {
//                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
//                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
//                }
//            }
//            oddRow = !oddRow;
//        }
//        indexCount = indices.size();
//
//        std::vector<float> data;
//        for (int i = 0; i < positions.size(); ++i)
//        {
//            data.push_back(positions[i].x);
//            data.push_back(positions[i].y);
//            data.push_back(positions[i].z);
//            if (uv.size() > 0)
//            {
//                data.push_back(uv[i].x);
//                data.push_back(uv[i].y);
//            }
//            if (normals.size() > 0)
//            {
//                data.push_back(normals[i].x);
//                data.push_back(normals[i].y);
//                data.push_back(normals[i].z);
//            }
//        }
//        glBindVertexArray(sphereVAO);
//        glBindBuffer(GL_ARRAY_BUFFER, vbo);
//        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
//        float stride = (3 + 2 + 3) * sizeof(float);
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
//        glEnableVertexAttribArray(1);
//        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
//        glEnableVertexAttribArray(2);
//        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
//    }
//
//    glBindVertexArray(sphereVAO);
//    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
//}
//
//// utility function for loading a 2D texture from file
//// ---------------------------------------------------
//unsigned int loadTexture(char const * path)
//{
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//
//    int width, height, nrComponents;
//    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
//    if (data)
//    {
//        GLenum format;
//        if (nrComponents == 1)
//            format = GL_RED;
//        else if (nrComponents == 3)
//            format = GL_RGB;
//        else if (nrComponents == 4)
//            format = GL_RGBA;
//
//        glBindTexture(GL_TEXTURE_2D, textureID);
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(data);
//    }
//    else
//    {
//        std::cout << "Texture failed to load at path: " << path << std::endl;
//        stbi_image_free(data);
//    }
//
//    return textureID;
//}
