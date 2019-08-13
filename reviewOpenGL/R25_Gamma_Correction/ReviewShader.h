//
//  ReviewShader.h
//  OpenGLTest
//
//  Created by Outro on 2019/7/7.
//  Copyright © 2019 Outro.com. All rights reserved.
//

#ifndef ReviewShader_h
#define ReviewShader_h

#include <iostream>
using namespace std;

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ReviewShader{
public:
    unsigned int ID; // used as program to link the shaders
    
    ReviewShader(const char *vertexPath, const char *fragmentPath){
        ifstream vertexFileStream, fragmentFileStream;
        
        vertexFileStream.exceptions(ifstream::failbit | ifstream::badbit);
        fragmentFileStream.exceptions(ifstream::failbit | ifstream::badbit);
        
        string vertexStr, fragmentStr;
        
        try {
            vertexFileStream.open(vertexPath);
            fragmentFileStream.open(fragmentPath);
            
            stringstream vertexStrStream, fragmentStrStream;
            
            vertexStrStream << vertexFileStream.rdbuf();
            fragmentStrStream << fragmentFileStream.rdbuf();
            
            vertexFileStream.close();
            fragmentFileStream.close();
            
            vertexStr = vertexStrStream.str();
            fragmentStr = fragmentStrStream.str();
        } catch (ifstream::failure e) {
            cout << "Open shader files failed\n";
        }
        const char *vertexCode;
        const char *fragmentCode;
        vertexCode = vertexStr.c_str();
        fragmentCode = fragmentStr.c_str();
        
        int success;
        char infoLog[512];
        
        unsigned int vShader;
        vShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vShader, 1, &vertexCode, NULL);
        glCompileShader(vShader);
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(vShader, 512, NULL, infoLog);
            cout << "Compile vertex shader failed\n" << infoLog << endl;
        }
        
        unsigned int fShader;
        fShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fShader, 1, &fragmentCode, NULL);
        glCompileShader(fShader);
        glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(fShader, 512, NULL, infoLog);
            cout << "Compile fragment shader failed\n" << infoLog << endl;
        }
        
        ID = glCreateProgram();
        glAttachShader(ID, vShader);
        glAttachShader(ID, fShader);
        glLinkProgram(ID);
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            cout << "Link program failed\n" << infoLog << endl;
        }
        glDeleteShader(vShader);
        glDeleteShader(fShader);
    }
    
    void use(){
        glUseProgram(ID);
    }
    
    void set4f(string name, float v0, float v1, float v2, float v3){
        glUniform4f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2, v3);
    }
    
    void setInt(string name, int v0){
        glUniform1i(glGetUniformLocation(ID, name.c_str()), v0);
    }
    
    void setMat4(string name, glm::mat4 mat){
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
    
    void setVec3(string name, glm::vec3 vec){
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
    }
    
    void setVec3(string name, float v0, float v1, float v2){
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(glm::vec3(v0, v1, v2)));
    }
    
    void setFloat(string name, float v0){
        glUniform1f(glGetUniformLocation(ID, name.c_str()), v0);
    }        
};

#endif /* ReviewShader_h */
