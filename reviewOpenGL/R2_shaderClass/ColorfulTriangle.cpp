#include <iostream>
using namespace std;
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include "ReviewShader.h"

void framebuffer_callback_function(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Review", NULL, NULL);
    if(window == NULL){
        cout << "Create window failed\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "Initialized glad failed\n";
        return -1;
    }
    
    float vertices[] = {
       -0.5, -0.5, 0.0, 1.0, 0.0, 0.0,
        0.5, -0.5, 0.0, 0.0, 1.0, 0.0,
        0.0, 0.5, 0.0,  0.0, 0.0, 1.0
    };
    
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    ReviewShader shader("OpenGLTest/Review.vs", "OpenGLTest/Review.fs");
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    while(!glfwWindowShouldClose(window)){
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        processInput(window);
        
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue)+1) * 0.5;
        
//        glUseProgram(shaderProgram);
        shader.use();
//        glUniform4f(glGetUniformLocation(shaderProgram, "ourColor"), 0.0, greenValue, 0.0, 1.0);
        shader.set4f("ourColor", 0.0, greenValue, 0.0, 1.0);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}
