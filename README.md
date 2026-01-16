# Microbit V2 Firmware

This project is part of my personal journey into embedded systems development. It contains the source code for a bare-metal firmware for the BBC Microbit V2 (nRF52833), focused on learning low-level hardware abstraction and peripheral management.

## Features

This firmware provides the following functionalities:

- **UART communication:** for sending and receiving data to a host computer.
- **LED matrix animations:** a set of animations for the 5x5 LED matrix.
- **GPIO drivers:** for controlling the GPIO pins.
- **Delay functions:** for creating delays in the code.

## Usage

To compile this project, ensure you have the ARM GCC toolchain installed. Navigate to the project root directory and execute the following command:

```bash
make
```

This will generate the necessary `.o` files, link them, and produce the `main.hex` firmware image suitable for flashing onto the Microbit V2.

Other useful `make` commands:

- `make clean`: Removes all compiled object files (`.o`), executable files (`.elf`), binary files (`.bin`), map files (`.map`), and firmware images (`.hex`). Use this to clean up the build directory.

- `make upload`: This command is used to flash the compiled `main.hex` firmware onto a connected Microbit V2. This command requires `sudo` privileges to mount and unmount the device.

  ```bash
  make upload
  ```
