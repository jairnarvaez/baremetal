#!/bin/bash

if [ $# -ne 3 ]; then
    echo "Uso: $0 <archivo.elf> <flash_size> <ram_size>"
    echo "Ejemplo: $0 build/main.elf 524288 131072"
    exit 1
fi

ELF_FILE=$1
FLASH_SIZE=$2
RAM_SIZE=$3

if [ ! -f "$ELF_FILE" ]; then
    echo "Error: El archivo '$ELF_FILE' no existe"
    exit 1
fi

SIZE_OUTPUT=$(arm-none-eabi-size "$ELF_FILE" | tail -n 1)

TEXT=$(echo "$SIZE_OUTPUT" | awk '{print $1}')
DATA=$(echo "$SIZE_OUTPUT" | awk '{print $2}')
BSS=$(echo "$SIZE_OUTPUT" | awk '{print $3}')

FLASH=$((TEXT + DATA))
RAM=$((DATA + BSS))

FLASH_PCT=$(awk "BEGIN {printf \"%.2f\", ($FLASH / $FLASH_SIZE) * 100}")
RAM_PCT=$(awk "BEGIN {printf \"%.2f\", ($RAM / $RAM_SIZE) * 100}")

echo ""
echo "========================================="
echo "           Uso de Memoria"
echo "========================================="
printf "Flash: %7d / %7d bytes (%5s%%)\n" $FLASH $FLASH_SIZE $FLASH_PCT
printf "RAM:   %7d / %7d bytes (%5s%%)\n" $RAM $RAM_SIZE $RAM_PCT

if [ $FLASH -gt $FLASH_SIZE ]; then
    echo " ERROR: Flash overflow!"
fi

if [ $RAM -gt $RAM_SIZE ]; then
    echo " ERROR: RAM overflow!"
fi

echo "========================================="
echo ""