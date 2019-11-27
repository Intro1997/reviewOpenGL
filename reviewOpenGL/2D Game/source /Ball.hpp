//
//  Ball.hpp
//  OpenGLTest
//
//  Created by Outro on 2019/11/22.
//  Copyright © 2019 Outro.com. All rights reserved.
//

#ifndef Ball_hpp
#define Ball_hpp

#include <iostream>
using namespace std;
#include "GameObject.hpp"

/*
 Responsibility(inherits class GameObject)
 Variable:
     1. Define radius
     2. Define a variable check if ball is stuck on the paddle
 
 Function:
     1. BallObject();
        [do nothing]
     2. BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);
        [initialize BallObject parameter and GameObject parameter]
     3. glm::vec2 Move(GLfloat dt, GLuint window_width);
        [Define the move action of the ball]
     4. void Reset(glm::vec2 pos, glm::vec2 velocity);
        [Reset the ball position and stuck status]
*/

class BallObject : public GameObject {
public:
    GLfloat Radius;
    GLboolean Stuck;
    
    BallObject();
    BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);
    glm::vec2 Move(GLfloat dt, GLuint window_width);
    void Reset(glm::vec2 pos, glm::vec2 velocity);
};

#endif /* Ball_hpp */
