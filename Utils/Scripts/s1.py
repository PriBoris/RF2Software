import serial,time



def serialPortsList(maxPortIndex):

    """ Lists serial port names on windows platform
        from  COM1 to COM[maxPortIndex]

        :returns:
            A list of the serial ports available on the system
    """

    ports = ['COM{0}'.format(i) for i in range(1,maxPortIndex+1)]

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result



print(serialPortsList(32))


with serial.Serial('COM5',115200,timeout=1) as ser:

    startTime = time.time()

    rxBytesCounter = 0
    for k in range(1,10):
        x = ser.read(size=1024)
        rxBytesCounter = rxBytesCounter+len(x)
        print(rxBytesCounter,"bytes received so far")

    ElapsedTime = time.time() - startTime
    print(ElapsedTime)
    
