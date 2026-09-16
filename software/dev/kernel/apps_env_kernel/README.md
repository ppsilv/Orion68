## Todas as aplicações devem ser compiladas com loader

'''
OUTPUT_FORMAT("elf32-m68k")
OUTPUT_ARCH(m68k)
ENTRY(main)

MEMORY
{
    SLOT (rwx) : ORIGIN = 0x00000000, LENGTH = 0x00040000
}

PHDRS
{
    text PT_LOAD FLAGS(5);   /* R+X -- so' texto/rodata */
    data PT_LOAD FLAGS(6);   /* R+W -- data/bss */
}

SECTIONS
{
    . = ORIGIN(SLOT);
    .text : {
        *(.text*)
        *(.rodata*)
    } > SLOT :text

    . = ALIGN(4);
    .data : {
        _data_start = .;
        *(.data*)
    } > SLOT :data

    .bss : {
        *(.bss*)
        *(COMMON)
        _bss_end = .;
    } > SLOT :data

    _end = .;
    /DISCARD/ : { *(.comment) *(.note*) *(.eh_frame*) }
}
'''
## e com as flags CFLAGS += -fPIC -msep-data
# use o makefile abaixo
#
CC = m68k-elf-gcc
LD = m68k-elf-ld
OBJCOPY = m68k-elf-objcopy

CFLAGS = -m68000 -Wall -Os -Wa,-adhln=duart.lst -I../../include -ffreestanding -nostdlib -D __68000__ -D ALIGN_MEMORY
CFLAGS += -fPIC -msep-data
LDFLAGS = -T ./linker_pic.ld -L../../lib -nostdlib -L../../lib --emit-relocs
LDLIBS = -lc68k_pic

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
ELFS = $(SRCS:.c=.elf)
BINS = $(SRCS:.c=.bin)

APP = duart

.PHONY: all clean

#all: $(BINS)
all: $(APP).bin
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra específica para cada arquivo para evitar problemas com $<
$(APP).elf: $(OBJS) ../../libs/libc/obj/libc/src/crt0.o
	$(LD) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)
	@echo "Linked: $@"



%.bin: $(APP).elf
	$(OBJCOPY) -O binary $< $@
	@echo "Binary: $@"

clean:
	rm -f $(OBJS).o $(ELFS) $(BINS).bin $(APP).elf
	@echo "Clean complete"
#
