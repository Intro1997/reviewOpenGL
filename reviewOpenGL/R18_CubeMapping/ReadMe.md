&emsp;&emsp;I find the correct codes on internet at [here](https://www.jianshu.com/p/ad691b3ea9d5). At the bottom of the passage, the author give the address of LearnOpenGL author's source codes. This code use glew to replace the glad, so I rewrite it in glad.</br>
&emsp;&emsp;Besides alerting the model.h, mesh.h and application's loading textures code, we also need to rewrite the fragment shader. By loading the reflection textures and making a judgement that gives up rendering the r component whose value is less than 0.1 of the reflection textures,  we finally get the reflect color, and the result effect is to add the reflect color and diffuse color.</br>
<<<<<<< HEAD
&emsp;&emsp;I upload the whole project at here (the project name is OpenGLTest) and hope these codes can help you!</br>
=======
&emsp;&emsp;I upload the whole project at here (the project name is OpenGLTest) and hope these codes can help you!
>>>>>>> cb64e8a0cd8ddf6bf25ba5314250a116eeecb5f9