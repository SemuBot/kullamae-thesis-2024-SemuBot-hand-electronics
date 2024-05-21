import serial
import serial.tools.list_ports
import struct 
from enum import IntEnum
import keyboard
import ser_nucleo
import time
steps = 5

def main(ser: serial.Serial):
  #print("Press any key to continue...")wwwawaaaaaaaaaaa
  vals = ser_nucleo.MotorValues()
  if keyboard.is_pressed(hotkey="W"):
    print("W pressed")
    vals.m1_speed = -6
    vals.m1_steps = steps
    ser_nucleo.ser_write(ser, packet=ser_nucleo.ser_make_motor_packet(vals))
    time.sleep(0.1)
  if keyboard.is_pressed(hotkey="A"):
    print("A pressed")
    vals.m2_speed = 8
    vals.m2_steps = 18
    ser_nucleo.ser_write(ser, packet=ser_nucleo.ser_make_motor_packet(vals))
    time.sleep(0.1)
  if keyboard.is_pressed(hotkey="S"):
    print("S pressed")
    vals.m2_speed = -8
    vals.m2_steps = 18
    ser_nucleo.ser_write(ser, packet=ser_nucleo.ser_make_motor_packet(vals))
    time.sleep(0.1)
  if keyboard.is_pressed(hotkey="D"):
    print("D pressed")
    time.sleep(0.1)
  if keyboard.is_pressed(hotkey="J"):
    print("J pressed")
    vals.m4_speed = -6
    vals.m4_steps = steps
    ser_nucleo.ser_write(ser, packet=ser_nucleo.ser_make_motor_packet(vals))
    time.sleep(0.1)
if __name__ == "__main__":
  ser = ser_nucleo.ser_init(9600, 5)
  try:
    while True:
      main(ser)
  except KeyboardInterrupt:
    ser_nucleo.ser_close(ser)




