//
//  Game.hpp
//  OpenGLTest
//
//  Created by Outro on 2019/11/20.
//  Copyright © 2019 Outro.com. All rights reserved.
//

/*
 Responsibility:
    Variables:
        1. Define the game state
        2. Define the collision direction
        3. Define the collision infomation
        4. Store keyboard input
        5. Define windows size
        6. Store Level information
        7. Create SpriteRenderer (Type: SpriteRenderer *)
        8. Create Player (Type GameObject *) [Paddle]
        9. Create Ball (Type GameObject *)
        10. Create Particles (Type ParticleGenerator *)

 
    Functions:
         1. Direction VectorDirection(glm::vec2 target)
            [Check the collision direction]
         2. Collision CheckCollision(BallObject &one, GameObject &two)
            [Check if objects exit collision]
         3. Constructor: Game(int width, int height) [intialize window size, game state, keyboard input]
         4. Destructor: ~Game() [do nothing]
         5. void Init()
             [Load shaders, textures and levels by referencing ResourceManager class]
             [Initialize shaders, player, ball, levels and particle]
         6. void ProcessInput(GLfloat dt)
            [Define the action for keyboard input]
         7. void Update(GLfloat dt)
            [Update ball, collision and particle]
         8. void Render()
            [Render all sprites, levels, player, particles and ball]
         9. void Game::DoCollisions()
            [Define action for collision]
         10. ResetLevel()
            [reset the level]
         11. ResetPlayer()
            [reset ball and player]
 */

#ifndef Game_hpp
#define Game_hpp

#include <iostream>
#include <GL/gl3.h> // only for hint of function
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "SpriteRenderer.hpp"
#include "GameLevel.hpp"
#include "Ball.hpp"
#include "Particle.hpp"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef::tuple<GLboolean, Direction, glm::vec2>Collision;

class Game {
public:
    GameState   State;
    GLboolean   Keys[1024];
    GLuint      Width, Height;
    vector<GameLevel> Levels;
    GLuint Level;
    
    Game(GLuint width, GLuint height);
    ~Game();
    
    void Init();
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();
    void DoCollisions();
    void ResetLevel();
    void ResetPlayer();
};

#endif /* Game_hpp */
