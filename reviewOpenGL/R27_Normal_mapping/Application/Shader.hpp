#ifndef Shader_hpp
#define Shader_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

class Shader{
public:
    unsigned int ID; // Use to link the vertex shader and the fragment shader

    /*
     Constructor:
     paramater: vertex file path, fragment file path
     function: Initialize the vertex and fragment shaders by reading file
    */
    Shader(const char* vertexPath, const char* fragmentPath){
        ifstream vertexFile, fragmentFile;                              // Create file reader
        vertexFile.exceptions(ifstream::badbit | ifstream::failbit);    // Set fail reading status
        fragmentFile.exceptions(ifstream::badbit | ifstream::failbit);  // Set fail reading status

        string vertexCode, fragmentCode;                                // Get string format contents in shader files

        try {                                                           // try to read shader files
            vertexFile.open(vertexPath);                                // open the file
            fragmentFile.open(fragmentPath);                            // open the file

            stringstream vertexSst, fragmentSst;                        // get buffer data (the return formate is stringstream) in file
            vertexSst << vertexFile.rdbuf();
            fragmentSst << fragmentFile.rdbuf();

            vertexFile.close();                                         // close the read stream after using
            fragmentFile.close();

            vertexCode = vertexSst.str();                               // tranfer the stringstream format to string
            fragmentCode = fragmentSst.str();

        } catch (ifstream::failure e) {                                 // throw the exception if reading failed
            cout << "Reading shader files failed\n";
        }

        const char* vCode, *fCode;
        vCode = vertexCode.c_str();     // transfer the format of shader code
        fCode = fragmentCode.c_str();

        int success;                    // set sign
        char infoLog[512];     // get error infolog

        GLuint vShader, fShader;        // define the shader

        vShader = glCreateShader(GL_VERTEX_SHADER);                 // create vertex shader
        glShaderSource(vShader, 1, &vCode, NULL);                   // configure the vertex shader source
        glCompileShader(vShader);                                   // compile the vertex shader
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);        // get the vertex shader compile status
        if(!success){
            glGetShaderInfoLog(vShader, 512, NULL, infoLog);        // get error infomation
            cout << "Compile Vertex Shader Failed \n" << infoLog << endl;
        }

        fShader = glCreateShader(GL_FRAGMENT_SHADER);               // create fragment shader
        glShaderSource(fShader, 1, &fCode, NULL);                   // configure the fragment shader source
        glCompileShader(fShader);                                   // Compile the fragment shader code
        glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);        // get the shader compile status
        if(!success){
            glGetShaderInfoLog(fShader, 512, NULL, infoLog);        // get the error information
            cout << "Compile Fragment Shader Failed \n" << infoLog << endl;
        }

        ID = glCreateProgram();
        glAttachShader(ID, vShader);                        // Attach the vertex shader
        glAttachShader(ID, fShader);                        // Attach the fragment shader
        glLinkProgram(ID);                                  // Link the program
        glGetProgramiv(ID, GL_LINK_STATUS, &success);       // get the link status
        if(!success){
            glGetProgramInfoLog(ID, 512, NULL, infoLog);    // get error infomation
            cout << "Link Shader Program Failed " << infoLog << endl;
        }

        glDeleteShader(vShader);    // release the shader object to optimize the memory
        glDeleteShader(fShader);
    }
    
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath){
        ifstream vertexFile, fragmentFile, geometryFile;                              // Create file reader
        vertexFile.exceptions(ifstream::badbit | ifstream::failbit);    // Set fail reading status
        fragmentFile.exceptions(ifstream::badbit | ifstream::failbit);  // Set fail reading status
        geometryFile.exceptions(ifstream::badbit | ifstream::failbit);  // Set fail reading status

        string vertexCode, fragmentCode, geometryCode;                                // Get string format contents in shader files

        try {                                                           // try to read shader files
            vertexFile.open(vertexPath);                                // open the file
            fragmentFile.open(fragmentPath);                            // open the file
            geometryFile.open(geometryPath);

            stringstream vertexSst, fragmentSst, geometrySst;                        // get buffer data (the return formate is stringstream) in file
            vertexSst << vertexFile.rdbuf();
            fragmentSst << fragmentFile.rdbuf();
            geometrySst << geometryFile.rdbuf();

            vertexFile.close();                                         // close the read stream after using
            fragmentFile.close();
            geometryFile.close();

            vertexCode = vertexSst.str();                               // tranfer the stringstream format to string
            fragmentCode = fragmentSst.str();
            geometryCode = geometrySst.str();

        } catch (ifstream::failure e) {                                 // throw the exception if reading failed
            cout << "Reading shader files failed\n";
        }

        const char* vCode, *fCode, *gCode;
        vCode = vertexCode.c_str();     // transfer the format of shader code
        fCode = fragmentCode.c_str();
        gCode = geometryCode.c_str();

        int success;                    // set sign
        char infoLog[512];     // get error infolog

        GLuint vShader, fShader, gShader;        // define the shader

        vShader = glCreateShader(GL_VERTEX_SHADER);                 // create vertex shader
        glShaderSource(vShader, 1, &vCode, NULL);                   // configure the vertex shader source
        glCompileShader(vShader);                                   // compile the vertex shader
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);        // get the vertex shader compile status
        if(!success){
            glGetShaderInfoLog(vShader, 512, NULL, infoLog);        // get error infomation
            cout << "Compile Vertex Shader Failed \n" << infoLog << endl;
        }

        fShader = glCreateShader(GL_FRAGMENT_SHADER);               // create fragment shader
        glShaderSource(fShader, 1, &fCode, NULL);                   // configure the fragment shader source
        glCompileShader(fShader);                                   // Compile the fragment shader code
        glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);        // get the shader compile status
        if(!success){
            glGetShaderInfoLog(fShader, 512, NULL, infoLog);        // get the error information
            cout << "Compile Fragment Shader Failed \n" << infoLog << endl;
        }

        gShader = glCreateShader(GL_GEOMETRY_SHADER);               // create geometry shader
        glShaderSource(gShader, 1, &gCode, NULL);                   // configure the geometry shader source
        glCompileShader(gShader);                                   // Compile the geometry shader code
        glGetShaderiv(gShader, GL_COMPILE_STATUS, &success);        // get the shader compile status
        if(!success){
            glGetShaderInfoLog(gShader, 512, NULL, infoLog);        // get the error information
            cout << "Compile geometry Shader Failed \n" << infoLog << endl;
        }

        ID = glCreateProgram();
        glAttachShader(ID, vShader);                        // Attach the vertex shader
        glAttachShader(ID, fShader);                        // Attach the fragment shader
        glAttachShader(ID, gShader);                        // Attach the geometry shader
        glLinkProgram(ID);                                  // Link the program
        glGetProgramiv(ID, GL_LINK_STATUS, &success);       // get the link status
        if(!success){
            glGetProgramInfoLog(ID, 512, NULL, infoLog);    // get error infomation
            cout << "Link Shader Program Failed " << infoLog << endl;
        }

        glDeleteShader(vShader);    // release the shader object to optimize the memory
        glDeleteShader(fShader);
        glDeleteShader(gShader);
    }

    // use shader
    void use(){
        glUseProgram(ID);
    }

    void setInt(const string name, int value) const{
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const string name, float value) const{
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setVec3(const string name, float v1, float v2, float v3) const{
        glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
    }

    void setVec3(const string name, const glm::vec3 vector) const{
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector));
    }

    void setVec4(const string name, float v1, float v2, float v3, float v4) const{
        glUniform4f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4);
    }

    void setVec4(const string name, const glm::vec4 vector) const{
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector));
    }

    void setMat3(const string name, const glm::mat3& mat) const{
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4(const string name, const glm::mat4 mat) const{
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
};

#endif

