
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
//value = [tlsSettings objectForKey:(NSString *)kCFStreamSSLAllowsExpiredCertificates];//过期属性

#include <GL/gl3.h> // only for hint of function
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Texture.hpp"
#include "Shader.hpp"
#include "Resource_Manager.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "GameLevel.hpp"
#include "Particle.hpp"

using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void renderQuad();

const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;
Game Breakout(SCR_WIDTH, SCR_HEIGHT);

GLuint nr_particles = 500;
vector<Particle> particles;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

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
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader particleShader = ResourceManager::loadShader("shader/particle.vs", "shader/particle.fs", nullptr, "particleShader");
            
    glCullFace(GL_CULL_FACE);
    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    Breakout.Init();

    GLfloat deltaTime = 0.0;
    GLfloat lastFrame = 0.0;

    Breakout.State = GAME_ACTIVE;    

    while(!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        Breakout.ProcessInput(deltaTime);

        Breakout.Update(deltaTime);

        processInput(window);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        Breakout.Render();

        glfwSwapBuffers(window);
        glfwPollEvents(); // why puts this function here?
    }
    glfwTerminate();
    return 0;
}
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // paddle control
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Breakout.Keys[GLFW_KEY_A] = true;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
        Breakout.Keys[GLFW_KEY_A] = false;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Breakout.Keys[GLFW_KEY_D] = true;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
        Breakout.Keys[GLFW_KEY_D] = false;
    
    // release ball
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        Breakout.Keys[GLFW_KEY_SPACE] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE){
        Breakout.Keys[GLFW_KEY_SPACE] = false;
    }

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
