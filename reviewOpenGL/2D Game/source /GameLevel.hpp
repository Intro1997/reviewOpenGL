//
//  GameLevel.hpp
//  OpenGLTest
//
//  Created by Outro on 2019/11/21.
//  Copyright © 2019 Outro.com. All rights reserved.
//

#ifndef GameLevel_hpp
#define GameLevel_hpp

#include <iostream>
using namespace std;
#include <vector>

#include "GameObject.hpp"
#include "Resource_Manager.hpp"

/*
 Responsibility
 Variable:
    1. Define the Bricks
 
 Function:
    1. GameLevel()
        [do nothing]
    2. void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight)
        [load game level file]
    3. void Draw(SpriteRenderer &renderer)
        [render each brick in bricks array]
    4. GLboolean IsCompleted()
        [check if the level is finished]
    5. void init(vector<vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
        [Initialize the level. Include the bricks and level size]
 */

class GameLevel {
public:
    vector<GameObject> Bricks;
    GameLevel(){}
    void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
    void Draw(SpriteRenderer &renderer);
    GLboolean IsCompleted();
    
private:
    void init(vector<vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

#endif /* GameLevel_hpp */
