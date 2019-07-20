### Introduction
&emsp;&emsp;The author use a formular to get origin z-value of a fragment via the following formular in the LearnOpenGL tutorials [Depth-testing](https://learnopengl.com/Advanced-OpenGL/Depth-testing):
here we set: 
&emsp;&emsp;z<sub>n</sub>: z-value in NDC
&emsp;&emsp;z<sub>b</sub>: z-value in depth buffer
&emsp;&emsp;z<sub>v</sub>: z-value in view space
&emsp;&emsp;n: near value of projection's frustum
&emsp;&emsp;f: far value of projection's frustum
the formual is:
$$z_v=\frac{2fn}{z_n(n-f)+n+f}$$
### Formula Derivation
&emsp;&emsp;According to the tutorial, we need to get z<sub>c</sub> by calculating with z<sub>s</sub>. And because the range of z<sub>c</sub> is [-1, 1] and z<sub>s</sub> is [0, 1], so we can use the following formula:
$$z_b = \frac{z_n + 1}{2}$$
&emsp;&emsp;Since we use non-linear equation to solve the depth buffer precision problem, we need to reverse it to get the origin v-value (z<sub>v</sub>), and let's do this step by step:
$$z_b = \frac{\frac{1}{z_v} - \frac{1}{n}}{\frac{1}{f}-\frac{1}{n}}\tag{1.1}$$
&emsp;&emsp;Molecular and denominator multiply by near*far
$$z_b = \frac{\frac{nf}{z_v} - f}{n-f}\tag{1.2}$$
$$z_b(n-f) = \frac{nf}{z_v} - f\tag{1.3}$$
$$z_b(n-f)+f = \frac{nf}{z_v}\tag{1.4}$$
$$z_v = \frac{nf}{z_b(n-f)+f}\tag{1.5}$$
&emsp;&emsp;As I see, it's enough to calculate z_v by equation above. However, according to author's fragment shader code, it seems that he wants to find the relationship between z<sub>v</sub> and z<sub>n</sub>.
```c
#version 330 core
out vec4 FragColor;

float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{             
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    FragColor = vec4(vec3(depth), 1.0);
}
```
&emsp;&emsp;So, we use z<sub>n</sub> to replace z<sub>b</sub> and finish the equation step by step:
$$z_v = \frac{nf}{\frac{z_n + 1}{2}(n-f)+f}\tag{2.1}$$
$$z_v = \frac{2nf}{(z_n + 1)(n-f)+2f}\tag{2.2}$$
$$z_v = \frac{2nf}{z_nn-z_nf+n-f+2f}\tag{2.3}$$
$$z_v = \frac{2nf}{z_n(n-f)+n+f}\tag{2.4}$$
$$z_v = \frac{2nf}{f+n-z_n(f-n)}\tag{2.5}$$
### Addition
&emsp;&emsp;I think the author does something extra thing in the fragment shader code. He transforms the gl_FragCoord.z(z<sub>b</sub>) value to NDC(z<sub>n</sub>) and then transforms z<sub>v</sub>-z<sub>b</sub> equation to  z<sub>v</sub>-z<sub>n</sub> euqation instead of use  z<sub>b</sub> value and z<sub>v</sub>-z<sub>b</sub> euqation. I try the two equations in my program and find that they have different result, which is the z<sub>v</sub>-z<sub>n</sub> equation needs further distance to see two cubes clearly than another.
&emsp;&emsp;There also has a fault in author's following fragment shader code
```c
    float depth = LinearizeDepth(gl_FragCoord.z) / far;
```
&emsp;&emsp;Since the LinearizeDepth(float depth) function only return the z<sub>v</sub>, we need modify this code to following according to the linear equation:
```c
    float depth = (LinearizeDepth(gl_FragCoord.z)-near) / (far - near);    
```
linear equation:
$$z_b = \frac{z_v - n}{f - n}$$