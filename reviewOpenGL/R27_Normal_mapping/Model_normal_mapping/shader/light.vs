#version 330 core
layout(location = 0) in vec3 lightPos;
layout(location = 1) in vec3 lightNormal;
layout(location = 2) in vec3 lightTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(lightPos, 1.0);
}
