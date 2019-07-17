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
     * Create model
     */
    ReviewShader shader("OpenGLTest/model.vs", "OpenGLTest/model.fs");
    Model nanomodel("Model/nanosuit/nanosuit.obj");

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!glfwWindowShouldClose(window)){
        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);
        
        glm::mat4 model(1.0);
        glm::mat4 view(1.0);
        view = camera.getViewMatrix();
        glm::mat4 projection(1.0);
        projection = glm::perspective(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        nanomodel.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

