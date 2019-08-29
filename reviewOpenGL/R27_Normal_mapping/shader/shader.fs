#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 viewPos;
    vec3 lightPos;
} fs_in;

uniform sampler2D wallTexture;
uniform sampler2D wallNormalTexture;

void main(){

    vec3 normal = texture(wallNormalTexture, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    // ambient light
    vec3 color = texture(wallTexture, fs_in.TexCoords).rgb;
    float ambientStrength = 0.2;
    vec3 ambient = color * ambientStrength;

    // diffuse light
    vec3 lightDir = normalize(fs_in.lightPos - fs_in.FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = color * diff;

    // specular light
    vec3 viewDir = normalize(fs_in.viewPos - fs_in.FragPos);
    vec3 halfDir = normalize(viewDir + lightDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), 128.0);
    vec3 specular = color * spec;

    // attenuation
    float contanst = 1.0;
    float Distance;
    float linearPar = 0.035;
    float quadraticPar = 0.0175;
    float attenuation;
    Distance = length(fs_in.lightPos - fs_in.FragPos);
    attenuation = 1.0 / (1.0 + linearPar * Distance + quadraticPar * Distance * Distance);
    vec4 result = vec4((ambient + (diffuse + specular)), 1.0) * attenuation;
    FragColor = result;
}

//#version 330 core
//out vec4 FragColor;
//
//in VS_OUT {
//    vec3 FragPos;
//    vec2 TexCoords;
//    vec3 TangentLightPos;
//    vec3 TangentViewPos;
//    vec3 TangentFragPos;
//} fs_in;
//
//uniform sampler2D diffuseMap;
//uniform sampler2D normalMap;
//
//uniform vec3 lightPos;
//uniform vec3 viewPos;
//
//void main()
//{
//    // obtain normal from normal map in range [0,1]
//    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
//    // transform normal vector to range [-1,1]
//    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
//
//    // get diffuse color
//    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
//    // ambient
//    vec3 ambient = 0.1 * color;
//    // diffuse
//    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
//    float diff = max(dot(lightDir, normal), 0.0);
//    vec3 diffuse = diff * color;
//    // specular
//    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
//    vec3 reflectDir = reflect(-lightDir, normal);
//    vec3 halfwayDir = normalize(lightDir + viewDir);
//    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
//
//    vec3 specular = vec3(0.2) * spec;
//    FragColor = vec4(ambient + diffuse + specular, 1.0);
//}
