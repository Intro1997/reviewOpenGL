#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;

out VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 lightTangent;
    vec3 viewTangent;
}vs_out;

void main(){
    vs_out.TexCoords = vec2(aTexCoord.x, aTexCoord.y);
    
    vec3 T = vec3(model * vec4(aTangent, 1.0));
    vec3 B = vec3(model * vec4(aBitangent, 1.0));
    vec3 N = cross(T, B);
    
    mat3 TBN = transpose(mat3(T, B, N));
    
    vs_out.FragPos = TBN * vec3(model * vec4(aPos, 1.0));
    vs_out.lightTangent = TBN * lightPos;
    vs_out.viewTangent = TBN * viewPos;
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
