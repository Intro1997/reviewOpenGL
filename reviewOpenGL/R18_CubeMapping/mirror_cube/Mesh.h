//
//  Mesh.h
//  OpenGLTest
//
//  Created by Outro on 2019/7/14.
//  Copyright © 2019 Outro.com. All rights reserved.
//

#ifndef Mesh_h
#define Mesh_h
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture{
    unsigned int id;
    string path;
    string type;
};

class Mesh{
public:
    vector<Vertex> vertices;
    vector<Texture> textures;
    vector<unsigned int> indices;
    unsigned int VAO;
    
    Mesh(vector<Vertex> vertices, vector<Texture> textures, vector<unsigned int> indices){
        this->vertices = vertices;
        this->textures = textures;
        this->indices = indices;
        
        setupMesh();
    }
    
    void Draw(ReviewShader shader){
        shader.use();
        int diffuseCount = 1;
        int specularCount = 1;
        
        for(int i = 0; i < textures.size(); i++){
            glActiveTexture(GL_TEXTURE0 + i);
            string name = textures[i].type;
            string num;
            if(name == "texture_diffuse")
                num = to_string(diffuseCount++);
            else if(name == "texture_specular")
                num = to_string(specularCount++);
            shader.setInt((name+num).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }
private:
    unsigned int VBO, EBO;
    void setupMesh(){
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        glEnableVertexAttribArray(2);
        
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        
        glBindVertexArray(0);
    }
};
#endif /* Mesh_h */
