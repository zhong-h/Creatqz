from gmssl import sm2, sm3, func
import ecdsa
import hmac
import hashlib

G = ecdsa.NIST256p.generator
p = G.curve().p()       # 椭圆曲线上素域的阶


def gen_k(msg, x):      # RFC6979
    h1 = sm3.sm3_hash(func.bytes_to_list(msg))
    bh1 = bytes(h1, encoding="utf-8")
    bx = bytes(x, encoding='utf-8')

    v = b'\x01' * 32
    k = b'\x00' * 32
    k = hmac.new(k, v + b'\x00' + bx + bh1, hashlib.sha256).digest()
    v = hmac.new(k, v, hashlib.sha256).digest()
    k = hmac.new(k, v + b'\x01' + bx + bh1, hashlib.sha256).digest()
    v = hmac.new(k, v, hashlib.sha256).digest()
    return hex(int.from_bytes(hmac.new(k, v, hashlib.sha256).digest(), "big") % p)[2:]


if __name__ == "__main__":
    # 16进制的公钥和私钥
    private_key = '00B9AB0B828FF68872F21A837FC303668428DEA11DCD1B24429D0C99E24EED83D5'
    public_key = 'B9C9A6E04E9C91F7BA880429273747D7EF5DDEB0BB2FF6317EB00BEF331A83081A6994B8993F3F5D6EADDDB81872266C87C018FB4162F5AF347B483E24620207 '
    sm2_crypt = sm2.CryptSM2(public_key=public_key, private_key=private_key)
    message1 = b"SDUCyberspaceSecurity"  # bytes类型
    message2 = b"2022630"   # bytes类型
    k1 = gen_k(message1, private_key)
    k2 = gen_k(message2, private_key)
    sign1 = sm2_crypt.sign(message1, k1)
    sign2 = sm2_crypt.sign(message2, k2)
    print("For the message:{}\nk is {}\nsignature is {}".format(message1.decode(), k1, sign1))
    print("The result of verification about it is:{}".format(sm2_crypt.verify(sign1, message1)))
    print("_______________________________________________________________________")
    print("For the message:{}\nk is {}\nsignature is {}".format(message2.decode(), k2, sign2))
    print("The result of verification about it is:{}".format(sm2_crypt.verify(sign2, message2)))





