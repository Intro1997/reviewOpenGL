#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D grassTexture;

void main(){
    vec4 result = texture(grassTexture, TexCoord);
    if(result.a < 0.1)
        discard;
    FragColor = result;
}

