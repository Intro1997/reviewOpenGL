#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 cubeColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

struct Material{
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

uniform Material material;

struct DirLight{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;
vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;
    
    return (ambient + diffuse + specular);
}

struct PointLight{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLight[NR_POINT_LIGHTS];

vec3 CalcSpotLight(PointLight light, vec3 norm, vec3 viewDir){
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    float Distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * Distance + light.quadratic * Distance * Distance);
    
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb * attenuation;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb * attenuation;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb * attenuation;
    
    return (ambient + diffuse + specular);
}

struct FlashLight{
    vec3 position;
    vec3 direction;
    float cutoff;
    float boardAngle;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform FlashLight flashLight;
vec3 CalcFlashLight(FlashLight light, vec3 norm, vec3 viewDir){
    float theta = max(dot(normalize(light.direction), normalize(FragPos - light.position)), 0.0);
    float strength = 1.0;
    if(theta < light.boardAngle){
        strength = clamp((theta - light.cutoff) / (light.boardAngle - light.cutoff), 0.0, 1.0);
    }
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(lightDir, norm), 0.0);

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float Distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * Distance + light.quadratic * Distance * Distance);

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb * attenuation * strength;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb * attenuation * strength;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb * attenuation * strength;

    return (ambient + diffuse + specular);
}



void main(){
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcSpotLight(pointLight[i], norm, viewDir);
    
    result += CalcFlashLight(flashLight, norm, viewDir);
    
    FragColor = vec4(result, 1.0);
}
