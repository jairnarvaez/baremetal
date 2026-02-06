# Microbit V2 Firmware
This project is part of my personal journey into embedded systems development. It contains the source code for a bare-metal firmware for the BBC Microbit V2 (nRF52833), focused on learning low-level hardware abstraction and peripheral management.

## Features
This firmware provides the following functionalities:
- **Timer API**: Hardware timer management supporting multiple channels (CC).
   - Simplified periodic task execution (Run/Stop/Init).
   - Interrupt-driven events for precise timing.
   - Auto-clear shortcuts for consistent frequency generation.
- **UART API:** Custom driver for serial communication.
   - Supports both polling and interrupt-driven transmission/reception.
- **GPIO API:** Direct register-level control for GPIO pins.
- **GPIOTE API**: Hardware-level bridge between GPIO pins and the event system.
   - Supports pin state change detection (rising, falling, or toggle) via hardware events.
   - Enables low-power wake-up and direct task triggering without CPU intervention.
- **I2C API:** Polling-based master driver for serial communication.
   - Supports read/write primitives with repeated start and hardware error detection.
- **LSM303AGR Driver:** Hardware abstraction for the 3-axis accelerometer and thermal sensor..
   - Features bit-field register mapping and LUT-based sensitivity scaling up to ±16g.
- **Delay functions:** for creating delays in the code.

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
