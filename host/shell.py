import argparse
import readline
import sys

import protocol
import transport

HISTORY_FILE = ".shell_history"

HELP_TEXT = """
Available commands:

  GPIO O <port> <pin>               configure pin as output
  GPIO I <port> <pin>               configure pin as input
  GPIO W <port> <pin> <0|1>         write pin
  GPIO R <port> <pin>               read pin

  DISPLAY I                         initialize display
  DISPLAY W r0 r1 r2 r3 r4          write full pattern (5 rows in binary)
  DISPLAY P <x> <y> <0|1>           write pixel
  DISPLAY C                         clear display

  CLEAR                             clear terminal

  RESET                             reset microbit

  help                              show this help
  exit / quit                       close the shell
"""

def handle_local(cmd):
    if cmd == "help":
        print(HELP_TEXT)
        return True
    if cmd in ("exit", "quit"):
        print("bye.")
        sys.exit(0)
    return False


def setup_history():
    try:
        readline.read_history_file(HISTORY_FILE)
    except FileNotFoundError:
        pass
    readline.set_history_length(500)

def save_history():
    try:
        readline.write_history_file(HISTORY_FILE)
    except Exception:
        pass

def run(ser):
    setup_history()
    print(f"\033[36mmicro:bit shell\033[0m — connected on {ser.port} @ {ser.baudrate}")
    print("type 'help' to see available commands\n")

    while True:
        try:
            cmd = input("\033[33m$\033[0m ").strip()
        except (EOFError, KeyboardInterrupt):
            print("\nbye.")
            save_history()
            break

        if not cmd:
            continue

        if handle_local(cmd.lower()):
            continue

        response = protocol.send(ser, cmd)
        print(protocol.decode(response))

    save_history()

def main():
    parser = argparse.ArgumentParser(description="micro:bit v2 host shell")
    parser.add_argument("--port", default=None,                   help="serial port (e.g. /dev/ttyACM0)")
    parser.add_argument("--baud", default=transport.DEFAULT_BAUD, help="baudrate (default: 115200)", type=int)
    args = parser.parse_args()

    port = args.port or transport.find_microbit() or transport.DEFAULT_PORT
    ser  = transport.connect(port, args.baud)

    run(ser)
    ser.close()

if __name__ == "__main__":
    main()
