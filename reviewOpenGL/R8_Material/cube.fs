#version 330 core
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;

struct Light{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 cubeColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main(){
    // ambient
    vec3 ambient = light.ambient * material.ambient;
    
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = light.diffuse * material.diffuse * diff;
    
    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * light.specular * material.specular;
    
    // result
    vec3 result = ambient + diffuse + specular;
    
    FragColor = vec4(result, 1.0);
}
