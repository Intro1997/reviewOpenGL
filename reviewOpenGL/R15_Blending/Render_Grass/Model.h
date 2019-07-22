//
//  Model.h
//  OpenGLTest
//
//  Created by Outro on 2019/7/14.
//  Copyright © 2019 Outro.com. All rights reserved.
//

#ifndef Model_h
#define Model_h
#include "Mesh.h"
#include "ReviewShader.h"
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb-master/stb_image.h>

vector<Texture> loaded_textures;

class Model{
public:
    string directory;
    vector<Mesh> meshes;
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
        const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_FlipUVs | aiProcess_Triangulate);
        if(!scene && scene->mFlags | AI_SCENE_FLAGS_INCOMPLETE && !scene->mRootNode){
            cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
            return;
        }
        directory = path.substr(0, path.find_last_of("/"));
        processNode(scene->mRootNode, scene);
    }
    void processNode(aiNode* node, const aiScene* scene){
        for(int i = 0; i < node->mNumMeshes; i++){
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        for(int i = 0; i < node->mNumChildren; i++){
            processNode(node->mChildren[i], scene);
        }
    }
    Mesh processMesh(aiMesh* mesh, const aiScene* scene){
        vector<Vertex> vertices;
        vector<Texture> textures;
        vector<unsigned int> indices;
        
        for(int i = 0; i < mesh->mNumVertices; i++){
            Vertex vertex;
            vertex.Position.x = mesh->mVertices[i].x;
            vertex.Position.y = mesh->mVertices[i].y;
            vertex.Position.z = mesh->mVertices[i].z;
            
            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;
            
            if(mesh->mTextureCoords[0]){
                vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
            }
            vertices.push_back(vertex);
        }
        
        for(int i = 0; i < mesh->mNumFaces; i++){
            aiFace face = mesh->mFaces[i];
            for(int j = 0; j < face.mNumIndices; j++){
                indices.push_back(face.mIndices[j]);
            }
        }
        
        aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuse_textures = loadMaterialTexture(mat, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuse_textures.begin(), diffuse_textures.end());
        
        vector<Texture> specular_textures = loadMaterialTexture(mat, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specular_textures.begin(), specular_textures.end());
        return Mesh(vertices, textures, indices);
        
    }
    vector<Texture> loadMaterialTexture(aiMaterial *mat, aiTextureType type, string Typename){
        vector<Texture> Textures;
        for(int i = 0; i < mat->GetTextureCount(type); i++){
            bool skip = false;
            aiString str;
            string name = Typename;
            mat->GetTexture(type, i, &str);
            for(int j = 0; j < loaded_textures.size(); j++){
                if(strcmp(loaded_textures[i].type.c_str(), str.C_Str()) == 0){
                    Textures.push_back(loaded_textures[j]);
                    skip = true;
                    break;
                }
            }
            if(!skip){
                Texture tex;
                tex.id = loadTextureFromFile(str.C_Str(), name);
                tex.path = str.C_Str();
                tex.type = name;
                Textures.push_back(tex);
                loaded_textures.push_back(tex);
            }
        }
        return Textures;
    }
    
    unsigned int loadTextureFromFile(string path, string name){
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        string filename = this->directory + "/" + path;
        int width, height, urChannel;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &urChannel, 0);
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
            cout << "ERROR::STBI_LOAD::Read texture failed" << endl;
            stbi_image_free(data);
        }
        return textureID;
    }    
};
#endif /* Model_h */
