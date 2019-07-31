#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

const float MAGNITUDE = 0.2;

in VS_OUT{
    vec3 Normal;
} gs_in[];

void GenearteLine(int index){
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].Normal, 0.0) * MAGNITUDE;
    EmitVertex();
    EndPrimitive();
}

void main(){
    GenearteLine(0);
    GenearteLine(1);
    GenearteLine(2);
}

