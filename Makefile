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
	@echo ""
	@echo "============ Uso de Memoria ============"
	@$(SIZE) $< | awk 'NR==2 {text=$$1; data=$$2; bss=$$3; \
		flash=text+data; ram=data+bss; \
		flash_pct=(flash/$(FLASH_SIZE))*100; \
		ram_pct=(ram/$(RAM_SIZE))*100; \
		printf "Flash: %d bytes / %d bytes (%.2f%%)\n", flash, $(FLASH_SIZE), flash_pct; \
		printf "RAM:   %d bytes / %d bytes (%.2f%%)\n", ram, $(RAM_SIZE), ram_pct; \
		if (flash > $(FLASH_SIZE)) printf " ERROR: Flash overflow!\n"; \
		if (ram > $(RAM_SIZE)) printf " ERROR: RAM overflow!\n"}'
	@echo "========================================"


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
