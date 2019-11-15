#### 1. Assimp only can load FBX's diffuse texture.
&emsp;&emsp;I try to use assimp to load diffuse texture of the FBX in tutorial but I failed. But you also can use stbi library to load them. And please use GL_REPEAT.
#### 2. The final effect is lighter than tutorial
&emsp;&emsp;Actually I don't know the reason but you can alter the gamma parameter in PBR.fs to change the result. 