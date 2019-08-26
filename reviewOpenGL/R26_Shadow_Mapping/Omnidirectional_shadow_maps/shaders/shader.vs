#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform mat4 lightSpaceMatrix;

out VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
}vs_out;

uniform bool reverse_normals;

void main(){
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    if(reverse_normals)
        vs_out.Normal = mat3(inverse(transpose(model))) * -1.0 * aNormal;
    else
        vs_out.Normal = mat3(inverse(transpose(model))) * aNormal;
    vs_out.TexCoords = vec2(aTexCoord.x, aTexCoord.y);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
