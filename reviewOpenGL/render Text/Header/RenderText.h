//
//  RenderText.h
//  OpenGLTest
//
//  Created by Outro on 2019/11/18.
//  Copyright © 2019 Outro.com. All rights reserved.
//

#ifndef RenderText_h
#define RenderText_h

#include <iostream>
using namespace std;

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GL/gl3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ReviewShader.h"

#include <map>

class RenderText{
private:
    struct Character{
        GLuint textureID;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        GLuint Advance;
    };
    map<GLchar, Character> Characters;
    GLuint VAO, VBO;
    glm::mat4 projection;
    FT_Error error;
    
    void initCharacterMap(FT_Face face){
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        for(GLubyte c = 0; c < 128; c++){
            error = FT_Load_Char(face, c, FT_LOAD_RENDER);
            if(error){
                cout << "ERROR::FREETYPE: load character " << (char)c << " failed\n";
                continue;
            }
            
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            
            Character ch{
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<GLuint>(face->glyph->advance.x)
            };
            
            Characters.insert(pair<GLchar, Character>(c, ch));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    void initVertexArrayObject(){
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    
public:
    RenderText(FT_UInt pixelWidth, FT_UInt pixelHeight, const char* fontPath, int SCR_WIDTH, int SCR_HEIGHT){
        FT_Library ft;
        FT_Face face;
        
        error = FT_Init_FreeType(&ft);
        if(error){
            cout << "ERROR::FREESTYLE::errno(" << error << "):: Initialize FREETYPE library failed\n";
            return;
        }
                    
        error = FT_New_Face(ft, fontPath, 0, &face);
        if(error){
            cout << "ERROR::FREESTYLE::errno(" << error << "):: Initialize FREETYPE face failed\n";
            return;
        }
        
        error = FT_Set_Pixel_Sizes(face, pixelWidth, pixelHeight);
        if(error){
            cout << "ERROR::FREESTYLE::errno(" << error << "):: Set FREETYPE pixel size failed\n";
            return;
        }
        
        projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT);
     
        initCharacterMap(face);
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        
        initVertexArrayObject();
    }
    
    /// render the text on screen
    /// @param text rendered text
    /// @param scale scale the text size
    /// @param shader shader: projection matrix in vertex shader; text color [vec3] and text texture [sampler2D] in fragment shader
    void renderString(string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 textColor, ReviewShader shader){
        shader.use();
        shader.setMat4("projection", projection);
        shader.setVec3("textColor", textColor);
        shader.setInt("characterTexture", 0);
        glActiveTexture(GL_TEXTURE0);

        for(string::iterator str = text.begin(); str != text.end(); str++){
            Character ch = Characters[*str];
            GLfloat xpos = x + ch.Bearing.x * scale;
            GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
            GLfloat w = ch.Size.x * scale;
            GLfloat h = ch.Size.y * scale;

            float vertices[6][4]{
                {xpos,     ypos + h, 0.0, 0.0},
                {xpos,     ypos,     0.0, 1.0},
                {xpos + w, ypos,     1.0, 1.0},
                {xpos,     ypos + h, 0.0, 0.0},
                {xpos + w, ypos,     1.0, 1.0},
                {xpos + w, ypos + h, 1.0, 0.0}
            };

            glBindVertexArray(VAO);
            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glDrawArrays(GL_TRIANGLES, 0, 6);
            x += (ch.Advance >> 6) * scale;
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};

#endif /* RenderText_h */
