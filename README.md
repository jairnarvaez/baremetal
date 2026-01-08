# nRF52833 Bare-metal

This project demonstrates bare-metal programming for the nRF52833 microcontroller. The current iteration focuses on basic hardware interaction, specifically controlling an on-board LED. This serves as a foundation for incrementally adding more complex functionalities.

## Usage

To compile this project, ensure you have the ARM GCC toolchain installed. Navigate to the project root directory and execute the following command:

```bash
make
```

This will generate the necessary `.o` files, link them, and produce the `main.hex` firmware image suitable for flashing onto the nRF52833 device.

Other useful `make` commands:

- `make clean`: Removes all compiled object files (`.o`), executable files (`.elf`), binary files (`.bin`), map files (`.map`), and firmware images (`.hex`). Use this to clean up the build directory.

- `make upload`: This command is used to flash the compiled `main.hex` firmware onto a connected nRF52833 device (specifically configured for a Microbit in the current Makefile). This command requires `sudo` privileges to mount and unmount the device.

  ```bash
  make upload
  ```
