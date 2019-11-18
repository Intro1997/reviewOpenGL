#version 330 core
out vec4 color;

in vec2 TexCoord;

uniform sampler2D characterTexture;
uniform vec3 textColor;

void main(){
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(characterTexture, TexCoord).r);
    color = vec4(textColor, 1.0) * sampled;
}


