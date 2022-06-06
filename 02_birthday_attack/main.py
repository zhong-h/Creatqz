import sm3


def bir_atk(exm):
    num = int(2 ** (exm / 2))  # 所要搜寻原像空间的大小
    ans = [-1] * 2**exm        # 这里即用空间换时间，也可以使用字典查询，但是当碰撞bit数较大时运行较慢
    for i in range(num):
        temp = int(sm3.SM3(str(i))[0:int(exm / 4)], 16)
        if ans[temp] == -1:
            ans[temp] = i
        else:
            return hex(temp), i, ans[temp]


if __name__ == '__main__':
    example = 16    # 此处进行前16bit的碰撞以作演示
    col, m1, m2 = bir_atk(example)
    print("找到碰撞，消息{}与{}哈希值的前{}bit相同，16进制表示为:{}。".format(m1, m2, example, col))
