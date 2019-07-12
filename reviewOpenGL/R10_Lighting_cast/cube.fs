#version 330 core
struct Material{
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

uniform Material material;

struct directionLight{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct spotLight{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

struct flashLight{
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

uniform flashLight flashlight;

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 cubeColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main(){

    vec3 result;
    float theta = dot(normalize(FragPos - flashlight.position), normalize(flashlight.direction));
    if(theta > flashlight.cutoff){
        float strength = 1.0;
        if(theta < flashlight.boardAngle)
            strength = clamp((theta - flashlight.cutoff)/(flashlight.boardAngle - flashlight.cutoff), 0.0, 1.0);
        // ambient
        vec3 ambient = flashlight.ambient * texture(material.diffuse, TexCoord).rgb;
        
        // diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(flashlight.position - FragPos);
        float diff = max(dot(lightDir, Normal), 0.0);
        vec3 diffuse = flashlight.diffuse * diff * texture(material.diffuse, TexCoord).rgb;
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = flashlight.specular * spec * texture(material.specular, TexCoord).rgb;
        
        // attenuation
        float direction = length(flashlight.position - FragPos);
        float attenuation = 1.0 / (flashlight.constant + flashlight.linear * direction + flashlight.quadratic * direction * direction);
        
        
        
        result = (ambient + (diffuse + specular) * strength) * attenuation ;
    }
    else{
        result = flashlight.ambient * texture(material.diffuse, TexCoord).rbg;
    }

    
    FragColor = vec4(result, 1.0);
}
