//
//  Ball.cpp
//  OpenGLTest
//
//  Created by Outro on 2019/11/22.
//  Copyright © 2019 Outro.com. All rights reserved.
//

#include "Ball.hpp"

BallObject::BallObject()
    : GameObject(), Radius(12.5f), Stuck(true)
{}

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2.0, radius * 2.0), sprite, glm::vec3(1.0), velocity), Radius(radius), Stuck(true)
{

}

glm::vec2 BallObject::Move(GLfloat dt, GLuint window_width){
    if(!this->Stuck){
        // move
        this->Position += this->Velocity * dt;
        // left border of screen
        if (this->Position.x <= 0.0f)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = 0.0f;
        }
        else if(this->Position.x + this->Size.x >= window_width){
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = window_width - this->Size.x;
        }

        // top border of the screen
        if(this->Position.y <= 0.0f){
            this->Velocity.y = -this->Velocity.y;
            this->Position.y = 0.0f;
        }
    }
    return this->Position;
}

void BallObject::Reset(glm::vec2 pos, glm::vec2 velocity){
    this->Position = pos;
    this->Velocity = velocity;
    this->Stuck = true;
}
