### The process of rendering text in OpenGL use FreeType
#### 1. Initial Freetype
1. Initial library, face
2. Set the size of Pixel   
#### 2. Create Character map
1. Create Character struct (texture id, size, bearing, advance) 
[Note: ```glPixelStorei(GL_UNPACK_ALIGNMENT, 1);```]
2. Create map of between character and struct
3. Use Freetype function load info of a character texture(size, bearing, buffer, advance)
#### 3. Render Character
1. Create corresponding VAO and VBO
2. Create a Test string
3. Get each character map info of the text and load the info. Create texture, position, texture coordinate and calculate the offset in x.
4. Render texture.

&emsp;&emsp;I write a class to render text use FreeType, and upload it in Header folder, you can down and use.