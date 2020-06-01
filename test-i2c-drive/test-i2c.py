import smbus
import time
bus = smbus.SMBus(1)

address = 0x04

def writeNumber(value):
  bus.write_byte(address, int(value))
  # bus.write_byte_data(address, 0, value)
  return -1

def readNumber():
  number = bus.read_byte(address)
  # number = bus.read_byte_data(address, 1)
  return number

while True:
#  var = input("Enter 1 - 9: ")
  var = input()
  if not var:
    continue
  writeNumber(var)
#  print("RPI: Hi Arduino, I sent you ", var)
  time.sleep(0.1)
#  number = readNumber()
#  print("Arduino: Hey RPI, I received a digit ", number)
