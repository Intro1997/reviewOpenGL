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
#include "Shader.hpp"

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
    void Draw(Shader shader){
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
        vector<Texture> texture_specular = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_specular");
        textures.insert(textures.end(), texture_specular.begin(), texture_specular.end());

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






//
//#pragma once
//// Std. Includes
//#include <string>
//#include <fstream>
//#include <sstream>
//#include <iostream>
//#include <map>
//#include <vector>
//using namespace std;
//// GL Includes
//#include <glad/glad.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//
//#include "Mesh.h"
//
//GLint TextureFromFile(const char* path, string directory);
//
//class Model
//{
//public:
//    /*  Functions   */
//    // Constructor, expects a filepath to a 3D model.
//    Model(GLchar* path)
//    {
//        this->loadModel(path);
//    }
//
//    // Draws the model, and thus all its meshes
//    void Draw(Shader shader)
//    {
//        for(GLuint i = 0; i < this->meshes.size(); i++)
//            this->meshes[i].Draw(shader);
//    }
//
//private:
//    /*  Model Data  */
//    vector<Mesh> meshes;
//    string directory;
//    vector<Texture> textures_loaded;    // Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
//
//    /*  Functions   */
//    // Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
//    void loadModel(string path)
//    {
//        // Read file via ASSIMP
//        Assimp::Importer importer;
//        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
//        // Check for errors
//        if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
//        {
//            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
//            return;
//        }
//        // Retrieve the directory path of the filepath
//        this->directory = path.substr(0, path.find_last_of('/'));
//
//        // Process ASSIMP's root node recursively
//        this->processNode(scene->mRootNode, scene);
//    }
//
//    // Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
//    void processNode(aiNode* node, const aiScene* scene)
//    {
//        // Process each mesh located at the current node
//        for(GLuint i = 0; i < node->mNumMeshes; i++)
//        {
//            // The node object only contains indices to index the actual objects in the scene.
//            // The scene contains all the data, node is just to keep stuff organized.
//            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//            this->meshes.push_back(this->processMesh(mesh, scene));
//        }
//        // After we've processed all of the meshes (if any) we then recursively process each of the children nodes
//        for(GLuint i = 0; i < node->mNumChildren; i++)
//        {
//            // Child nodes are actually stored in the node, not in the scene (which makes sense since nodes only contain
//            // links and indices, nothing more, so why store that in the scene)
//            this->processNode(node->mChildren[i], scene);
//        }
//
//    }
//
//    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
//    {
//        // Data to fill
//        vector<Vertex> vertices;
//        vector<GLuint> indices;
//        vector<Texture> textures;
//
//        // Walk through each of the mesh's vertices
//        for(GLuint i = 0; i < mesh->mNumVertices; i++)
//        {
//            Vertex vertex;
//            glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
//            // Positions
//            vector.x = mesh->mVertices[i].x;
//            vector.y = mesh->mVertices[i].y;
//            vector.z = mesh->mVertices[i].z;
//            vertex.Position = vector;
//            // Normals
//            vector.x = mesh->mNormals[i].x;
//            vector.y = mesh->mNormals[i].y;
//            vector.z = mesh->mNormals[i].z;
//            vertex.Normal = vector;
//            // Texture Coordinates
//            if(mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
//            {
//                glm::vec2 vec;
//                // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
//                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
//                vec.x = mesh->mTextureCoords[0][i].x;
//                vec.y = mesh->mTextureCoords[0][i].y;
//                vertex.TexCoords = vec;
//            }
//            else
//                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
//            vertices.push_back(vertex);
//        }
//        // Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
//        for(GLuint i = 0; i < mesh->mNumFaces; i++)
//        {
//            aiFace face = mesh->mFaces[i];
//            // Retrieve all indices of the face and store them in the indices vector
//            for(GLuint j = 0; j < face.mNumIndices; j++)
//                indices.push_back(face.mIndices[j]);
//        }
//        // Process materials
//        if(mesh->mMaterialIndex >= 0)
//        {
//            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//            // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
//            // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
//            // Same applies to other texture as the following list summarizes:
//            // Diffuse: texture_diffuseN
//            // Specular: texture_specularN
//            // Normal: texture_normalN
//
//            // 1. Diffuse maps
//            vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
//            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//            // 2. Specular maps
//            vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
//            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//            // 3. Reflection maps (Note that ASSIMP doesn't load reflection maps properly from wavefront objects, so we'll cheat a little by defining the reflection maps as ambient maps in the .obj file, which ASSIMP is able to load)
//            vector<Texture> reflectionMaps = this->loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_reflection");
//            textures.insert(textures.end(), reflectionMaps.begin(), reflectionMaps.end());
//        }
//
//        // Return a mesh object created from the extracted mesh data
//        return Mesh(vertices, indices, textures);
//    }
//
//    // Checks all material textures of a given type and loads the textures if they're not loaded yet.
//    // The required info is returned as a Texture struct.
//    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
//    {
//        vector<Texture> textures;
//        for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
//        {
//            aiString str;
//            mat->GetTexture(type, i, &str);
//            // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
//            GLboolean skip = false;
//            for(GLuint j = 0; j < textures_loaded.size(); j++)
//            {
//                if(textures_loaded[j].path == str)
//                {
//                    textures.push_back(textures_loaded[j]);
//                    skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
//                    break;
//                }
//            }
//            if(!skip)
//            {   // If texture hasn't been loaded already, load it
//                Texture texture;
//                texture.id = TextureFromFile(str.C_Str(), this->directory);
//                texture.type = typeName;
//                texture.path = str;
//                textures.push_back(texture);
//                this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
//            }
//        }
//        return textures;
//    }
//};
//
//
//
//
//GLint TextureFromFile(const char* path, string directory)
//{
//    //Generate texture ID and load texture data
//    string filename = string(path);
//    filename = directory + '/' + filename;
//    GLuint textureID;
//    glGenTextures(1, &textureID);
//    int width,height,urChannel;
//    unsigned char* image = stbi_load(filename.c_str(), &width, &height, &urChannel, 0);
//    // Assign texture to ID
//    glBindTexture(GL_TEXTURE_2D, textureID);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//    glGenerateMipmap(GL_TEXTURE_2D);
//
//    // Parameters
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glBindTexture(GL_TEXTURE_2D, 0);
//    stbi_image_free(image);
//    return textureID;
//}
