vs: vertex shader
fs: fragment shader
#### The process of create SSAO
##### 1. Render the scene in MDR FBO
    1. Create FBO with position, normal and albedo buffer
    2. Create fs and vs then configure them
    3. Render them
##### 2. Use MDR FBO's buffer info render SSAO
    1. Create SSAO FBO with GL_RED buffer
    2. Create random sample point and noises (use to rotate sample). Store noises in 4*4 texture
    3. Create fs and vs. Create TBN (view space) in fs and rotate sample points. 
    [NOTE: you should compare the sample point depth and depth of position buffer's z-axis value geting with sample point coordinates]
    4. Calculate SSAO value and out it to FragColor;
##### 3. Use blur core effect to remove the noise point in SSAO
    1. Create blur FBO
    2. Create vs and fs 
    3. Do blur effect (don't forget calculate the textureSize)
##### 4. Combine Color buffer with SSAO buffer
    1. Prepare your light calculation vertex, fragment Shader and relevant info (don't forget transfrom them to view space)
    2. Use ssao values as ambient.