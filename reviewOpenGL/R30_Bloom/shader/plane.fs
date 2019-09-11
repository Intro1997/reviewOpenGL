#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
} fs_in;

struct Light{
    vec3 Position;
    vec3 Color;
};

uniform vec3 Normal;

uniform vec3 viewPos;
uniform Light lights[16];

uniform sampler2D diffuseTexture;

void main(){
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 normal = normalize(Normal);
    vec3 lightEffect = vec3(0.0);
    
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    float ambientStrength = 0.1;
    vec3 ambient = color * ambientStrength;
    
    for(int i = 0; i < 16; i++){
        vec3 lightDir = normalize(lights[i].Position - fs_in.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = color * diff * lights[i].Color;
        vec3 result = diffuse;
        float Distance = length(lights[i].Position - fs_in.FragPos);
        result *= 1.0 / (Distance * Distance);
        lightEffect += result;
    }
    FragColor = vec4(ambient + lightEffect, 1.0);
    float brightColor = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightColor > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
