#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 lightTangent;
    vec3 viewTangent;
}fs_in;

uniform sampler2D wallTexture;
uniform sampler2D wallNormalTexture;

void main(){
    vec3 normal = texture(wallNormalTexture, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2 - 1.0);
    vec3 color = texture(wallTexture, fs_in.TexCoords).rgb;
    
    // ambient light
    float ambientStrength = 0.5;
    vec3 ambient =  color * ambientStrength;
    
    // diffuse light
    vec3 lightDir = normalize(fs_in.lightTangent - fs_in.FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = color * diff;
    
    // specular light
    
    vec3 viewDir = normalize(fs_in.viewTangent - fs_in.FragPos);
    vec3 halfDir = normalize(viewDir + lightDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), 128);
    vec3 specular = color * spec;
    
    
    // attenuation
    float contanst = 1.0;
    float Distance;
    float linearPar = 0.035;
    float quadraticPar = 0.0175;
    float attenuation;
    Distance = length(fs_in.lightTangent - fs_in.FragPos);
    attenuation = 1.0 / (1.0 + linearPar * Distance + quadraticPar * Distance * Distance);
    vec4 result = vec4((ambient + diffuse + specular), 1.0) * attenuation;
    FragColor = result;
}


