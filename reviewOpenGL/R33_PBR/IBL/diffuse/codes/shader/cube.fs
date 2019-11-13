#version 330 core
out vec4 FragColor;

in vec3 localPos;

uniform sampler2D equirectangularMap;
const float Pi = 3.14159265359;

vec2 getUV(vec3 position){
    const vec2 stride = vec2(1.0 / (2.0 * Pi), 1.0 / Pi);
    vec2 uv = vec2(atan(position.z, position.x), asin(position.y));
    uv *= stride;
    uv += 0.5;
    return uv;
}

void main(){
    vec2 uv = getUV(normalize(localPos));
    vec3 color = texture(equirectangularMap, uv).rgb;
    FragColor = vec4(color, 1.0);
}
