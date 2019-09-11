#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D scene;
uniform sampler2D blurScene;

uniform float exposure;

void main(){
    const float gamma = 2.2f;
    
    vec3 sceneTex = texture(scene, TexCoord).rgb;
    vec3 blurTex = texture(blurScene, TexCoord).rgb;
    vec3 result = sceneTex + blurTex;
    
    result = vec3(1.0) - exp(-result * exposure);
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}
