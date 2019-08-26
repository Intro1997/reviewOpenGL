#version 330 core
out vec4 FragColor;

uniform sampler2D depthMapTexture;

in vec2 TexCoord;

void main(){
    float depthValue = texture(depthMapTexture, TexCoord).r;
    FragColor = vec4(vec3(depthValue), 1.0);
}

