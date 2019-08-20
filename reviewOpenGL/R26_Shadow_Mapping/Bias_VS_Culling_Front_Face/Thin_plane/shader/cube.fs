//#version 330 core
//out vec4 FragColor;
//
//in vec2 TexCoord;
//
//uniform sampler2D cubeTexture;
//
//void main(){
//    FragColor = texture(cubeTexture, TexCoord);
//}


#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform sampler2D cubeTexture;

vec3 diffuseCalculate(vec3 normal, vec3 lightDir){
    float diff = max(dot(normal, lightDir), 0.0);
    return texture(cubeTexture, TexCoord).rgb * diff;
}

vec3 specularCalculate(vec3 normal, vec3 lightDir){
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 halfDir = normalize(viewDir + lightDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), 128);
    return texture(cubeTexture, TexCoord).rgb * spec;
}

void main(){
    vec3 normal = normalize(Normal);
    
    // ambient light
    float ambientStrength = 0.5;
    vec3 ambient =  texture(cubeTexture, TexCoord).rgb * ambientStrength;
    
    // diffuse light
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 diffuse = diffuseCalculate(normal, lightDir);
    
    // specular light
    vec3 specular;
    specular = specularCalculate(normal, lightDir);
    
    // attenuation
    float contanst = 1.0;
    float Distance;
    float linearPar = 0.035;
    float quadraticPar = 0.0175;
    float attenuation;
    Distance = length(lightPos - FragPos);
    attenuation = 1.0 / (1.0 + linearPar * Distance + quadraticPar * Distance * Distance);
    
    vec4 result;
    result = vec4(ambient + diffuse + specular, 1.0) * attenuation;
    
    
    FragColor = result;
}
