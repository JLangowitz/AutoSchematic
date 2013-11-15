"""
Team AutoSchematic
PoE F2013
Python to Arduino Serial Communication
Contributors: Josh Langowitz
"""


def sendWires(ser, wireData):
    """
    sends the data from each wire over to the Arduino for cutting

    wireData: list of tuples containing a color char and a wire length int
    """
    for color, wireLength in wireData:
        print color, wireLength
        makeWire(ser, color, wireLength)
        waitForCompletion(ser)

def makeWire(ser, color, wireLength, commandLength=1):
    """
    Tells the Arduino to make a wire of the given color (if we get that working)
    with the correct wire length 

    Our protocol for serial communication is a command character followed by
    a character for length of command, a character for color, and a character
    for wire length. In this case, command length is always 2.

    ser: pyserial object
    color: char- color of wire
    wireLength: int- length of wire
    commandLength: int- length of command, should always be 6
    """
    print commandLength
    print 'W%c%c' %(commandLength, int(wireLength))
    print ser.write('W%c%c' %(commandLength, int(wireLength)))
    print '____________________'

def waitForCompletion(ser):
    """
    Waits for confirmation from the arduino that the task is complete,
    then clears the buffer

    ser: pyserial object
    """
    # while ser.inWaiting()==0:
    #     pass
    for byte in ser.readline():
        print byte
    # ser.flushInput()
    
if __name__ == '__main__':
    import serial
    ser=serial.Serial('/dev/ttyACM0') #connect to serial port
    sendWires(ser, [('r',100)])
