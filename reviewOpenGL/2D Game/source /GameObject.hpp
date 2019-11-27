//
//  GameObject.hpp
//  OpenGLTest
//
//  Created by Outro on 2019/11/21.
//  Copyright © 2019 Outro.com. All rights reserved.
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include <GL/gl3.h> // only for hint of function
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Resource_Manager.hpp"
#include "SpriteRenderer.hpp"

#include <iostream>
using namespace std;

/*
 Responsibility
 Varialbe:
     1. Define the position, size, color, rotation and velocity for objects
     2. Define if the brick is destroyed
     3. Define if the brick is solid (cannot be destroyed)
     4. Define the texture of Sprite used in object
 Function:
     1. GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0), glm::vec2 velocity = glm::vec2(0.0, 0.0));
        [initialize parameters]
     2. void GameObject::Draw(SpriteRenderer &renderer)
        [Use renderer render the texture of object]
 */

class GameObject {
public:
    glm::vec2 Position, Size, Velocity;
    glm::vec3 Color;
    GLfloat Rotation;
    GLboolean Destroyed;
    GLboolean IsSolid;
    Texture2D Sprite;
    
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0), glm::vec2 velocity = glm::vec2(0.0, 0.0));
    
    virtual void Draw(SpriteRenderer &renderer);
    
};

#endif /* GameObject_hpp */
