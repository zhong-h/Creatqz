##### Project：report on the application of this deduce technique in Ethereum with ECDSA

**A.代码说明**

该项目使用Python完成

签名的生成比较简单，直接调用Python中的ecdsa模块，确定使用的椭圆曲线为NIST256p，使用库中的函数和类即可完成，在此不做赘述。

得到消息$m$和其对应的签名$(r,s)$后，我们需要恢复能够成功验证签名的公钥Q(曲线上的点)：

$s=k^{-1}(e+dr)\Rightarrow s\cdot kG=eG+dG\cdot r\Rightarrow s\cdot kG-eG=Q\cdot r\Rightarrow Q=r^{-1}(s\cdot kG-eG)$

观察上述等式，已知$s,r,e=H(m)$，则只需知道$P=kG$，即可恢复出正确的验签公钥$Q$.

但是敌手并不知道$P$，只知道$r \equiv x \mod n，x\in[1,p],p$为椭圆曲线上素域的阶，通常来说$n\lt p$，因此一个$r$可能对应两个$x$：

1. 若$r\lt p-n$，则$x=r$或$ x=r+n$
2. 若$r\gt p-n$，则$x=r$

在$x$被确定后，需要用其确定$P$，根据椭圆曲线的公式可知一个$x$对应了两个曲线上的点$P_1,P_2$，且这两个点生成的$Q$都可以用于验签，所以理论上来看最多可以恢复4个能够正确验签的公钥。

恢复公钥需要用到的各类计算与公式在ecdsa库中都已有，只需调用即可。

**B.运行指导**

直接运行deduce_pk.py即可

**C.运行过程截图**

该程序首先会对消息message=“SDUCyberspaceSecurity”进行签名，然后再根据消息和签名进行公钥恢复，并将恢复出的公钥与原公钥进行比对，判断是否成功恢复

![deduce_pk.png](https://github.com/zhong-h/Creatqz/blob/main/image/06_deduce_pk/deduce_pk.png)

