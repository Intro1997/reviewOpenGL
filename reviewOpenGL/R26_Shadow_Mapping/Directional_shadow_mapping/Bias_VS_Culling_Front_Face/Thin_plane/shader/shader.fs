#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragPos;
    vec4 FragPosSpaceLight;
} fs_in;

uniform vec3 viewPos;
uniform vec3 lightPos;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

vec3 diffuseCalculate(vec3 normal, vec3 lightDir){
    float diff = max(dot(normal, lightDir), 0.0);
    return texture(diffuseTexture, fs_in.TexCoord).rgb * diff;
}

vec3 specularCalculate(vec3 normal, vec3 lightDir){
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfDir = normalize(viewDir + lightDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), 128);
    return texture(diffuseTexture, fs_in.TexCoord).rgb * spec;
}

float calculateShadowValue(vec4 FragPosSpaceLight){
    vec3 projCoords = FragPosSpaceLight.xyz / FragPosSpaceLight.w;
    projCoords = (projCoords + 1.0) * 0.5;
    float closetDepthValue = texture(shadowMap, projCoords.xy).r;
    float bias = 0.05;
    float currentDepthValue = projCoords.z;
    return currentDepthValue - bias > closetDepthValue ? 1.0 : 0.0;
}

void main(){
    vec3 normal = normalize(fs_in.Normal);

    // ambient light
    float ambientStrength = 0.5;
    vec3 ambient =  texture(diffuseTexture, fs_in.TexCoord).rgb * ambientStrength;

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
    float shadow = calculateShadowValue(fs_in.FragPosSpaceLight);
    vec4 result = vec4((ambient + (1.0 - shadow) * (diffuse + specular)), 1.0) * attenuation;

    FragColor = result;
}

