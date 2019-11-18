#version 330 core
layout(location = 0) in vec4 vector;    // vec2 pos, vec2 texCoord;

out vec2 TexCoord;
uniform mat4 projection;

void main(){
    gl_Position = projection * vec4(vector.xy, 0.0, 1.0);
    TexCoord = vector.zw;
}

