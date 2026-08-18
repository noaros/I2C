arm-none-eabi-gcc -Tlink.ld -nostdlib 0.c -mcpu=cortex-m4
arm-none-eabi-objcopy -O binary a.out 0.bin
