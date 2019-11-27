//
//  Particle.hpp
//  OpenGLTest
//
//  Created by Outro on 2019/11/25.
//  Copyright © 2019 Outro.com. All rights reserved.
//

#ifndef Particle_hpp
#define Particle_hpp

#include <iostream>
using namespace std;
#include <vector>

#include <GL/gl3.h> // only for hint of function
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Resource_Manager.hpp"
#include "GameObject.hpp"

/*
 Responsibility
 Type:
    1. struct Particle
        [Define the position, velocity, color and life of particle]
 Variable:
    1. Define particle array
    2. Define particle amount
    3. Define shader
    4. Define texture
    5. Define vertex array object
 
 Function:
    1. ParticleGenerator(Shader shader, Texture2D texture, GLuint amount)
        [call init() function and initialize shader, texture and amount]
    2. void Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f))
        [check if a particle is destoryed and add a new one. And update life of all particles]
    3. void Draw();
        [render the particles]
    4. void init()
        [initialize VAO and particle array]
    5. GLuint firstUnusedParticle();
        [acts as its name]
    6. void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f))
        [random position and color when respawn]
 */

struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    GLfloat Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


class ParticleGenerator {
public:
    // Constructor
    ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
    // Update all particles
    void Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    // Render all particles
    void Draw();
private:
    // State
    std::vector<Particle> particles;
    GLuint amount;
    // Render state
    Shader shader;
    Texture2D texture;
    GLuint VAO;
    // Initializes buffer and vertex attributes
    void init();
    // Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    GLuint firstUnusedParticle();
    // Respawns particle
    void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};


#endif /* Particle_hpp */
