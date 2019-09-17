#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 aViewPos;
uniform vec3 aLightPos;

out VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 viewPos;
    vec3 lightPos;
}vs_out;

void main(){

    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.TexCoords = aTexCoord;

    mat3 normalMatrix = inverse(transpose(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 B = normalize(normalMatrix * aBitangent);
    vec3 N = normalize(normalMatrix * aNormal);

    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.viewPos  = TBN * aViewPos;
    vs_out.lightPos = TBN * aLightPos;
    vs_out.FragPos  = TBN * vec3(model * vec4(aPos, 1.0));
}