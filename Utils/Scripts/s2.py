import serial,time

with serial.Serial('COM10',115200,timeout=0.05) as ser:

    startTime = time.time()

    for k in range(1,10):

        x = 'привет!'.encode('utf-8')
        y = bytes([0xC0,0xC1,0x34])
        
        ser.write(y)
        print(y)
        time.sleep(1.0)
        

    ElapsedTime = time.time() - startTime
    print(ElapsedTime)
    
