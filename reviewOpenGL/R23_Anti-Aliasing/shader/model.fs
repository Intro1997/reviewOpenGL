#version 330 core
out vec4 FragColor;

in VS_OUT{
    in vec3 Normal;
    in vec2 TexCoord;
} fs_in;

uniform sampler2D texture_diffues1;

void main(){
    FragColor = texture(texture_diffues1, fs_in.TexCoord);
}
