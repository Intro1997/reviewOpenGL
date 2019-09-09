#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 normal;
} fs_in;

struct Light{
    vec3 Position;
    vec3 Color;
};

uniform vec3 viewPos;
uniform Light lights[16];

uniform sampler2D diffuseTexture;

void main(){
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 normal = -normalize(fs_in.normal);
    vec3 lightEffect = vec3(0.0);

    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    float ambientStrength = 0.0;
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
}

//#version 330 core
//out vec4 FragColor;
//
//in VS_OUT {
//    vec3 FragPos;
//    vec3 normal;
//    vec2 TexCoords;
//} fs_in;
//
//
//struct Light {
//    vec3 Position;
//    vec3 Color;
//};
//
//uniform Light lights[16];
//uniform sampler2D diffuseTexture;
//uniform vec3 viewPos;
//
//void main()
//{
//    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
//    vec3 normal = normalize(fs_in.normal);
//    // ambient
//    vec3 ambient = 0.0 * color;
//    // lighting
//    vec3 lighting = vec3(0.0);
//    for(int i = 0; i < 16; i++)
//    {
//        // diffuse
//        vec3 lightDir = normalize(lights[i].Position - fs_in.FragPos);
//        float diff = max(dot(lightDir, normal), 0.0);
//        vec3 diffuse = lights[i].Color * diff * color;
//        vec3 result = diffuse;
//        // attenuation (use quadratic as we have gamma correction)
//        float distance = length(fs_in.FragPos - lights[i].Position);
//        result *= 1.0 / (distance * distance);
//        lighting += result;
//
//    }
//    FragColor = vec4(ambient + lighting, 1.0);
//}
