#version 330 core
out vec4 FragColor;

uniform sampler2D screen_texture;

in vec2 TexCoord;

void main(){
    FragColor = texture(screen_texture, TexCoord);
}
