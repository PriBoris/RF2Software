
#testing Crc32 functions


import ctypes,random

def getCrc32Table():
    table = []
    for i in range(0,256):
        c = ctypes.c_uint32(i)
        for k in range(0,8):
            if (c.value & 1)!=0:
                c = ctypes.c_uint32(c.value >> 1)
                c = ctypes.c_uint32(c.value ^ 0xedb88320)
            else:
                c = ctypes.c_uint32(c.value >> 1)
        table.append(c)
    return table

def getCrc32InitValue():
    return ctypes.c_uint32(0)

def crc32AppenByte(nextByte,crcValue):

    c = crcValue.value ^ 0xFFFFFFFF
    tableIndex = ((c ^ nextByte) & 0xFF);
    tableValue = getCrc32Table()[tableIndex].value
    newCrcValue = tableValue ^ (c>>8) ^ 0xFFFFFFFF
    return ctypes.c_uint32(newCrcValue)




##crc32Table = getCrc32Table()
##for i in range(0,256):
##    print(hex(crc32Table[i].value),",//",i)

testSequence0 =  bytearray([0x00])
crcValue = getCrc32InitValue()
for b in testSequence0:
    crcValue = crc32AppenByte(b,crcValue)
print("testSequence0",hex(crcValue.value))

testSequence1 =  bytearray([0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00])
crcValue = getCrc32InitValue()
for b in testSequence1:
    crcValue = crc32AppenByte(b,crcValue)
print("testSequence1",hex(crcValue.value))  

testSequence2 =  bytearray()
sequenceLength2 = 8
for k in range(0,sequenceLength2):
    testSequence2.append(random.randint(0, 255))
crcValue = getCrc32InitValue()
for b in testSequence2:
    crcValue = crc32AppenByte(b,crcValue)
    print(b,',')
print("testSequence2",hex(crcValue.value))  

testSequence3 =  bytearray([0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF])
crcValue = getCrc32InitValue()
for b in testSequence3:
    crcValue = crc32AppenByte(b,crcValue)
print("testSequence3",hex(crcValue.value))  



