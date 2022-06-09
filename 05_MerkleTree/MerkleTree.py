from hashlib import sha256
import random
import string
import math


# 随机生成data
def gen_data(length):
    res = []
    for i in range(length):
        block = [random.choice(string.digits + string.ascii_letters) for i in range(5)]  # 一个消息块长为5
        res.append(''.join(block))
    return res


# 生成MerkleTree
def gen_mkt(data):
    depth = math.ceil(math.log2(len(data)) + 1)  # merkle tree深度
    mktree = [[sha256(i.encode()).hexdigest() for i in data]]  # merkle tree的第0层(倒置)
    for i in range(depth - 1):
        len_lay = len(mktree[i])  # 第i层消息块个数
        mkt_lay = [sha256(mktree[i][j * 2].encode() + mktree[i][j * 2 + 1].encode()).hexdigest() for j in
                   range(int(len_lay / 2))]  # merkle tree的第i+1层
        if len_lay % 2 != 0:
            mkt_lay.append(mktree[i][-1])  # 若块数为奇数，则最后一个块直接放入下一层
        mktree.append(mkt_lay)
    return mktree


def proof(spec_elm, mktree, root):
    hash_se = (sha256(spec_elm.encode())).hexdigest()
    if hash_se in mktree[0]:
        index_se = mktree[0].index(hash_se)  # 指定元素在数第一层的索引值
    else:
        return "This message isn't in the data."
    depth = len(mktree)  # merkle tree深度
    audit_path = []  # 审计路线
    for i in range(depth - 1):
        if index_se % 2 == 0:  # 左子结点
            if len(mktree[i]) - 1 != index_se:  # 该结点不是该层最后的一个单独结点
                audit_path.append(['l', mktree[i][index_se + 1]])
        else:  # 右子结点
            audit_path.append(['r', mktree[i][index_se - 1]])
        index_se = int(index_se / 2)    # 更新索引值
    for ele in audit_path:
        if ele[0] == 'l':
            hash_se = sha256(hash_se.encode() + ele[1].encode()).hexdigest()
        else:
            hash_se = sha256(ele[1].encode() + hash_se.encode()).hexdigest()
    if hash_se == root:
        return "This message is in the merkle tree."
    else:
        return "This message is in the data but it isn't in the merkle tree."


if __name__ == "__main__":
    index = random.randint(0, 99999)
    data = gen_data(100000)
    data2 = gen_data(100000)
    spec_element = data[index]       # inclusion proof
    data2[index] = spec_element      # exclusion proof 指定消息在data中
    spec_element2 = '12345'          # exclusion proof 指定消息不在data中(大概率)
    mktree = gen_mkt(data)
    mktree2 = gen_mkt(data2)
    root = mktree[-1][0]
    # for i in mktree:          # 输出merkle tree
    #     print(i, "\n")
    print("For message:", spec_element, proof(spec_element, mktree, root))
    print("For message:", spec_element, proof(spec_element, mktree2, root))
    print("For message:", spec_element2, proof(spec_element2, mktree, root))
