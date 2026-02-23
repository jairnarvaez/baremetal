import serial
import serial.tools.list_ports
import sys
import time

DEFAULT_PORT = "/dev/ttyACM0"
DEFAULT_BAUD = 115200


def find_microbit():
    for port in serial.tools.list_ports.comports():
        if "microbit" in port.description.lower() or "mbed" in port.description.lower():
            return port.device
    return None


def connect(port, baud):
    try:
        ser = serial.Serial(port, baud, timeout=2.0)
        time.sleep(0.5)
        ser.reset_input_buffer()
        return ser
    except serial.SerialException as e:
        print(f"[ERR] Could not connect to {port}: {e}")
        sys.exit(1)
