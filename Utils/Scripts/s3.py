
import ctypes

def addStuffing(inMessage):

    outMessage =  bytearray([0xC0,0xC0,0xC0])
    outMessage.append(0xC0)
    
    for b in inMessage:
        if b==0xC0:
            outMessage.append(0xDB)
            outMessage.append(0xDC)
        elif b==0xDB:
            outMessage.append(0xDB)
            outMessage.append(0xDD)
        else:            
            outMessage.append(b)
    return outMessage
   

def getCrcTableValue(index):

    c = ctypes.c_uint32(index)
    for k in range(0,8):
        c = ctypes.c_uint32(c.value//2)
        if (c.value and 1)==0:
            print(k,"A",c.value)
            c = ctypes.c_uint32(c.value ^ 0xedb88320)
        else:
            print(k,"B",c.value)
            pass
    return c.value




m1 = bytes([0xDB,0x02,0x03,0xF3])
mm1 = addStuffing(m1)


c = []
for k in range(0,1):
    c.append((getCrcTableValue(k)))
    print("___",c[k])




    

