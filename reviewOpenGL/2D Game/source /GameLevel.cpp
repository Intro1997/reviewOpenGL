//
//  GameLevel.cpp
//  OpenGLTest
//
//  Created by Outro on 2019/11/21.
//  Copyright © 2019 Outro.com. All rights reserved.
//

#include "GameLevel.hpp"
#include <fstream>
#include <sstream>

void GameLevel::Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight){
    ifstream fs(file);
    vector<vector<GLuint>> tileData;
    if(fs){
        string line;
        GLuint tileCode;

        while(getline(fs, line)){
            istringstream ss(line);
            vector<GLuint> row;
            while(ss >> tileCode){
                row.push_back(tileCode);        
            }
            tileData.push_back(row);
        }
        
        if(tileData.size() > 0){
            this->init(tileData, levelWidth, levelHeight);
        }

    }
    else{
        cout << "Load file[" << file << "] failed\n" << endl;
    }
}

void GameLevel::Draw(SpriteRenderer &renderer){
    for (GameObject &tile : this->Bricks) {
        if(!tile.Destroyed)
            tile.Draw(renderer);
    }
}

GLboolean GameLevel::IsCompleted(){
    for (GameObject &tile : this->Bricks) {
        if(!tile.IsSolid && !tile.Destroyed)
            return false;
    }
    return true;
}

void GameLevel::init(vector<vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight) {
    // get bricks' height and width
    GLfloat height, width;
    height = tileData.size();
    width = tileData[0].size();
    
    // get each brick's height and width
    GLfloat unit_width, unit_height;
    unit_width = (float)levelWidth / width;
    unit_height = (float)levelHeight / height;
    
    // set position, size, color and game object of each brick
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            if(tileData[y][x] == 1){
                glm::vec2 pos = glm::vec2((float)x * unit_width, (float)y * unit_height);
                glm::vec2 size = glm::vec2(unit_width, unit_height);
                glm::vec3 color = glm::vec3(0.8f, 0.8f, 0.7f);
                Texture2D texture = ResourceManager::getTexture("block_solid");
                GameObject obj = GameObject(pos, size, texture, color);

                obj.IsSolid = GL_TRUE;
                this->Bricks.push_back(obj);
            }
            else if(tileData[y][x] > 1){
                glm::vec2 pos = glm::vec2((float)x * unit_width, (float)y * unit_height);
                glm::vec2 size = glm::vec2(unit_width, unit_height);
                glm::vec3 color = glm::vec3(1.0f);
                if (tileData[y][x] == 2)
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (tileData[y][x] == 3)
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (tileData[y][x] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[y][x] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);
                Texture2D texture = ResourceManager::getTexture("block");
                this->Bricks.push_back(GameObject(pos, size, texture, color));
            }
        }
    }
}
