'''
@ Project Name: Picture Encrypt

@ Author: Honbey, honbey@honbey.com
@ Date: 2019-04-17
@ Desc: homework of Security Protocol
'''
from Crypto.Cipher import AES
from Crypto.Hash import SHA
from Crypto import Random

from zlib import (
    decompress, compress, crc32
)

'''
 28-60行代码是自己对PNG文件格式的探索
 PNG文件我选的很简单，只有一个IDAT数据块
 图像经过压缩删减其他无关信息
 仅保证对logo.png图片有用
 这种解码过程不通用
 其他图片可能有多个IDAT块和辅助信息块
 本程序无法处理复杂情况
 仅仅研究ECB和CBC模式加密后的图象
 解密加密图像函数没写了
 过程都差不多
'''
with open("logo.png", "rb") as inputI : 
    picStr = inputI.read()
    print(picStr[0:8].hex()) # PNG flag
    print(picStr[8:12].hex()) # IHDR length
    print(picStr[12:16].hex()) # IHDR flag
    print(picStr[16:20].hex(), picStr[20:24].hex()) # height and weight
    print(picStr[24:28].hex()) # bit depth, color tyope，compression method, filter method
    print(picStr[28:29].hex()) # interlace method
    print(picStr[29:33].hex()) # CRC 
    # ...
    
    # find IDAT
    indexIDAT = picStr.hex().find('49444154') // 2 
    #print(indexIDAT)
    
    # the length of IDAT
    lengthStr = picStr[indexIDAT-4:indexIDAT].hex()
    print(lengthStr)
    # convert to int
    lengthHex = int(lengthStr, 16)
    
    # get string from IDAT chunk
    # part need calculate CRC
    idatCRCStr = picStr[indexIDAT:indexIDAT+4+lengthHex]
    # IDAT data part
    idatStr = picStr[indexIDAT+4:indexIDAT+4+lengthHex]
    # CRC result
    crcStr = picStr[indexIDAT+4+lengthHex:indexIDAT+4+lengthHex+4]
    print(crcStr.hex())

#len(idatStr.hex())
#hex(crc32(idatCRCStr)) # verify CRC

# now begin to encrypt

key = b"hainanuxxaqzhang"
iv = Random.new().read(AES.block_size)
cipher = AES.new(key, AES.MODE_ECB)

# ECB
# encryption
def ecbEncrypt(plainStr):
    paddingStr = b"0000-0000-0000-0000".replace(b"-", b"")
    length = len(plainStr)
    paddingLen = 16 - length % AES.block_size
    print("Padding Length:", paddingLen)
    toEncryptStr = plainStr + paddingStr[0:paddingLen]
    cipherStr = cipher.encrypt(toEncryptStr)
    #print("Cipher Text: ", cipherStr)
    return cipherStr


# Encrypt Picture and make sure the encrypted picture can be open
# decompress IDAT Chunk
rowCompress = decompress(idatStr)

# encrypt row data
rowEncrypt = ecbEncrypt(rowCompress)
# compress encrypt data
encCompress = compress(rowEncrypt)

# must remove '0x'
lenHex = hex(len(encCompress))[2:10]
# convert to bytes
lenBytes = b"\x00\x00" + bytes.fromhex(lenHex)

# must remove '0x'
newCRCHex = hex(crc32(picStr[indexIDAT:indexIDAT+4] + encCompress))[2:10]
# convert to bytes
newCRCBytes = bytes.fromhex(newCRCHex)

# PNG flag, IHEA... , IDAT chunk length ,  'IDAT' , IDAT data ,  IDAT CRC , IEND chunk
newStr = picStr[0:indexIDAT-4] \
    + lenBytes \
    + picStr[indexIDAT:indexIDAT+4] \
    + encCompress \
    + newCRCBytes \
    + picStr[indexIDAT+4+lengthHex+4:]

with open("output-ecb.png", "wb") as output:
    output.write(newStr)

# CBC
# encryption
cipher = AES.new(key, AES.MODE_CBC, iv)
def cbcEncrypt(plainStr):
    paddingStr = b"0000-0000-0000-0000".replace(b"-", b"")
    length = len(plainStr)
    paddingLen = 16 - length % AES.block_size
    print("Padding Length:", paddingLen)
    toEncryptStr = plainStr + paddingStr[0:paddingLen]
    cipherStr = cipher.encrypt(toEncryptStr)
    #print("Cipher Text: ", cipherStr)
    return cipherStr


# Encrypt Picture and make sure the encrypted picture can be open
# decompress IDAT Chunk
rowCompress = decompress(idatStr)

# encrypt row data
rowEncrypt = cbcEncrypt(rowCompress)
# compress encrypt data
encCompress = compress(rowEncrypt)

# must remove '0x'
lenHex = hex(len(encCompress))[2:10]
# convert to bytes
# the function i don't how to use it correctly
# so i choose input length
lenBytes = b"\x00\x01\x39\x0f" 

# must remove '0x'
newCRCHex = hex(crc32(picStr[indexIDAT:indexIDAT+4] + encCompress))[2:10]
# convert to bytes
newCRCBytes = bytes.fromhex(newCRCHex)

# PNG flag, IHEA... , IDAT chunk length ,  'IDAT' , IDAT data ,  IDAT CRC , IEND chunk
newStr = picStr[0:indexIDAT-4] \
    + lenBytes \
    + picStr[indexIDAT:indexIDAT+4] \
    + encCompress \
    + newCRCBytes \
    + picStr[indexIDAT+4+lengthHex+4:]

with open("output-cbc.png", "wb") as output:
    output.write(newStr)