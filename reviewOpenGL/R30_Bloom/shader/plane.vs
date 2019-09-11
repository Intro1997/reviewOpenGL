#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
} vs_out;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.TexCoords = aTexCoord;
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
}
