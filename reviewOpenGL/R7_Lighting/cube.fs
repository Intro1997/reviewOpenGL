#version 330 core
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 cubeColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
    // calculate the ambient
    float ambientStrenth = 0.1;
    vec3 ambient = ambientStrenth * lightColor;
    
    // calculate the diffuse
    float diffuseStrength = 0.5;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * diffuseStrength;
    
    // calculate the specular
    float specularStrength = 0.8;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = lightColor * specularStrength * spec;
    
    // the result
    vec3 result = (ambient + diffuse + specular) * cubeColor ;
    
    FragColor = vec4(result, 1.0);
}
