##### Project：implement the Rho method of reduced SM3



**A.代码说明**

该项目使用Python实现

该方法是一种改进的生日攻击，其并不会优化运行速度，但是可以**节省更多空间**。与之前生日攻击中使用字典的方法类似，该方法并不会先开辟一可以容纳所有原像的空间，而是**边寻找碰撞边存储**，使得一些不必要的信息对可以不用存储。

若按照该方法找到一对相同的数据，则一定能够找到碰撞，其正确性证明如下：

![proof.png](https://github.com/zhong-h/Creatqz/blob/main/image/03_Rho_method/proof.png)

SM3实现参考：https://blog.csdn.net/weixin_45688634/article/details/123292997

**B.运行指导**

main.py与sm3.py放到同一级目录下，直接运行



**C.代码运行过程截图**

通过该方法找到了多个不同的碰撞

![rho_atk.png](https://github.com/zhong-h/Creatqz/blob/main/image/03_Rho_method/rho_atk.png)

