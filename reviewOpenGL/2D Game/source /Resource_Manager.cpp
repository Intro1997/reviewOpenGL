//
//  Resource_Manager.cpp
//  OpenGLTest
//
//  Created by Outro on 2019/11/19.
//  Copyright © 2019 Outro.com. All rights reserved.
//

#include "Resource_Manager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb-master/stb_image.h>
map<string, Shader> ResourceManager::Shaders;
map<string, Texture2D> ResourceManager::Textures;

Shader ResourceManager::loadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::getShader(string name)
{
    return Shaders[name];
}

Texture2D ResourceManager::loadTexture(const GLchar *file, GLboolean alpha, string name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D ResourceManager::getTexture(string name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    for(auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    for(auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile)
{
    ifstream vertexFileStream, fragmentFileStream, geometryFileStream;
    
    vertexFileStream.exceptions(ifstream::failbit | ifstream::badbit);
    fragmentFileStream.exceptions(ifstream::failbit | ifstream::badbit);
    geometryFileStream.exceptions(ifstream::failbit | ifstream::badbit);
    
    string vertexStr, fragmentStr, geometryStr;
    
    try {
        stringstream vertexStrStream, fragmentStrStream, geometryStrStream;
        vertexFileStream.open(vShaderFile);
        vertexStrStream << vertexFileStream.rdbuf();
        vertexFileStream.close();
        vertexStr = vertexStrStream.str();
        
        fragmentFileStream.open(fShaderFile);
        fragmentStrStream << fragmentFileStream.rdbuf();
        fragmentFileStream.close();
        fragmentStr = fragmentStrStream.str();
        
        if(gShaderFile != nullptr){
            geometryFileStream.open(gShaderFile);
            geometryStrStream << geometryFileStream.rdbuf();
            geometryFileStream.close();
            geometryStr = geometryStrStream.str();
        }
    } catch (ifstream::failure e) {
        cout << "Open shader [ " << vShaderFile << " or " << fShaderFile << " ] files failed\n";
    }
    const char *vertexCode;
    vertexCode = vertexStr.c_str();
    
    const char *fragmentCode;
    fragmentCode = fragmentStr.c_str();

    const char *geometryCode;
    if(gShaderFile != nullptr)
        geometryCode = geometryStr.c_str();
    else
        geometryCode = nullptr;
    Shader shader;
    shader.Compile(vertexCode, vShaderFile, fragmentCode, fShaderFile, geometryCode, gShaderFile);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha)
{
    Texture2D texture;
    if(alpha)
    {
        texture.Internel_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }

    int width, height, urChannel;
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned char* image = stbi_load(file, &width, &height, &urChannel, 0);
    if(image){
        texture.Generate(width, height, image);
    }
    else
        cout << "ERROR::Load image texture[" << file << "] failed." << endl;
    stbi_image_free(image);
    return texture;
}
