#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include "ReviewShader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb-master/stb_image.h>
using namespace std;

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

void framebuffer_callback_function(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

int main(){
    /**
     * Create the window
     *
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
    
    // 3. Initialize GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "Initialize glad failed\n";
        return -1;
    }
    
    
    
    /**
     * Set vertices
     */
    // 1. Define the vertex and index data
    float vertices[] = {
        // vertex         // texCoord
        -0.5, -0.5, 0.0,  0.0, 0.0,
        -0.5,  0.5, 0.0,  0.0, 1.0,
         0.5,  0.5, 0.0,  1.0, 1.0,
         0.5, -0.5, 0.0,  1.0, 0.0
    };
    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    
    // 2. Create VAO, VBO, EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    /**
     * Create shader
     */
    ReviewShader shader("OpenGLTest/Review.vs", "OpenGLTest/Review.fs");
    
    /**
     * Create texture
     */
    stbi_set_flip_vertically_on_load(true);
    // 1. Create texture1
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    // 2. Read the images
    int width, height, urChannel;
    unsigned char* data;
    data = stbi_load("img/container.jpg", &width, &height, &urChannel, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        cout << "Read images failed\n";
    }
    stbi_image_free(data);
    
    // 3. Repeat for textrue2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    data = stbi_load("img/awesomeface.png", &width, &height, &urChannel, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        cout << "Read images failed\n";
    }
    stbi_image_free(data);
    
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    while(!glfwWindowShouldClose(window)){
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        processInput(window);
        glBindVertexArray(VAO);
        shader.use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    
    return 0;
}

