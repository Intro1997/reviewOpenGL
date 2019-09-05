#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 viewPos;
    vec3 lightPos;
} fs_in;

uniform sampler2D brickNormal;
uniform sampler2D brickDiffuse;
uniform sampler2D brickDepth;

uniform float height_scale;

vec2 ParallaxMapping(vec2 texCoord, vec3 viewDir){
    // origin way
    /*
        float height = texture(brickDepth, texCoord).r;
        vec2 p = viewDir.xy / viewDir.z * (height * height_scale);
        return texCoord - p;
     */
    const float minLayerNum = 8;
    const float maxLayerNum = 32;
    float layerNum = mix(maxLayerNum, minLayerNum, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    
    float layerDepthOffset = 1.0 / layerNum;
    float currentLayerDepth = 0.0f;

    vec2 p = viewDir.xy * height_scale;
    vec2 texCoordOffset = p / layerNum;
    
    float currentTextureDepth = texture(brickDepth, texCoord).r;
    
    while(currentLayerDepth < currentTextureDepth){
        texCoord -= texCoordOffset;
        currentTextureDepth = texture(brickDepth, texCoord).r;
        currentLayerDepth += layerDepthOffset;
    }
    vec2 preTexCoord = texCoord + texCoordOffset;
    float afterDepthDiff = currentLayerDepth - currentTextureDepth;
    float beforeDepthDiff = texture(brickDepth, preTexCoord).r - currentLayerDepth + layerDepthOffset;
    float weight = afterDepthDiff / (afterDepthDiff + beforeDepthDiff);
    texCoord = texCoord * (1.0 - weight) + preTexCoord * weight;

    return texCoord;
}

void main(){
    
    vec3 viewDir = normalize(fs_in.viewPos - fs_in.FragPos);
    vec2 texCoord = ParallaxMapping(fs_in.TexCoords, viewDir);
    if(texCoord.x < 0.0 || texCoord.y < 0.0 || texCoord.x > 1.0 || texCoord.y > 1.0)
       discard;

    vec3 normal = texture(brickNormal, texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    // ambient light
    vec3 color = texture(brickDiffuse, texCoord).rgb;
    float ambientStrength = 0.2;
    vec3 ambient = color * ambientStrength;

    // diffuse light
    vec3 lightDir = normalize(fs_in.lightPos - fs_in.FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = color * diff;

    // specular light
//    vec3 viewDir = normalize(fs_in.viewPos - fs_in.FragPos);
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
