#version 330 core
out vec4 FragColor;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_roughness;

in vec3 Normal;
in vec2 UV;
in vec3 Position;

void main(){
    FragColor = texture(texture_diffuse, UV);
}
