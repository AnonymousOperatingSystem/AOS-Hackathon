from pyaos.chain import chain
from pyaos.wallet import wallet
from pyaos.aosencryfunc import ecdsa_verifing, ecdsa_sign
from pprint import pprint

aos_url = "http://api.aos.plus:8888/"
print("链接口测试：")
_chain = chain(aos_url)
message = _chain.get_block(block_num_or_id=1)
pprint(message)
print("=================================")



print("钱包接口测试：")
_wallet = wallet(aos_url)
message = _wallet.wallet_create("[YOUR WALLET NAME]")
print(message)
print("==================================")




print("AOS 内置加密函数测试：")
print("===================================")
data = b"hello, AOS"
privkey = "0x2e66dbbfc7b8ae9ebd3bdce831509ce5136bc0a54055eaa4bff364e07291f5ab"
signature = ecdsa_sign(data, privkey)

# 正确的签名进行验签
verify = ecdsa_verifing(signature=signature[0], data=data, verifing_key=signature[2])
print("data：", data)
print("签名内容：", signature[0])
print("验签结果：", verify)
print()

# 错误的签名进行验签
test_signature = 'test signature'
verify_false = ecdsa_verifing(signature=test_signature, data=data, verifing_key=signature[2])

print("data：", data)
print("签名内容：", test_signature)
print("验签结果：", verify_false)
print("===================================")