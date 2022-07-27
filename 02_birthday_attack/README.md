##### Project：implement the naïve birthday attack of reduced SM3



**A.代码说明**

该项目使用Python实现

SM3的实现较为简单，参考了已有代码并进行了简单修改，在此不做赘述

SM3实现参考：https://blog.csdn.net/weixin_45688634/article/details/123292997



若使用生日攻击的思想对 长$l$ bit的串进行攻击，则进行$2^{l/2}$次搜索即可以较高概率找到碰撞。在这里要对16 bit进行碰撞，则需要在大小为$2^8$的原像空间中进行碰撞。

在确定原像空间大小后，直接建立一个对应大小的列表并进行碰撞。相当于**用空间换时间**，消耗了较多的存储空间但程序**运行较快**，其缺点是当要攻击的串更长时，可能会存在空间不足的问题。

这里也可以使用字典的方法进行攻击，不需要提前创建一个很大的空间，而是每次都在已有的信息中比对碰撞，这样**可以对更长的串进行攻击**，其缺点是运行时间较慢，攻击较长的串会消耗大量时间。



**B.运行指导**

main.py与sm3.py放到同一级目录下，直接运行



**C.代码运行过程截图**

![bit_atk.png](https://github.com/zhong-h/Creatqz/blob/main/image/02_birthady_attack/bir_atk.png)

