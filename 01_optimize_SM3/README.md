##### Project：do your best to optimize SM3 implementation (software)



**A.代码说明**

该项目对SM3加密方案进行了加速，具体代码实现是使用SIMD程序指令实现了原加密方案中消息扩展的部分，将原本的串行优化为并行，使得程序运行更快。其他部分代码原理已在代码中注释。

SM3加密方案实现参考：https://www.cnblogs.com/20135319zl/p/6366129.html

SIMD程序指令优化参考GmSSL，在GmSSL中将消息扩展的一**部分**用SIMD指令实现：
![gmssl_simd.png](https://github.com/zhong-h/Creatqz/blob/main/01_optimize_SM3/gmssl_simd.png)

<img src="01_optimize_SM3/gmssl_simd.png" alt="gmssl_simd.png" style="zoom:67%;" />

在该项目中我使用SIMD实现了**全部**的消息扩展。



**B.运行指导**

直接运行代码即可

使用Visual Studio运行该代码时，Debug模式会输出详细运行信息，显示全部加密过程，Release模式只会显示最终结果。



**C.代码运行过程截图**

Debug模式：

<img src="C:\Users\zhongh\AppData\Roaming\Typora\typora-user-images\image-20220727084457146.png" alt="image-20220727084457146" style="zoom:67%;" />

Release模式：

<img src="C:\Users\zhongh\AppData\Roaming\Typora\typora-user-images\image-20220727084304588.png" alt="image-20220727084304588" style="zoom:67%;" />
