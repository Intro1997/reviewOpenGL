#version 330 core
out vec4 FragColor;
in vec3 WorldPos;   // model surface position
in vec2 UV;
in vec3 Normal;

uniform vec3 camPos;
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform float ao;

//uniform vec3 albedo;
//uniform float metallic;
//uniform float roughness;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;
uniform sampler2D albedoTexture;
uniform sampler2D metallicTexture;
uniform sampler2D normalTexture;
uniform sampler2D roughnessTexture;

const float PI = 3.14159265359;

vec3 getNormal(){
    vec3 normal = texture(normalTexture, UV).xyz * 2.0 - 1.0;
    vec2 uv1 = dFdx(UV);
    vec2 uv2 = dFdy(UV);
    vec3 pos1 = dFdx(WorldPos);
    vec3 pos2 = dFdy(WorldPos);

    vec3 N = normalize(Normal);
    vec3 T = normalize(pos1 * uv2.t - pos2 * uv1.t);
    vec3 B = -normalize(cross(N, T));

    mat3 TBN = mat3(T, B, N);
    return normalize(TBN * normal);
}

// F function in f_cook-torrance
// F0: base reflectivity or surface reflection at zero incidence
// cosTheta: halfway vector dot view vector
vec3 fresnelSchlick(vec3 F0, float cosTheta){
    return F0 + (1.0 - F0) * pow(1 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

// D function in f_cook-torrance
// N: normal vector
// H: halfway vector
float DistributionGGX(vec3 N, vec3 H, float roughness){
    float a      = roughness * roughness;    // According to Disney and Epic game, squaring looks more correct
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float denom = NdotH2 * (a2 - 1.0) + 1.0;
    denom = PI * denom * denom;

    return a2 / max(denom, 0.00000001);
//    return a2 / denom;
}

// G_sub in G function
float GeometrySchlickGGX(float NdotV, float roughness){
    // use direct light
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float denom = NdotV * (1.0 - k) + k;

    return NdotV / denom;
}
// G function in f_cook-torrance
// N: normal vector
// L: light vector from surface position to light position
// V: view vector from surface position to view position
float GeometrySmith(vec3 N, vec3 L, vec3 V, float roughness){
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

void main(){
//    vec3 normal = normalize(Normal);
    vec3 normal = getNormal();
    vec3 albedo = texture(albedoTexture, UV).rgb;
    float metallic = texture(metallicTexture, UV).r;
    float roughness = texture(roughnessTexture, UV).r;
        

    vec3 N = normal;     // normal position
    vec3 V = normalize(camPos - WorldPos);      // view position
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04);
    F0      = mix(F0, albedo, metallic); //mix(x, y, a)   x⋅(1−a)+y⋅a

    vec3 Lo = vec3(0.0);    // output radiance

    for(int i = 0; i < 4; i++){    // we calculat 4 point light in the environment
        vec3 L = normalize(lightPositions[i] - WorldPos);   // light direction position
        vec3 H = normalize(V + L);                          // halfway vector

        // calculate Li
        float distance = length(WorldPos - lightPositions[i]);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = attenuation * lightColors[i];

        // calculate specular
        vec3 F0 = vec3(0.04);
        F0      = mix(F0, albedo, metallic); //mix(x, y, a)   x⋅(1−a)+y⋅a

        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, L, V, roughness);
        vec3 F    = fresnelSchlick(F0, clamp(dot(H, V), 0.0, 1.0));

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, L), 0.0) * max(dot(N, V), 0.0);
        vec3 specular = numerator / max(denominator, 0.001);

        // calculate diffuse
        vec3 KS = F;
        vec3 KD = vec3(1.0) - KS;
        KD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (KD * albedo / PI + specular) * radiance * NdotL;
    }


    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 Ks = F;
    vec3 Kd = 1.0 - Ks;
    Kd *= 1.0 - metallic;

    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilterColor = textureLod(prefilterMap, R, MAX_REFLECTION_LOD * roughness).rgb;
    vec2 envBRDF = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilterColor * (F * envBRDF.x + envBRDF.y);


    vec3 ambient = (diffuse * Kd + specular) * ao;
    vec3 color = ambient + Lo;


    // sRGB gamma correct
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color , 1.0);
}
