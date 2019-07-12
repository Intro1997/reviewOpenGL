#version 330 core
struct Material{
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
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
in vec2 TexCoord;

uniform vec3 cubeColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main(){
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
    
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(lightDir, Normal), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
