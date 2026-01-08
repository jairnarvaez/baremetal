all: main.hex

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
AR = arm-none-eabi-ar
SIZE = arm-none-eabi-size
OBJCOPY = arm-none-eabi-objcopy

CPU = -mcpu=cortex-m4 -mthumb
CFLAGS = -O -g -Wall -ffreestanding
STARTUP = startup.o
LIBS = -lgcc

%.o: %.c
	$(CC) $(CPU) $(CFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.s
	$(AS) $(CPU) $< -o $@

%.elf: %.o $(DEPS) $(STARTUP)
	$(CC) $(CPU) $(CFLAGS) -T linker.ld \
		$^ -nostdlib $(LIBS) -o $@ -Wl,-Map,$*.map
	$(SIZE) $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

clean:
	rm -f *.hex *.elf *.bin *.map *.o

MICROBIT_DEV = /dev/disk/by-label/MICROBIT
MOUNT_POINT  = /mnt/microbit

upload:
	sudo mount $(MICROBIT_DEV) $(MOUNT_POINT)
	sudo cp main.hex $(MOUNT_POINT)/
	sync
	sudo umount $(MOUNT_POINT)