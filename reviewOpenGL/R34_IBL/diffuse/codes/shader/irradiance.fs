#version 330 core
out vec4 FragColor;
in vec3 localPos;

uniform samplerCube skyboxTexture;

const float Pi = 3.14159265359;

void main(){
    vec3 N = normalize(localPos);
    vec3 irrandiance = vec3(0.0);

    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, N);
    up = cross(N, right);

    float stride = 0.025;
    float countTime = 0.0;
    for(float phi = 0.0; phi < 2.0 * Pi; phi += stride){
        for(float theta = 0.0; theta < 0.5 * Pi; theta += stride){
            vec3 sphereCoordinate = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            vec3 sampleCoordinate = sphereCoordinate.x * right + sphereCoordinate.y * up + sphereCoordinate.z * N;
            irrandiance += texture(skyboxTexture, sampleCoordinate).rgb * cos(theta) * sin(theta);
            countTime++;
        }
    }
    irrandiance = irrandiance * Pi * (1.0 / float(countTime));
    FragColor = vec4(irrandiance, 1.0);
}
