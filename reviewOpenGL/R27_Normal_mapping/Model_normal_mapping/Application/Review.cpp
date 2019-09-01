#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include "ReviewShader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "ReviewCamera.h"
#include "Model.h"
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
Camera camera(glm::vec3(0.0, 0.0, 3.0));

// positions
glm::vec3 lightPos(0.0, 0.0, 8.0);

// Record time
float lastTime = 0;
float deltaTime = 0;
/**
 * Functions
 */
void framebuffer_callback_function(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void renderCube();

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
//    glEnable(GL_CULL_FACE);
    
    /**
     * Create model
     */
    ReviewShader modelShader("shader/model.vs", "shader/model.fs");
    ReviewShader lightShader("shader/light.vs", "shader/light.fs");
    Model nanomodel("Model/nanosuit/nanosuit.obj");

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!glfwWindowShouldClose(window)){
        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);
    
        // render model
        // ------------
        glm::mat4 model(1.0);
        model = glm::translate(model, glm::vec3(0.0, -2.0, 0.0));
        model = glm::scale(model, glm::vec3(0.2));
        glm::mat4 view(1.0);
        view = camera.getViewMatrix();
        glm::mat4 projection(1.0);
        projection = glm::perspective(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        modelShader.use();
        modelShader.setMat4("model", model);
        modelShader.setMat4("view", view);
        modelShader.setMat4("projection", projection);
        modelShader.setVec3("lightPos", lightPos);
        modelShader.setVec3("viewPos", camera.Position);        
        nanomodel.Draw(modelShader);
        // render Cube light
        // -----------------
        lightShader.use();
        model = glm::mat4(1.0);
        model = glm::scale(model, glm::vec3(0.05f));
        model = glm::translate(model, lightPos);
        lightShader.setMat4("model", model);
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);
        renderCube();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
unsigned int cubeVAO = 0;
unsigned int cubeVBO;
void renderCube(){
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
    
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        lightPos += glm::vec3(0.0, 1.0, 0.0) * deltaTime * 10.0f;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        lightPos += glm::vec3(0.0, -1.0, 0.0) * deltaTime * 10.0f;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        lightPos += glm::vec3(-1.0, 0.0, 0.0) * deltaTime * 10.0f;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        lightPos += glm::vec3(1.0, 0.0, 0.0) * deltaTime * 10.0f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        lightPos += glm::vec3(0.0, 0.0, -1.0) * deltaTime * 10.0f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        lightPos += glm::vec3(0.0, 0.0, 1.0) * deltaTime * 10.0f;
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
