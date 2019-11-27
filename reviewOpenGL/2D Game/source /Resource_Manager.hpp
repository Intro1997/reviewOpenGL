//
//  Resource_Manager.hpp
//  OpenGLTest
//
//  Created by Outro on 2019/11/19.
//  Copyright © 2019 Outro.com. All rights reserved.
//

/*
 Resonsibility: static, needed by other class
 Variable:
     1. Define Shaders (name maps shader)
     2. Define Textures (name maps texture)
 
 Function:
     1. static Shader loadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, string name);
        [call loadShaderFromFile]
     2. static Shader getShader(string name);
     3. static Texture2D loadTexture(const GLchar* file, GLboolean alpha, string name);
        [call loadTextureFromFile]
     4. static Texture2D getTexture(string name);
     5. static void Clear();
     6. ResourceManager(){}
        [do nothing]
     7. static Shader loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);
     8. static Texture2D loadTextureFromFile(const GLchar* file, GLboolean alpha);
 */

#ifndef Resource_Manager_hpp
#define Resource_Manager_hpp

#include <map>
#include <string>

#include <GL/gl3.h> // only for hint of function
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Texture.hpp"
#include "Shader.hpp"

class ResourceManager{
public:
    static map<string, Shader> Shaders;
    static map<string, Texture2D> Textures;
    
    static Shader loadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, string name);
    static Shader getShader(string name);
    static Texture2D loadTexture(const GLchar* file, GLboolean alpha, string name);
    static Texture2D getTexture(string name);
    static void Clear();
private:
    ResourceManager(){}
    static Shader loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);
    static Texture2D loadTextureFromFile(const GLchar* file, GLboolean alpha);
};

#endif /* Resource_Manager_hpp */
