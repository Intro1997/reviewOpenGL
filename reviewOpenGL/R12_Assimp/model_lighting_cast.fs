#version 330 core
/**
 * global struct variables
 */

struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};
uniform Material material;

struct DirLight{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct PointLight{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};
uniform PointLight pointLight;

struct SpotLight{
    vec3 position;
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
    
    float innerborder;
    float outerborder;
};
uniform SpotLight spotLight;

/**
 * global normal variables
 */
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;
uniform vec3 viewPos;

/**
 * Functions
 */
vec3 dirLightCalculate(vec3 normal, DirLight light){
    vec3 ambient = texture(material.texture_diffuse1, TexCoord).rgb * light.ambient;
    
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(lightDir, normal), 0.0f);
    vec3 diffuse = diff * texture(material.texture_diffuse1, TexCoord).rgb * light.diffuse;
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
    vec3 specular = spec * texture(material.texture_specular1, TexCoord).rgb * light.specular;
    
    return (ambient + diffuse + specular);
}
vec3 pointLightCalculate(vec3 normal, PointLight light){
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoord).rgb;
    
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * texture(material.texture_diffuse1, TexCoord).rgb * diff;
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
    vec3 specular = light.specular * texture(material.texture_specular1, TexCoord).rgb * spec;
    
    float Distance = length(light.position - FragPos);
    float attenuation = 1.0f/(light.constant + light.linear * Distance + light.quadratic * Distance * Distance);
    
    return (ambient + (diffuse + specular) * attenuation);
}
vec3 spotLightCalculate(vec3 normal, SpotLight light){
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoord).rgb;
    
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * texture(material.texture_diffuse1, TexCoord).rgb * diff;
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
    vec3 specular = light.specular * texture(material.texture_specular1, TexCoord).rgb * spec;
    
    float Distance = length(light.position - FragPos);
    float attenuation = 1.0f/(light.constant + light.linear * Distance + light.quadratic * Distance * Distance);
    
    float theta = max(dot(normalize(light.direction), normalize(FragPos - light.position)), 0.0);
    float flashStrength = 1.0;
    if(theta < light.innerborder){
        flashStrength = clamp((theta - light.outerborder) / (light.innerborder - light.outerborder), 0.0, 1.0);
    }
    
    return (ambient + (diffuse + specular) * flashStrength) * attenuation ;
}

void main(){
    vec3 normal = normalize(Normal);
    vec3 result;
    
    result = dirLightCalculate(normal, dirLight);
    result += pointLightCalculate(normal, pointLight);
    result += spotLightCalculate(normal, spotLight)*2;
    FragColor = vec4(result, 1.0);
}

