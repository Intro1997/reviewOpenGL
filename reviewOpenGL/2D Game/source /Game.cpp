//
//  Game.cpp
//  OpenGLTest
//
//  Created by Outro on 2019/11/20.
//  Copyright © 2019 Outro.com. All rights reserved.
//

#include "Game.hpp"

SpriteRenderer *Renderer;

GameObject *Player;
const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);

BallObject *Ball;
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const GLfloat BALL_RADIUS = 12.5f;

ParticleGenerator *Particles;

Direction VectorDirection(glm::vec2 target){
    glm::vec2 compass[] = {
        glm::vec2(0.0, 1.0),
        glm::vec2(1.0, 0.0),
        glm::vec2(0.0, -1.0),
        glm::vec2(-1.0, 0.0)
    };
    GLfloat max = 0.0;
    GLuint best_match = -1;
    for (int i = 0; i < 4; i++) {
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if(dot_product > max){
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

Collision CheckCollision(BallObject &one, GameObject &two) {
    // get the center coordinate of the ball
    glm::vec2 ball_center = one.Position + one.Radius;
    
    // get the center corrdinate of the brick
    glm::vec2 brick_half_extents = glm::vec2(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 brick_center = two.Position + brick_half_extents;
    
    // get the p original coordinate (none offset)
    glm::vec2 P_origin = ball_center - brick_center;
    P_origin = glm::clamp(P_origin, -brick_half_extents, brick_half_extents);
    
    // get the p coordinate
    glm::vec2 P = P_origin + brick_center;
    
    // get the vector between the ball center and the brick center
    glm::vec2 difference = ball_center - P;
    
    if(glm::length(difference) <= one.Radius)
        return make_tuple(GL_TRUE, VectorDirection(difference), difference);
    else
        return make_tuple(GL_FALSE, UP, glm::vec2(0.0, 0.0));
}


Game::Game(GLuint width, GLuint height)
    : Width(width), Height(height), State(GAME_ACTIVE), Keys()
{
    
}

Game::~Game()
{
    
}

void Game::Init() {
    ResourceManager::loadShader("shader/sprite.vs", "shader/sprite.fs", nullptr, "sprite");
    glm::mat4 projection = glm::mat4(1.0);
    projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::getShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::getShader("sprite").SetMatrix4("projection", projection);
    Renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));
    ResourceManager::loadTexture("img/awesomeface.png", GL_TRUE, "face");
    
    // load game texture
    ResourceManager::loadTexture("img/background.jpg", GL_FALSE, "background");
    ResourceManager::loadTexture("img/block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::loadTexture("img/block.png", GL_FALSE, "block");
    ResourceManager::loadTexture("img/paddle.png", GL_TRUE, "paddle");
    
    // load game level(bricks)
    GameLevel one;      one.Load("files/level_1", this->Width, this->Height * 0.5);
    GameLevel two;      two.Load("files/level_2", this->Width, this->Height * 0.5);
    GameLevel three;    three.Load("files/level_3", this->Width, this->Height * 0.5);
    GameLevel four;     four.Load("files/level_4", this->Width, this->Height * 0.5);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;
    
    glm::vec2 playerPos = glm::vec2((this->Width / 2) - (PLAYER_SIZE.x / 2), this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("paddle"));
    
    // initalize ball
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexture("face"));
    
    // initialize
    ResourceManager::loadShader("shader/particle.vs", "shader/particle.fs", nullptr, "particle");
    ResourceManager::loadTexture("img/particle.png", GL_TRUE, "particle");
    ResourceManager::getShader("particle").Use().SetInteger("sprite", 0);
    ResourceManager::getShader("particle").SetMatrix4("projection", projection);
    Particles = new ParticleGenerator(ResourceManager::getShader("particle") ,ResourceManager::getTexture("particle"), 500);
    
}

void Game::ProcessInput(GLfloat dt)
{
    if(this->State == GAME_ACTIVE){
        GLfloat velocity = PLAYER_VELOCITY * dt;
        if(this->Keys[GLFW_KEY_A]){
            if(Player->Position.x >= 0.0){
                Player->Position.x -= velocity;
                if(Ball->Stuck)
                    Ball->Position.x -= velocity;
            }
        }
        if(this->Keys[GLFW_KEY_D]){
            if(Player->Position.x <= this->Width - Player->Size.x){
                Player->Position.x += velocity;
                if(Ball->Stuck)
                    Ball->Position.x += velocity;
            }
        }
        if(this->Keys[GLFW_KEY_SPACE]){
            Ball->Stuck = false;
        }
    }
}

void Game::Update(GLfloat dt)
{
    Ball->Move(dt, this->Width);
    this->DoCollisions();
    if(Ball->Position.y >= this->Height){
        this->ResetLevel();
        this->ResetPlayer();
    }
    Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));
}

void Game::Render() {
    if(this->State == GAME_ACTIVE){
        Renderer->DrawSprite(ResourceManager::getTexture("background"), glm::vec2(0.0, 0.0), glm::vec2(this->Width, this->Height), 0.0f);
        this->Levels[this->Level].Draw(*Renderer);
        Player->Draw(*Renderer);
        Particles->Draw();
        Ball->Draw(*Renderer);
    }
}

void Game::DoCollisions(){
    Collision result = CheckCollision(*Ball, *Player);
    if(!Ball->Stuck && get<0>(result)){
        GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
        GLfloat distance = (Ball->Position.x + Ball->Radius - centerBoard);
        GLfloat percentage = distance / (Player->Size.x / 2);
        
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        Ball->Velocity.y = -abs(Ball->Velocity.y);
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
    }
    for(GameObject &brick : this->Levels[this->Level].Bricks){
        if(!brick.Destroyed){
            Collision collision = CheckCollision(*Ball, brick);
            if(get<0>(collision)){

                // destory brick
                if(brick.IsSolid == GL_FALSE){
                    brick.Destroyed = GL_TRUE;
                }
                
                // move the ball and change direction of the velocity
                Direction direction = get<1>(collision);
                glm::vec2 diff_vector = get<2>(collision);
                if(direction == LEFT || direction == RIGHT){
                    GLfloat peneration = Ball->Radius - abs(diff_vector.x);
                    if(direction == LEFT){
                        Ball->Velocity.x = -abs(Ball->Velocity.x);
                        Ball->Position.x -= peneration;
                    }
                    else{
                        Ball->Velocity.x = abs(Ball->Velocity.x);
                        Ball->Position.x += peneration;
                    }
                }
                else{
                    GLfloat peneration = Ball->Radius - abs(diff_vector.y);
                    if(direction == UP){
                        Ball->Velocity.y = abs(Ball->Velocity.y);
                        Ball->Position.y += peneration;
                    }
                    else {
                        Ball->Velocity.y = -abs(Ball->Velocity.y);
                        Ball->Position.y -= peneration;
                    }
                }
                
            }
        }
    }
}

void Game::ResetLevel()
{
    switch (this->Level) {
        case 0:
            this->Levels[this->Level].Load("files/level_1", this->Width, this->Height * 0.5);
            break;
        case 1:
            this->Levels[this->Level].Load("files/level_2", this->Width, this->Height * 0.5);
            break;
        case 2:
            this->Levels[this->Level].Load("files/level_3", this->Width, this->Height * 0.5);
            break;
        case 3:
            this->Levels[this->Level].Load("files/level_4", this->Width, this->Height * 0.5);
            break;
    }
}
void Game::ResetPlayer(){
    Player->Position = glm::vec2((this->Width / 2) - (PLAYER_SIZE.x / 2), this->Height - PLAYER_SIZE.y);
    Player->Size = PLAYER_SIZE;
    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}
