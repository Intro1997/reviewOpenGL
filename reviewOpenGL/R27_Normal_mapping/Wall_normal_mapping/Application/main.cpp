#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"
#include "Shader.hpp"
#include "Model.h"

#include <vector>
#include <iostream>
using namespace std;

// functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(vector<string> faces);
void renderQuad();



// size settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH  / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// globle vertex
glm::vec3 lightPos(0.0f, 0.0f, 1.0f);


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
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // Configure the Texture
    // --------------------
    unsigned int imgTexture = loadTexture("img/image1.jpg");
    
    // Load texture
    // ------------
    unsigned int wallTexture = loadTexture("img/brickwall.jpg");
    unsigned int wallNormalTexture = loadTexture("img/brickwall_normal.jpg");
    
    // Create shader
    // -------------
    Shader shader("shader/shader.vs", "shader/shader.fs");
    
    // Configure the shaders
    // ---------------------
    shader.use();
    shader.setInt("wallTexture", 0);
    shader.setInt("wallNormalTexture", 1);
    
    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float aspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;
        glm::mat4 model = glm::mat4(1.0);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, aspect, 0.1f, 100.0f);
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0));
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("viewPos", camera.Position);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, wallNormalTexture);
        renderQuad();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// render quad
// -----------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad(){
    if(quadVAO == 0){
        glm::vec3 pos1 = glm::vec3(-1.0, 1.0, 0.0);     // left-up
        glm::vec3 pos2 = glm::vec3(-1.0, -1.0, 0.0);    // left-down
        glm::vec3 pos3 = glm::vec3(1.0, 1.0, 0.0);      // right-up
        glm::vec3 pos4 = glm::vec3(1.0, -1.0, 0.0);     // right-down
        
        glm::vec2 uv1 = glm::vec2(0.0, 1.0);
        glm::vec2 uv2 = glm::vec2(0.0, 0.0);
        glm::vec2 uv3 = glm::vec2(1.0, 1.0);
        glm::vec2 uv4 = glm::vec2(1.0, 0.0);
        
        // create T1, B1
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos4 - pos1;
        
        glm::vec2 delta1 = uv2 - uv1;
        glm::vec2 delta2 = uv4 - uv1;
        
        float f = 1.0 / (delta1.x * delta2.y - delta2.x * delta1.y);
        
        glm::vec3 T1 = f * ( delta2.y * edge1 - delta1.y * edge2);
        T1 = glm::normalize(T1);
        glm::vec3 B1 = f * (-delta2.x * edge1 + delta1.x * edge2);
        B1 = glm::normalize(B1);
        
        // create T2, B2
        edge1 = pos4 - pos1;
        edge2 = pos3 - pos1;
        
        delta1 = uv4 - uv1;
        delta2 = uv3 - uv1;
        
        f = 1.0 / (delta1.x * delta2.y - delta2.x * delta1.y);
        
        glm::vec3 T2 = f * (delta2.y * edge1 - delta1.y * edge2);
        T2 = glm::normalize(T2);
        glm::vec3 B2 = f * (-delta2.x * edge1 + delta1.x * edge2);
        B2 = glm::normalize(B2);
        
        float quadVertices[] = {
            pos1.x, pos1.y, pos1.z, uv1.x, uv1.y, T1.x, T1.y, T1.z, B1.x, B1.y, B1.z,
            pos2.x, pos2.y, pos2.z, uv2.x, uv2.y, T1.x, T1.y, T1.z, B1.x, B1.y, B1.z,
            pos4.x, pos4.y, pos4.z, uv4.x, uv4.y, T1.x, T1.y, T1.z, B1.x, B1.y, B1.z,
            
            pos1.x, pos1.y, pos1.z, uv1.x, uv1.y, T2.x, T2.y, T2.z, B2.x, B2.y, B2.z,
            pos4.x, pos4.y, pos4.z, uv4.x, uv4.y, T2.x, T2.y, T2.z, B2.x, B2.y, B2.z,
            pos3.x, pos3.y, pos3.z, uv3.x, uv3.y, T2.x, T2.y, T2.z, B2.x, B2.y, B2.z
        };
        
        glGenVertexArrays(1, &quadVAO);
        glBindVertexArray(quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(sizeof(float) * 5));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(sizeof(float) * 8));
        glEnableVertexAttribArray(3);
        glBindVertexArray(0);
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
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

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
        lightPos += glm::vec3(0.0, deltaTime, 0.0);
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
        lightPos -= glm::vec3(0.0, deltaTime, 0.0);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
        lightPos -= glm::vec3(deltaTime, 0.0, 0.0);
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
        lightPos += glm::vec3(deltaTime, 0.0, 0.0);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        lightPos -= glm::vec3(0.0, 0.0, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        lightPos += glm::vec3(0.0, 0.0, deltaTime);
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if(data)
    {
        GLenum format;
        if (nrComponents == 1)
        format = GL_RED;
        else if (nrComponents == 3)
        format = GL_RGB;
        else if (nrComponents == 4)
        format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
//        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
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

unsigned int loadCubemap(vector<string> faces){
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

