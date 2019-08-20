#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 lightSpacePos;
} fs_in;

uniform sampler2D floorTexture;
uniform sampler2D depthMapTexture;
uniform vec3 cameraPos;
uniform vec3 lightPos;


vec3 diffuseCalculate(vec3 normal, vec3 lightDir){
    float diff = max(dot(normal, lightDir), 0.0);
    return texture(floorTexture, fs_in.TexCoord).rgb * diff;
}

vec3 specularCalculate(vec3 normal, vec3 lightDir){
    vec3 viewDir = normalize(cameraPos - fs_in.FragPos);
    vec3 halfDir = normalize(viewDir + lightDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), 128);
    return texture(floorTexture, fs_in.TexCoord).rgb * spec;
}

float calculateShadow(vec4 lightSpacePos){
    vec3 projCoord = lightSpacePos.xyz / lightSpacePos.w;
    projCoord = (projCoord + 1.0) * 0.5;
    float currentDepthValue;
    // Remove the repeat shadow and obvious edge
    // -----------------------------------------
    if(projCoord.z > 1.0)
        currentDepthValue = 1.0;
    else
        currentDepthValue = projCoord.z;
    
    // Make the PCF
    // ------------
    vec2 texelSize = 1.0 / textureSize(depthMapTexture, 0);
    float shadow = 0.0;
    float closetDepthValue;
    for(int x = -1; x <= 1; x++){
        for(int y = -1; y <= 1; y++){
            closetDepthValue = texture(depthMapTexture, projCoord.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepthValue > closetDepthValue ? 0.0 : 1.0;
        }
    }
    return shadow / 9.0;
}


void main(){
    vec3 normal = normalize(fs_in.Normal);
    
    // ambient light
    float ambientStrength = 0.5;
    vec3 ambient =  texture(floorTexture, fs_in.TexCoord).rgb * ambientStrength;
    
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
    float shadow = calculateShadow(fs_in.lightSpacePos);
    vec4 result = vec4((ambient + shadow * (diffuse + specular)), 1.0) * attenuation;
    
    FragColor = result;
}


