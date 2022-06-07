import sm3
import random


def rho_method(exm):
    num = int(exm/4)                # 16进制位数
    x = hex(random.randint(0, 2**(exm+1)-1))[2:]
    x_a = sm3.SM3(x)                # x_a = x_1
    x_b = sm3.SM3(x_a)              # x_b = x_2
    i = 1
    while x_a[:num] != x_b[:num]:
        i += 1
        x_a = sm3.SM3(x_a)              # x_a = x_i
        x_b = sm3.SM3(sm3.SM3(x_b))     # x_b = x_2i
    x_b = x_a           # x_b = x_i
    x_a = x             # x_a = x
    for j in range(i):
        if sm3.SM3(x_a)[:num] == sm3.SM3(x_b)[:num]:
            return sm3.SM3(x_a)[:num], x_a, x_b
        else:
            x_a = sm3.SM3(x_a)
            x_b = sm3.SM3(x_b)


if __name__ == '__main__':
    example = 8    # 此处进行前8bit的碰撞以作演示
    col, m1, m2 = rho_method(example)
    print("找到碰撞！")
    print("消息1:", m1)
    print("消息2:", m2)
    print("两者哈希值的前{}bit相同，16进制表示为:{}".format(example, col))