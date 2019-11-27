//
//  GameObject.cpp
//  OpenGLTest
//
//  Created by Outro on 2019/11/21.
//  Copyright © 2019 Outro.com. All rights reserved.
//

#include "GameObject.hpp"
GameObject::GameObject()
    : Position(0, 0), Size(1, 1), Velocity(0, 0), Color(1.0f), Rotation(0.0), Sprite(), IsSolid(false), Destroyed(false)
{
    
}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
    :Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0), Sprite(sprite), IsSolid(false), Destroyed(false)
{
}

void GameObject::Draw(SpriteRenderer &renderer) {
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}
