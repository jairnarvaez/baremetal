FLASH_SIZE = 524288
RAM_SIZE = 131072

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CPU = -mcpu=cortex-m4 -mthumb
CFLAGS = -O -g -Wall -ffreestanding

BUILD_DIR = build

SRC_DIRS = src drivers/src examples
SRCS = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
INCLUDE_DIRS = src drivers/inc examples
INCLUDES = $(addprefix -I, $(INCLUDE_DIRS))

OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/main.hex

$(BUILD_DIR)/main.hex: $(BUILD_DIR)/main.elf
	$(OBJCOPY) -O ihex $< $@
	@./scripts/memory_report.sh $< $(FLASH_SIZE) $(RAM_SIZE)

$(BUILD_DIR)/main.elf: $(OBJS)
	$(CC) $(CPU) $(CFLAGS) -T linker.ld $^ -nostdlib -lgcc -o $@ -Wl,-Map,$(BUILD_DIR)/main.map
	$(SIZE) $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPU) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

upload: $(BUILD_DIR)/main.hex
	sudo mount /dev/disk/by-label/MICROBIT /mnt/microbit
	sudo cp $< /mnt/microbit/
	sync
	sudo umount /mnt/microbit
