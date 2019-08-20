//
//  Model.h
//  OpenGLTest
//
//  Created by Outro on 2019/7/14.
//  Copyright © 2019 Outro.com. All rights reserved.
//

#ifndef Model_h
#define Model_h

#include <iostream>
using namespace std;

#include <string>

#include <vector>
#include "Mesh.h"
#include "ReviewShader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb-master/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

vector<Texture> loaded_textures;

class Model{
public:
    vector<Mesh> meshes;
    string directory;

    Model(char *path){
        loadModel(path);
    }
    void Draw(ReviewShader shader){
        for(int i = 0; i < meshes.size(); i++){
            meshes[i].Draw(shader);
        }
    }
private:
    void loadModel(string path){
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_FlipUVs|aiProcess_Triangulate);
        if(!scene && scene->mFlags | AI_SCENE_FLAGS_INCOMPLETE && !scene->mRootNode){
            cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
            return;
        }
        directory = path.substr(0, path.find_last_of("/"));
        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode *node, const aiScene *scene){
        for(int i = 0; i < node->mNumMeshes; i++){
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        for(int i = 0; i < node->mNumChildren; i++){
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene){
        vector<Vertex> vertices;
        vector<Texture> textures;
        vector<unsigned int> indices;

        for(int i = 0; i < mesh->mNumVertices; i++){
            Vertex vertex;
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;

            if(mesh->mTextureCoords[0]){
                glm::vec2 tex;
                tex.x = mesh->mTextureCoords[0][i].x;
                tex.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoord = tex;
            }
            else{
                vertex.TexCoord = glm::vec2(0.0, 0.0);
            }
            vertices.push_back(vertex);
        }

        for(int i = 0; i < mesh->mNumFaces; i++){
            aiFace face = mesh->mFaces[i];
            for(int j = 0; j < face.mNumIndices; j++){
                indices.push_back(face.mIndices[j]);
            }
        }

        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> texture_diffuse = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), texture_diffuse.begin(), texture_diffuse.end());
        vector<Texture> texture_specular = loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), texture_specular.begin(), texture_specular.end());
        vector<Texture> texture_ambient = loadMaterialTexture(material, aiTextureType_AMBIENT, "texture_reflection");
        textures.insert(textures.end(), texture_ambient.begin(), texture_ambient.end());

        return Mesh(vertices, textures, indices);
    }

    vector<Texture> loadMaterialTexture(aiMaterial *material, aiTextureType type, string Typename){
        vector<Texture> textures;
        for(int i = 0; i < material->GetTextureCount(type); i++){
            bool skip = false;
            aiString str;
            material->GetTexture(type, i, &str);
            for(int j = 0; j < loaded_textures.size(); j++){
                if(strcmp(loaded_textures[j].path.c_str(), str.C_Str()) == 0){
                    skip = true;
                    textures.push_back(loaded_textures[i]);
                    break;
                }
            }

            if(!skip){
                Texture tex;
                tex.id = loadTextureFromFile(str.C_Str(), directory);
                tex.type = Typename;
                tex.path = str.C_Str();
                textures.push_back(tex);
                loaded_textures.push_back(tex);
            }
        }
        return textures;
    }

    unsigned int loadTextureFromFile(string path, string &Directory){
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        string filepath = Directory + "/" + path;
        int width, height, urChannel;
        unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &urChannel, 0);
        if(data){
            GLenum format;
            if(urChannel == 1)
                format = GL_RED;
            if(urChannel == 3)
                format = GL_RGB;
            if(urChannel == 4)
                format = GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }
        else{
            cout << "Read texture Data failed\n";
            stbi_image_free(data);
        }
        return textureID;
    }
};



#endif /* Model_h */


