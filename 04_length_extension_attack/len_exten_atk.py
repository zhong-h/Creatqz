import sm3


def len_ext_atk(msg, ext, n):
    h_m = sm3.SM3(msg)              # H(m)
    Hm = []
    for i in range(8):
        Hm.append(int(h_m[i*8:i*8+8], 16))
    len_e = hex((n + len(ext))*4)[2:]   # 总消息长度
    len_e = (16 - len(len_e)) * '0' + len_e
    ext = ext + '8'
    if len(ext) % 128 > 112:
        ext = ext + '0' * (128 - len(ext) % 128 + 112) + len_e
    else:
        ext = ext + '0' * (112 - len(ext) % 128) + len_e
    ext_g = sm3.Group(ext)      # 数据分组
    n_g = len(ext_g)            # 分组个数
    V = [Hm]
    for i in range(n_g):
        V.append(sm3.CF(V, ext_g, i))
    res = ''
    for x in V[n_g]:
        res += hex(x)[2:]
    return res


if __name__ == '__main__':
    message = '616263'     # 原始消息
    extend = '123'         # 扩展部分
    if len(message) % 128 < 112:
        n = (int(len(message) / 128) + 1) * 128  # 16进制数个数
    else:
        n = (int(len(message) / 128) + 2) * 128  # 16进制数个数
    len_m = hex(len(message)*4)[2:]
    len_m = (16 - len(len_m)) * '0' + len_m  # 消息长度
    pad = n - len(message) - 16 - 1                # 补0个数=总长-消息-消息长度-1
    new_m = message + '8' + pad*'0' + len_m + extend    # new_m = message||1000...|| |message| ||extend
    res_new = sm3.SM3(new_m)
    res = len_ext_atk(message, extend, n)
    print("新消息的哈希值为:", res_new)
    print("长度扩展攻击结果:", res)
    if res_new == res:
        print("长度扩展攻击成功!")
    else:
        print("长度扩展攻击失败!")

