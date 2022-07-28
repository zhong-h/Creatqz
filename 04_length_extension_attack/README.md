##### Project：implement length extension attack for SM3, SHA256, etc.

**A.代码说明**

该项目使用Python实现

对SM3进行长度扩展攻击，设已知消息$m$，其哈希值为$H(m)$，扩展部分为$ext$，则对其进行长度扩展攻击的原理如下图所示：

![](C:\Users\zhongh\AppData\Roaming\Typora\typora-user-images\image-20220727164728682.png)

按照该图进行代码实现即可实现对SM3的长度扩展攻击

事实上对哈希函数的长度扩展攻击没有实际意义，多用于教学中

SM3实现参考：https://blog.csdn.net/weixin_45688634/article/details/123292997

**B.运行指导**

len_exten_atk.py与sm3.py放到同一级目录下，直接运行

**C.代码运行过程截图**

代码会自动计算出添加扩展内容后的新消息，并计算出其哈希值与长度扩展攻击得到的结果进行对比，以验证长度扩展攻击是否成功。

![len_exten_atk.png](https://github.com/zhong-h/Creatqz/blob/main/image/04_length_extension_attack/len_exten_atk.png)

