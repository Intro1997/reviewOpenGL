#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out VS_OUT{
    vec3 Normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main(){
    vec3 normal = mat3(inverse(transpose(view * model))) * aNormal; 
    vs_out.Normal = normalize(vec3(projection * vec4(normal, 0.0)));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

