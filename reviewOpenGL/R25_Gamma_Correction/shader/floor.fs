#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 cameraPos;
uniform vec3 lightPos[4];
uniform sampler2D floorTexture;

vec3 diffuseCalculate(vec3 normal, vec3 lightDir){
    float diff = max(dot(normal, lightDir), 0.0);
    return texture(floorTexture, TexCoord).rgb * diff;
}

vec3 specularCalculate(vec3 normal, vec3 lightDir){
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 halfDir = normalize(viewDir + lightDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), 128);
    return texture(floorTexture, TexCoord).rgb * spec;
}

void main(){
    vec3 normal = normalize(Normal);
    
    // ambient light
    float ambientStrength = 0.1;
    vec3 ambient =  texture(floorTexture, TexCoord).rgb * ambientStrength;
    
    // diffuse light
    vec3 diffuse[4];
    vec3 lightDir[4];
    for(int i = 0; i < 4; i++){
        lightDir[i] = normalize(lightPos[i] - FragPos);
        diffuse[i] = pow(diffuseCalculate(normal, lightDir[i]), vec3(1.0/2.2));
    }
    
    // specular light
    vec3 specular[4];
    for(int i = 0; i < 4; i++){
        specular[i] = specularCalculate(normal, lightDir[i]);
    }
    
//    Distance    Constant    Linear    Quadratic
//    7            1.0           0.7      1.8
//    13           1.0           0.35     0.44
//    20           1.0           0.22     0.20
//    32           1.0           0.14     0.07
//    50           1.0           0.09     0.032
//    65           1.0           0.07     0.017
//    100          1.0           0.045    0.0075
//    160          1.0           0.027    0.0028
//    200          1.0           0.022    0.0019
//    325          1.0           0.014    0.0007
//    600          1.0           0.007    0.0002
//    3250         1.0           0.0014   0.000007

    
    // attenuation
    float contanst = 1.0;
    float Distance[4];
    float linearPar = 0.14;
    float quadraticPar = 0.07;
    float attenuation[4];
    for(int i = 0; i < 4; i++){
        Distance[i] = length(lightPos[i] - FragPos);
        attenuation[i] = 1.0 / (1.0 + linearPar * Distance[i] + quadraticPar * Distance[i] * Distance[i]);
    }
    
    vec4 result;
    for(int i = 0; i < 4; i++){
        result += vec4(diffuse[i] + specular[i], 1.0) * attenuation[i];
    }
    
    FragColor = result;
}
