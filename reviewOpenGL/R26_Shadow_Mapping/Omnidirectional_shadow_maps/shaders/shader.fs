#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec2 TexCoords;
    vec3 Normal;
    vec3 FragPos;
//    vec4 FragPosSpaceLight;
} fs_in;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform float far_plane;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

vec3 diffuseCalculate(vec3 normal, vec3 lightDir){
    float diff = max(dot(normal, lightDir), 0.0);
    return texture(diffuseTexture, fs_in.TexCoords).rgb * diff;
}

vec3 specularCalculate(vec3 normal, vec3 lightDir){
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfDir = normalize(viewDir + lightDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), 128);
    return texture(diffuseTexture, fs_in.TexCoords).rgb * spec;
}

float calculateShadowValue(vec3 FragPos){
    vec3 fragToLight = FragPos - lightPos;
    float closetDepth = texture(depthMap, fragToLight).r;
    closetDepth = closetDepth * far_plane;
    float currentDepth = length(fragToLight);
    float bias = 0.005;
    float shadow = currentDepth - bias > closetDepth ? 0.0 : 1.0;
    return shadow;
}

void main(){
    vec3 normal = normalize(fs_in.Normal);

    // ambient light
    float ambientStrength = 0.5;
    vec3 ambient =  texture(diffuseTexture, fs_in.TexCoords).rgb * ambientStrength;

    // diffuse light
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
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
    Distance = length(lightPos - fs_in.FragPos);
    attenuation = 1.0 / (1.0 + linearPar * Distance + quadraticPar * Distance * Distance);
    float shadow = calculateShadowValue(fs_in.FragPos);
    vec4 result = vec4((ambient + shadow * (diffuse + specular)), 1.0) * attenuation;
    FragColor = result;
}


