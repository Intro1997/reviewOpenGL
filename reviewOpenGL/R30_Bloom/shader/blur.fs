#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D Texture;
uniform bool horizontal;

void main(){
    float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
    vec2 offset = 1.0 / textureSize(Texture, 0);
    vec3 result = texture(Texture, TexCoord).rgb * weight[0];
    if(horizontal){
        for(int i = 1; i < 5; i++){
            result += texture(Texture, TexCoord + vec2(i * offset.x, 0.0)).rgb * weight[i];
            result += texture(Texture, TexCoord - vec2(i * offset.x, 0.0)).rgb * weight[i];
        }
    }
    else{
        for(int i = 1; i < 5; i++){
            result += texture(Texture, TexCoord + vec2(0.0, i * offset.y)).rgb * weight[i];
            result += texture(Texture, TexCoord - vec2(0.0, i * offset.y)).rgb * weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}

