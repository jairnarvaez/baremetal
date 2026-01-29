# Microbit V2 Firmware
This project is part of my personal journey into embedded systems development. It contains the source code for a bare-metal firmware for the BBC Microbit V2 (nRF52833), focused on learning low-level hardware abstraction and peripheral management.

## Features
This firmware provides the following functionalities:
- **UART communication:** supports both polling and interrupt-driven transmission and reception.
- **GPIO drivers:** for controlling the GPIO pins.
- **Delay functions:** for creating delays in the code.
- **LED matrix animations:** a set of animations for the 5x5 LED matrix.

## Prerequisites
### Installing PyOCD on Arch Linux
PyOCD is required for flashing and debugging the Microbit V2. Install it using `yay`:
```bash
# Install PyOCD from AUR
yay -S python-pyocd

# Verify installation
pyocd --version

# Install udev rules for USB access (no sudo needed for flashing)
sudo sh -c 'curl -o /etc/udev/rules.d/99-microbit.rules https://raw.githubusercontent.com/mbedmicro/pyOCD/master/udev/99-pyocd.rules'

# Reload udev rules
sudo udevadm control --reload-rules
sudo udevadm trigger

# Add your user to the plugdev group (logout/login required)
sudo usermod -a -G plugdev $USER
```

## Usage
To compile this project, ensure you have the ARM GCC toolchain installed. Navigate to the project root directory and execute the following command:
```bash
make
```
This will generate the necessary `.o` files, link them, and produce the `main.hex` firmware image suitable for flashing onto the Microbit V2.

Other useful `make` commands:
- `make clean`: Removes all compiled object files (`.o`), executable files (`.elf`), binary files (`.bin`), map files (`.map`), and firmware images (`.hex`). Use this to clean up the build directory.
- `make upload`: This command is used to flash the compiled `main.hex` firmware onto a connected Microbit V2.
```bash
  make upload
```