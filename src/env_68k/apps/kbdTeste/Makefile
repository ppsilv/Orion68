CC = m68k-elf-gcc
LD = m68k-elf-ld
OBJCOPY = m68k-elf-objcopy

CFLAGS = -Wall -Os -I../../include -ffreestanding -nostdlib
#CFLAGS = -Wall -Os -I../include -ffreestanding -nostdlib -Dmain=main_with_exit
LDFLAGS = -T ../../linker.ld -L../../lib -nostdlib

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
ELFS = $(SRCS:.c=.elf)
BINS = $(SRCS:.c=.bin)

.PHONY: all clean

all: kbdTeste.bin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra específica para cada arquivo para evitar problemas com $<
kbdTeste.elf: kbdTeste.o keyboard.o ../../obj/crt0.o ../../obj/startup.o
	$(LD) $(LDFLAGS) ../../obj/crt0.o ../../obj/startup.o kbdTeste.o keyboard.o -lmc68000 -o $@
	@echo "Linked: $@"

kbdTeste.bin: kbdTeste.elf
	$(OBJCOPY) -O binary $< $@
	@echo "Binary: $@"

clean:
	rm -f $(OBJS) $(ELFS) $(BINS)
	@echo "Clean complete"
