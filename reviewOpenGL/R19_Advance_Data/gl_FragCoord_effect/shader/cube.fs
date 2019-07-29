#version 330 core
out vec4 FragColor;

void main(){
    if(gl_FragCoord.x >= 800) // My screen is reinta screen, if your screen isn't reinta screen it's better to set it half of Screen width
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    else
        FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}


