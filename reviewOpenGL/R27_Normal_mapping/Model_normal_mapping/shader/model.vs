#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoord;
layout(location=3) in vec3 aTangent;
layout(location=4) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;

out VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
    vec3 tangentLightPos;
    vec3 tangentViewPos;
}vs_out;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    vs_out.TexCoords = aTexCoord;
    
    // Create TBN matrix
    mat3 normalMatrix = mat3(inverse(transpose(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    vec3 B = normalize(normalMatrix * aBitangent);
    if(dot(cross(N, T), B) < 0.0)
        T = T * -1.0;
    vs_out.TBN = transpose(mat3(T, B, N));
    
    // other parameters
    vs_out.FragPos = vs_out.TBN * vec3(model * vec4(aPos, 1.0));
    vs_out.tangentLightPos = vs_out.TBN * lightPos;
    vs_out.tangentViewPos = vs_out.TBN * viewPos;
}


