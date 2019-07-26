#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoord;

uniform samplerCube skybox;
uniform vec3 cameraPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_reflection1;

void main(){
    // Diffuse
    vec4 diffuse_color = texture(texture_diffuse1, TexCoord);
    // Reflection
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    float reflect_intensity = texture(texture_reflection1, TexCoord).r;
    vec4 reflect_color;
    if(reflect_intensity > 0.1) // Only sample reflections when above a certain treshold
        reflect_color = texture(skybox, R) * reflect_intensity;
    // Combine them
    FragColor = diffuse_color + reflect_color;
}
