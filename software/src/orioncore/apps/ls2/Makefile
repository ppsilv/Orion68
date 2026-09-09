CC = m68k-elf-gcc
LD = m68k-elf-ld
OBJCOPY = m68k-elf-objcopy

ROOT_DIR=../../../

APP = ls2

CFLAGS = -Wall -O0 -Wa,-adhln=$(APP).lst -I$(ROOT_DIR)/include -I../include -ffreestanding -nostdlib -D __68000__ -D ALIGN_MEMORY
LDFLAGS = -T ../linker.ld -L../../libc/lib -nostdlib
LDLIBS = -lc68bios
SRCS = $(APP).c \
../apps_libs/fatfs/ff.c \
../apps_libs/fatfs/ffsystem.c \
../apps_libs/fatfs/ffunicode.c \
../apps_libs/ide/ata.c \
../apps_libs/picoVga/drv_picoVga.c \
../apps_libs/ide/diskio.c

OBJS = $(SRCS:.c=.o)
ELFS = $(SRCS:.c=.elf)
BINS = $(SRCS:.c=.bin)

.PHONY: all clean

all: $(APP).bin
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra específica para cada arquivo para evitar problemas com $<
$(APP).elf: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)
	@echo "Linked: $@"

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
	@echo "Binary: $@"

clean:
	rm -f $(OBJS) $(ELFS) $(BINS) *.lst
	@echo "Clean complete"
