#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 modelMatrices;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
    vec3 Normal;
    vec2 TexCoord;
} vs_out;

void main(){
    gl_Position = projection * view * modelMatrices * vec4(aPos, 1.0);
    vs_out.Normal = vec3(inverse(transpose(model)) * vec4(aNormal, 1.0));
    vs_out.TexCoord = aTexCoord;
}
