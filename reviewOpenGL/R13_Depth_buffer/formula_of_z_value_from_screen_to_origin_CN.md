### 引言
&emsp;&emsp;作者在LearnOpenGL教程中的[深度测试](https://learnopengl.com/Advanced-OpenGL/Depth-testing)中使用了一个方程获取原始的 z 值:
这里我们设置这些变量: 
&emsp;&emsp;z<sub>n</sub>: NDC 中的 z 值
&emsp;&emsp;z<sub>b</sub>: 深度缓冲区中的 z 值
&emsp;&emsp;z<sub>v</sub>: 视角空间中的 z 值
&emsp;&emsp;n: 投影平截头体中的近平面值
&emsp;&emsp;f: 投影平截头体中的远平面值
公式如下:
$$z_v=\frac{2fn}{z_n(n-f)+n+f}$$
### 公式推导
&emsp;&emsp;根据教程所述，我们想让 z<sub>n</sub> 通过计算，得到 z<sub>b</sub>。 因为 z<sub>n</sub> 的范围是 [-1, 1]，z<sub>b</sub> 的范围是[0, 1]， 因此我们可以用如下公式:
$$z_b = \frac{z_n + 1}{2}$$
&emsp;&emsp;因为要使用非线性方程去解决深度缓冲中的精度问题, 我们需要通过公式逆向求出 z 的原始值(z<sub>v</sub>), 我们一步一步推导:
$$z_b = \frac{\frac{1}{z_v} - \frac{1}{n}}{\frac{1}{f}-\frac{1}{n}}\tag{1.1}$$
&emsp;&emsp;分子分母同乘 nf
$$z_b = \frac{\frac{nf}{z_v} - f}{n-f}\tag{1.2}$$
$$z_b(n-f) = \frac{nf}{z_v} - f\tag{1.3}$$
$$z_b(n-f)+f = \frac{nf}{z_v}\tag{1.4}$$
$$z_v = \frac{nf}{z_b(n-f)+f}\tag{1.5}$$
&emsp;&emsp;我认为，使用上面的公式已经能够计算出 z<sub>v</sub> 的值了。然而根据作者的代码，他似乎想找出 z<sub>v</sub> 和 <sub>n</sub> 的之间的关系。
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
&emsp;&emsp;所以，我们用 z<sub>n</sub> 替换 z<sub>b</sub> 并且一步步整理等式：
$$z_v = \frac{nf}{\frac{z_n + 1}{2}(n-f)+f}\tag{2.1}$$
$$z_v = \frac{2nf}{(z_n + 1)(n-f)+2f}\tag{2.2}$$
$$z_v = \frac{2nf}{z_nn-z_nf+n-f+2f}\tag{2.3}$$
$$z_v = \frac{2nf}{z_n(n-f)+n+f}\tag{2.4}$$
$$z_v = \frac{2nf}{f+n-z_n(f-n)}\tag{2.5}$$
### 补充
&emsp;&emsp; 我认为作者在片段着色器中做了一些多余的事情，他将 gl_FragCoord.z(z<sub>b</sub>) 转换到了 NDC(z<sub>n</sub>)，然后将 z<sub>v</sub>-z<sub>b</sub> 等式转换成了 z<sub>v</sub>-z<sub>n</sub> 等式而不是直接使用 z<sub>b</sub> 值和 z<sub>v</sub>-z<sub>b</sub> 等式。我在我的程序中尝试两个不同的等式，他们结果不一致。z<sub>v</sub>-z<sub>n</sub> 的等式比另一个等式需要更远的距离才能看见两个立方体。
 
&emsp;&emsp;下列作者的片段着色器代码中也出现了错误
```c
    float depth = LinearizeDepth(gl_FragCoord.z) / far;
```
&emsp;&emsp;因为函数 LinearizeDepth(float depth)仅仅返回 z<sub>v</sub>, 因此我们需要根据线性等式将代码写成如下形式:
```c
    float depth = (LinearizeDepth(gl_FragCoord.z)-near) / (far - near);    
```
线性等式:
$$z_b = \frac{z_v - n}{f - n}$$