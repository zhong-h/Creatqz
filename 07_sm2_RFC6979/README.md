##### Project: impl sm2 with RFC6979



**A.代码说明**

该项目使用Python实现

SM2公钥密码算法中有一步需要生成一个随机数k，在原本的SM2加密方案中k是随机生成的，而RFC6979对于k的生成进行了改进。在本项目中我们主要关注**根据RFC6979的标准生成k**，对于其他部分的代码我们大多采用调库来实现。

[1]SM2调用参考：https://www.cnblogs.com/pythonywy/p/13638806.html

[2]RFC6979文档：https://datatracker.ietf.org/doc/html/rfc6979



**B.运行指导**

直接运行sm2_RFC6979.py即可



**C.代码运行过程截图**

![RFC6979.png](https://github.com/zhong-h/Creatqz/blob/main/image/07_m2_RFC6979/RFC6979.png)


