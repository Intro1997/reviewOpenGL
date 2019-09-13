#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform vec3 viewPos;

struct Light{
    vec3 Position;
    vec3 Color;
};

uniform Light lights[32];

void main(){
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lighting = Albedo * 0.1; // ambient
    for(int i = 0; i < 32; i++){
        vec3 lightDir = normalize(lights[i].Position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * lights[i].Color;
        
        vec3 halfDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(Normal, halfDir), 0.0), 16.0);
        vec3 specular = Specular * lights[i].Color * spec;
        
        float Distance = length(lights[i].Position - FragPos);
        float attenuation = 1.0 / (1.0 + 0.7 * Distance + 1.0 * Distance * Distance);

        diffuse *= attenuation;
        specular *= attenuation;
        
        lighting +=  specular + diffuse;
    }
    FragColor = vec4(lighting, 1.0);
}

