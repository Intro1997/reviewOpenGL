//#version 330 core
//out vec4 FragColor;
//
//in VS_OUT{
//    vec2 TexCoords;
//    vec3 Normal;
//    vec3 FragPos;
//} fs_in;
//
//uniform vec3 viewPos;
//uniform vec3 lightPos;
//
//uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;
//
//vec3 diffuseCalculate(vec3 normal, vec3 lightDir){
//    float diff = max(dot(normal, lightDir), 0.0);
//    return texture(texture_diffuse1, fs_in.TexCoords).rgb * diff;
//}
//
//vec3 specularCalculate(vec3 normal, vec3 lightDir){
//    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
//    vec3 halfDir = normalize(viewDir + lightDir);
//    float spec = pow(max(dot(halfDir, normal), 0.0), 64);
//    return texture(texture_specular1, fs_in.TexCoords).rgb * spec;
//}
//
//void main(){
//    vec3 normal = normalize(fs_in.Normal);
//
//    // ambient light
//    float ambientStrength = 0.5;
//    vec3 ambient =  texture(texture_diffuse1, fs_in.TexCoords).rgb * ambientStrength;
//
//    // diffuse light
//    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
//    vec3 diffuse = diffuseCalculate(normal, lightDir);
//
//    // specular light
//    vec3 specular;
//    specular = specularCalculate(normal, lightDir);
//
//    // attenuation
//    float contanst = 1.0;
//    float Distance;
//    float linearPar = 0.035;
//    float quadraticPar = 0.0175;
//    float attenuation;
//    Distance = length(lightPos - fs_in.FragPos);
//    attenuation = 1.0 / (1.0 + linearPar * Distance + quadraticPar * Distance * Distance);
//    vec4 result = vec4((ambient + diffuse + specular), 1.0);
//    FragColor = result;
//}

#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
    vec3 tangentLightPos;
    vec3 tangentViewPos;
}fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

void main(){
    // Calculate by the model's Bitangent
    float ambientStrength = 0.5;
    vec3 normal = normalize(texture(texture_normal1, fs_in.TexCoords).rgb);
    normal = normal * 2.0 - 1.0;

    normal = normalize(normal);
    
    // create color
    vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;
    // ambient light

    vec3 ambient = color * ambientStrength;

    // diffuse light
    vec3 lightDir = normalize(fs_in.tangentLightPos - fs_in.FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = texture(texture_diffuse1, fs_in.TexCoords).rgb * diff;

    // specular light
    vec3 viewDir = normalize(fs_in.tangentViewPos - fs_in.FragPos);
    vec3 halfDir = normalize(viewDir + lightDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), 64);
    vec3 specular = texture(texture_specular1, fs_in.TexCoords).rgb * spec;

    // attenuation
    float contanst = 1.0;
    float Distance;
    float linearPar = 0.035;
    float quadraticPar = 0.0175;
    float attenuation;
    Distance = length(fs_in.tangentLightPos - fs_in.FragPos);
    attenuation = 1.0 / (1.0 + linearPar * Distance + quadraticPar * Distance * Distance);
    vec4 result = vec4((ambient + diffuse + specular), 1.0);
    FragColor = result;
//    FragColor = vec4(normal, 1.0)
}


