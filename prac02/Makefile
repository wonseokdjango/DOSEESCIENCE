
ARMGNU ?= aarch64-elf

AOPS = --warn --fatal-warnings
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : kernel7.img

all : gcc

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.srec
	rm -f *.elf
	rm -f *.list
	rm -f *.img

start.o : start.s
	$(ARMGNU)-as $(AOPS) start.s -o start.o

prac02.o : prac02.c
	$(ARMGNU)-gcc $(COPS) -c prac02.c -o prac02.o

kernel7.img : memmap start.o prac02.o 
	$(ARMGNU)-ld start.o prac02.o -T memmap -o prac02.elf
	$(ARMGNU)-objdump -D prac02.elf > prac02.list
	$(ARMGNU)-objcopy --srec-forceS3 prac02.elf -O srec prac02.srec
	$(ARMGNU)-objcopy prac02.elf -O binary kernel7.img


