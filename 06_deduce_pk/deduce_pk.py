import ecdsa
import random
import hashlib


if __name__ == "__main__":
    G = ecdsa.NIST256p.generator
    n = G.order()
    d_A = random.randrange(1, n - 1)      # 生成私钥d_A
    # 生成公私钥对象
    public_key = ecdsa.ecdsa.Public_key(G, G * d_A)
    private_key = ecdsa.ecdsa.Private_key(public_key, d_A)
    message = "SDUCyberspaceSecurity"
    e = int(hashlib.sha1(message.encode("utf8")).hexdigest(), 16)   # Hash(m)
    k = random.randrange(1, n - 1)    # 生成临时密钥
    sign = private_key.sign(e, k)      # 生成签名(r,s)
    r = sign.r
    s = sign.s
    curve = G.curve()
    x = r       # P = kG,x为P横坐标,x = r mod n
    y_square = (pow(x, 3, curve.p()) + (curve.a() * x) + curve.b()) % curve.p()     # y^2 = x^3 + ax + b
    y_hat = ecdsa.numbertheory.square_root_mod_prime(y_square, curve.p())
    y = y_hat if y_hat % 2 == 0 else curve.p() - y_hat

    R1 = ecdsa.ellipticcurve.PointJacobi(curve, x, y, 1, n)
    Q1 = ecdsa.numbertheory.inverse_mod(r, n) * (s * R1 + (-e % n) * G)     # P = (s*kG - e*G)*r^(-1) mod n
    Pk1 = ecdsa.ecdsa.Public_key(G, Q1)

    R2 = ecdsa.ellipticcurve.PointJacobi(curve, r, -y, 1, n)
    Q2 = ecdsa.numbertheory.inverse_mod(r, n) * (s * R2 + (-e % n) * G)     # P = (s*kG - e*G)*r^(-1) mod n
    Pk2 = ecdsa.ecdsa.Public_key(G, Q2)
    # 恢复的公钥不止一个
    de_p_key = [[Pk1.point.x(), Pk1.point.y()], [Pk2.point.x(), Pk2.point.y()]]
    p_key = [public_key.point.x(), public_key.point.y()]
    if p_key in de_p_key:
        print("Deduce public key from signature successfully!")
        print("The deduced public key is: \n{} or\n{}".format(de_p_key[0], de_p_key[1]))
        print("The true public key is:\n", p_key)
    else:
        print("Didn't deduce public key from signature")