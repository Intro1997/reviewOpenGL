#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D frontTexture;
uniform sampler2D backTexture;

void main(){
    if(gl_FrontFacing)
        FragColor = texture(frontTexture, TexCoord);
    else
        FragColor = texture(backTexture, TexCoord);
}


