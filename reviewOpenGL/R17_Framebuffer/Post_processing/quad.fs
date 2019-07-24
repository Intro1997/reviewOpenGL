#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;
void main(){
//    Inversion effect
//    ----------------
//    FragColor = vec4(1.0 - texture(screenTexture, TexCoord).rgb, 1.0);
    
//    Grayscale
//    ---------
//    FragColor = texture(screenTexture, TexCoord);
//    float averge = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
//    FragColor = vec4(vec3(averge, averge, averge), 1.0);

//    Grayscale--more blue
//    --------------------
//    FragColor = texture(screenTexture, TexCoord);
//    float averge = (0.2126*FragColor.r + 0.7152*FragColor.g + 0.0722*FragColor.b) / 3.0 * 2;
//    FragColor = vec4(vec3(averge, averge, averge), 1.0);
//
//    // Kernel effect
//    // -------------
//    float kernel[9] = float[](
//        -1, -1, -1,
//        -1,  9, -1,
//        -1, -1, -1
//    );
//
//    // Blur effect
//    // -----------
//    float blur[9] = float[](
//        1.0/16, 2.0/16, 1.0/16,
//        2.0/16, 4.0/16, 2.0/16,
//        1.0/16, 2.0/16, 1.0/16
//    );
    // Edge effect
    // -----------
    float edge[9] = float[](
        1, 1,  1,
        1, -8, 1,
        1, 1,  1
    );
    
    float offset = 1.0 / 300.0;
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset),
        vec2(0.0, offset),
        vec2(offset, offset),
        vec2(-offset, 0.0),
        vec2(0.0, 0.0),
        vec2(offset, 0.0),
        vec2(-offset, -offset),
        vec2(0.0, -offset),
        vec2(offset, -offset)
    );
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++){
        sampleTex[i] = texture(screenTexture, TexCoord.st + offsets[i]).rgb;
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++){
        col += sampleTex[i] * edge[i];
    }
    FragColor = vec4(col, 1.0);
    
}
