import time
from pyb import UART

uart = UART(3, 19200)

while(True):
    uart.write("Hello World!\r")
    time.sleep(1000)
