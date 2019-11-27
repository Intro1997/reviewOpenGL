//
//  SpriteRenderer.hpp
//  OpenGLTest
//
//  Created by Outro on 2019/11/20.
//  Copyright © 2019 Outro.com. All rights reserved.
//

#ifndef SpriteRenderer_hpp
#define SpriteRenderer_hpp

#include <GL/gl3.h> // only for hint of function
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Texture.hpp"
#include "Shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

/*
 Responsibility:
 Variable:
    1. Define sprite render shader
    2. Define vertex array object render the vertex info
 
 Function:
    1. SpriteRenderer(Shader shader)
        [initialize shader and call initRenderData()]
    2. ~SpriteRenderer()
        [call glDeleteVertexArrays(1, &this->quadVAO)]
    3. SpriteRenderer::DrawSprite(Texture2D texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
        [bind shader, VAO and texture. Then render sprite]
    4. void initRenderData()
        [initialize vertex array object]
 */

class SpriteRenderer {
public:
    SpriteRenderer(Shader shader);
    ~SpriteRenderer();
    void DrawSprite(Texture2D texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
    Shader shader;
    GLuint quadVAO;
    void initRenderData();
};

#endif /* SpriteRenderer_hpp */
