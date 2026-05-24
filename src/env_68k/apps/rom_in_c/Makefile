CC = m68k-elf-gcc
LD = m68k-elf-ld
OBJCOPY = m68k-elf-objcopy

CFLAGS = -Wall -Os -I../../include -ffreestanding -nostdlib
LDFLAGS = -T ./linker.ld -L../../lib -nostdlib -Map=rom_ext.map

# 1. Liste aqui explicitamente os arquivos de suporte do seu binário mestre
SUPPORTE_SRCS = xmodem.c crc16.c keyboard.c
SUPPORT_OBJS = $(SUPPORTE_SRCS:.c=.o)

PROJ = rom_ext

.PHONY: all clean

# O alvo principal agora gera apenas os binários finais que importam
all: $(PROJ).bin

# Regra genérica para compilar qualquer arquivo .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 2. Modificado: Adicionado $(SUPPORT_OBJS) na linkagem do projeto principal
$(PROJ).elf: ../../obj/crt0.o ../../obj/startup.o $(SUPPORT_OBJS)
	$(LD) $(LDFLAGS) ../../obj/crt0.o ../../obj/startup.o $(SUPPORT_OBJS) -lmc68000 -o $@
	@echo "Linked: $@"

# Regra genérica para transformar .elf em .bin
%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
	@echo "Binary: $@"
	python3 gera_includes.py rom_ext.map keyboard.o xmodem.o

clean:
	rm -f *.o *.elf *.bin *.map *.inc pds317_rom.h
	@echo "Clean complete"