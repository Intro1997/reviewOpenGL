#version 330 core
out vec4 FragColor;

in vec3 localPos;

uniform float roughness;
uniform samplerCube environmentMap;

const float Pi = 3.14159265359;

float RadicalInverse_VdC(uint bits){
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 Hammersley(uint i, uint N){
    return vec2(float(i) / float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness){
    float a = roughness * roughness;
    
    float Phi = 2 * Pi * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
    
    vec3 H;
    H.x = sinTheta * cos(Phi);
    H.y = sinTheta * sin(Phi);
    H.z = cosTheta;
    
    vec3 up = N.z < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, N));
    vec3 bitangent = normalize(cross(N, tangent));
    
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

float DistributionGGX(vec3 N, vec3 H, float roughness){
    float a      = roughness * roughness;    // According to Disney and Epic game, squaring looks more correct
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float denom = NdotH2 * (a2 - 1.0) + 1.0;
    denom = Pi * denom * denom;

//    return a2 / max(denom, 0.00000001);
    return a2 / denom;
}

vec3 prefilterEnvironmentMap(vec3 N, vec3 V, float roughness){
    const uint SAMPLE_COUNT = 512u;
    vec3 prefilterColor = vec3(0.0);
    float totalWeight = 0.0;
    for(uint i = 0u; i < SAMPLE_COUNT; ++i){
        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
        vec3 H = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L = normalize(2.0 * dot(V, H) * H - V);

        float NdotH = max(dot(N, H), 0.0);
        float NdotL = max(dot(N, L), 0.0);
        float HdotV = max(dot(H, V), 0.0);
        
        if(NdotL > 0.0){
            float D   = DistributionGGX(N, H, roughness);
            float pdf = (D * NdotH / (4.0 * HdotV)) + 0.0001;

            float resolution = 512.0; // resolution of source cubemap (per face)
            float saTexel  = 4.0 * Pi / (6.0 * resolution * resolution);
            float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

            float mipLevel = roughness == 0.0 ? 0.0 : 0.6 * log2(saSample / saTexel);
            
//            prefilterColor += texture(environmentMap, L).rgb * NdotL;
            prefilterColor += textureLod(environmentMap, L, mipLevel).rgb;
            totalWeight += NdotL;
        }
    }
    return prefilterColor / max(totalWeight, 0.0);
}

void main(){
    vec3 N = normalize(localPos);
    vec3 R = N;
    vec3 V = R;
        
    vec3 color = prefilterEnvironmentMap(N, V, roughness);
    
    FragColor = vec4(color, 1.0);
}

