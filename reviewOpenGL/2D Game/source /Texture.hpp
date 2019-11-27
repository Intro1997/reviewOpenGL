//
//  Texture.hpp
//  OpenGLTest
//
//  Created by Outro on 2019/11/19.
//  Copyright © 2019 Outro.com. All rights reserved.
//

/*
 Responsibility
 Variables:
     1. Define texture ID store the texture
     2. Define texture size variable
     3. Define texture loading formats
     4. Define texture loading parameters
 
 Function:
     1. Texture2D()
        [Initialize the variables]
     2. void Generate(GLuint width, GLuint height, unsigned char* data)
        [Generate the texture according to variables]
     3. void Bind() const
        [Bind current texture]
     4. void Unbind() const
        [Unbind current texture]
 */


#ifndef Texture_hpp
#define Texture_hpp

#include <iostream>
#include <GL/gl3.h> // only for hint of function
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;


class Texture2D {
public:
    GLuint ID;
    GLuint Width, Height;
    GLenum Internel_Format, Image_Format;
    GLenum Wrap_S, Wrap_T;
    GLenum Filter_Min, Filter_Max;
    
    Texture2D();
    void Generate(GLuint width, GLuint height, unsigned char* data);
    void Bind() const;
    void Unbind() const;
    
};

#endif /* Texture_hpp */
