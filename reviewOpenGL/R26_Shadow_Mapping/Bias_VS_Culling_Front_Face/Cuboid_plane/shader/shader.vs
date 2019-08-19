#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out VS_OUT{
    vec3 FragPos;
    vec2 TexCoord;
    vec3 Normal;
    vec4 FragPosSpaceLight;
}vs_out;

void main(){
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = mat3(inverse(transpose(model))) * aNormal;
    vs_out.TexCoord = aTexCoord;
    vs_out.FragPosSpaceLight = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

