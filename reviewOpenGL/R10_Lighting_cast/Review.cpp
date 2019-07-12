#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include "ReviewShader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb-master/stb_image.h>

#include "ReviewCamera.h"
using namespace std;

// Window parameters
#define SCR_WIDTH 800
#define SCR_HEIGHT 600
float fov = 45.0;

// Camera parameters
bool firstIn = true;
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

// Camera object
Camera camera(glm::vec3(0.0, 0.0, 5.0));

// colors
glm::vec3 cubeColor(1.0, 0.5, 0.31);
glm::vec3 lightColor(1.0);

// positions
glm::vec3 lightPos(0.0, 0.0, 2.0);

// Record time
float lastTime = 0;
float deltaTime = 0;
/**
 * Functions
 */
void framebuffer_callback_function(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
    float moveSpeed = 0.05;

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.processKeyboard(FORWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.processKeyboard(BACKWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.processKeyboard(LEFT, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.processKeyboard(RIGHT, deltaTime);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if(firstIn){
        lastX = xpos;
        lastY = ypos;
        firstIn = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX  = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.processMouseScroll(yoffset);
}

int main(){
    /**
     * Create the window
     */
    // 1. Initialize the glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 2. Create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Review", NULL, NULL);
    if(window == NULL){
        glfwTerminate();
        cout << "Create window failed\n";
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // 3. Initialize GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "Initialize glad failed\n";
        return -1;
    }
    // Enable some function that you want
    glEnable(GL_DEPTH_TEST);

    /**
     * Set vertices
     */
    // 1. Define the vertex and index data
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    /**
     * light VAO, VBO
     */
    unsigned int lightVAO, lightVBO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glGenBuffers(1, &lightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    /**
     * cube VAO, VBO
     */
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    /**
     * Create diffuse texture
     */
    int width, height, urChannel;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("img/container2.png", &width, &height, &urChannel, 0);

    unsigned int diffuseTexture;
    glGenTextures(1, &diffuseTexture);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if(data){
        GLint format;
        if(urChannel == 3)
            format = GL_RGB;
        if(urChannel == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        cout << "Read texture failed\n";
    }
    stbi_image_free(data);


    /**
     * Create specular texture
     */
    unsigned int specularTexture;
    glGenTextures(1, &specularTexture);
    glBindTexture(GL_TEXTURE_2D, specularTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    data = stbi_load("img/container2_specular.png", &width, &height, &urChannel, 0);
    if(data){
        GLint format;
        if(urChannel == 3)
        format = GL_RGB;
        if(urChannel == 4)
        format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularTexture);

    /**
     * Create light and cube shader
     */
    ReviewShader lightShader("OpenGLTest/light.vs", "OpenGLTest/light.fs");
    ReviewShader cubeShader("OpenGLTest/cube.vs", "OpenGLTest/cube.fs");

    while(!glfwWindowShouldClose(window)){
        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        glClearColor(0.1, 0.1, 0.1, 0.1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window);

        glm::mat4 model = glm::mat4(1.0);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glBindVertexArray(lightVAO);
        lightShader.use();

        model = glm::mat4(1.0);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2));

        lightShader.setMat4("model", model);
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);
        lightShader.setVec3("lightColor", lightColor);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(cubeVAO);
        cubeShader.use();
        cubeShader.setMat4("view", view);
        cubeShader.setMat4("projection", projection);

        cubeShader.setVec3("cubeColor", cubeColor);
        cubeShader.setVec3("lightColor", lightColor);
        cubeShader.setVec3("viewPos", camera.Position);

        cubeShader.setInt("material.diffuse", 0);
        cubeShader.setInt("material.specular", 1);
        cubeShader.setFloat("material.shininess", 32.0);

        cubeShader.setVec3("flashlight.position", lightPos);
        cubeShader.setVec3("flashlight.ambient", glm::vec3(0.2));
        cubeShader.setVec3("flashlight.diffuse", glm::vec3(0.5));
        cubeShader.setVec3("flashlight.specular", glm::vec3(1.0));
        cubeShader.setFloat("flashlight.constant", 1.0);
        cubeShader.setFloat("flashlight.linear", 0.09);
        cubeShader.setFloat("flashlight.quadratic", 0.032);
        cubeShader.setVec3("flashlight.position", camera.Position);
        cubeShader.setVec3("flashlight.direction", camera.Front);
        cubeShader.setFloat("flashlight.cutoff", glm::cos(glm::radians(12.5f)));
        cubeShader.setFloat("flashlight.boardAngle", glm::cos(glm::radians(10.0f)));

        for(int i = 0; i < 10; i++){
            model = glm::mat4(1.0);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0, 0.3, 0.5));
            cubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

