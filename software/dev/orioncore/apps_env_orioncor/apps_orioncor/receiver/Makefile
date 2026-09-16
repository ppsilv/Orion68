CC = m68k-elf-gcc
LD = m68k-elf-ld
OBJCOPY = m68k-elf-objcopy


ORIONCOR_DIR=../../..
SYS_ROOT=../../../../..

APP = recv

CFLAGS = -Wall -O0 -Wa,-adhln=$(APP).lst -I$(SYS_ROOT)/include -I$(ORIONCOR_DIR)/includes -I  -ffreestanding -nostdlib -D __68000__ -D ALIGN_MEMORY
LDFLAGS = -T ../linker.ld -L$(ORIONCOR_DIR)/libc/lib -nostdlib
LDLIBS = -lc68bios
SRCS = recv.c \
../../apps_libs/fatfs/ff.c \
../../apps_libs/fatfs/ffsystem.c \
../../apps_libs/fatfs/ffunicode.c \
../../apps_libs/ide/ata.c \
../../apps_libs/picoVga/drv_picoVga.c \
../../apps_libs/ide/diskio.c

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
