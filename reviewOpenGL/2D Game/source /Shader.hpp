//
//  Shader.hpp
//  OpenGLTest
//
//  Created by Outro on 2019/7/7.
//  Copyright © 2019 Outro.com. All rights reserved.
//

/*
 Responsibility
 Variable:
    1. Define the shader program ID.
 
 Function:
     1. constructor Shader()
        [do nothing]
     2. Shader  &Use()
        [use this shader]
     3. void Compile(const GLchar *vertexSource, const GLchar *vShaderPath, const GLchar *fragmentSource, const GLchar *fShaderPath, const GLchar *geometrySource, const GLchar *gShaderPath)
        [compile the shader]
     4. void checkCompileErrors(GLuint object, std::string type)
        [print message if error occurred]
     5. A series of set shader value function
 */

#ifndef Shader_hpp
#define Shader_hpp

#include <iostream>
using namespace std;

#include <GL/gl3.h> // only for hint of function
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Shader{
public:
    // State
    GLuint ID;
    // Constructor
    Shader() { }
    // Sets the current shader as active
    Shader  &Use();
    // Compiles the shader from given source code
    void Compile(const GLchar *vertexSource, const GLchar *vShaderPath, const GLchar *fragmentSource, const GLchar *fShaderPath, const GLchar *geometrySource, const GLchar *gShaderPath);
    // Note: geometry source code is optional
    // Utility functions
    void    SetFloat    (const GLchar *name, GLfloat value, GLboolean useShader = false);
    void    SetInteger  (const GLchar *name, GLint value, GLboolean useShader = false);
    void    SetVector2f (const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
    void    SetVector2f (const GLchar *name, const glm::vec2 &value, GLboolean useShader = false);
    void    SetVector3f (const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
    void    SetVector3f (const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);
    void    SetVector4f (const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
    void    SetVector4f (const GLchar *name, const glm::vec4 &value, GLboolean useShader = false);
    void    SetMatrix4  (const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);
private:
    // Checks if compilation or linking failed and if so, print the error logs
    void    checkCompileErrors(GLuint object, std::string type);

};

#endif /* Shader_h */
