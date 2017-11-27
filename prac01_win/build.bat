set cc=arm-none-eabi
set target=prac01

%cc%-as start.s -o start.o
%cc%-gcc -Wall -O0 -nostdlib -nostartfiles -ffreestanding -c %target%.c -o %target%.o
%cc%-ld start.o %target%.o -T memmap -o %target%.elf
%cc%-objcopy %target%.elf -O binary kernel.img