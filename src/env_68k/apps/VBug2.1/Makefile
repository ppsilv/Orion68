CC = m68k-elf-gcc
LD = m68k-elf-ld
OBJCOPY = m68k-elf-objcopy

CFLAGS = -Wall -O0 -I ./includes -I../../include -ffreestanding -nostdlib
LDFLAGS = -T ./vbug2-1.ld -L../../lib -nostdlib  -Map=vbug2-1.map

#SRCS = $(wildcard *.c)
SRCS = vbug2-1.c vectors.c console.c \
./dump/mem_dump.c \
./kbd/drv_kbd.c \
./picoVga/drv_picoVga.c \
./xmodem/xmodem.c \
./uarts/drv_uart0.c \
./ide/ata.c \
timers.c interrupt.c

OBJS = $(SRCS:.c=.o)
#ELFS = $(SRCS:.c=.elf)
#BINS = $(SRCS:.c=.bin)

#Tools
SPLITTER := python3 ./tools/split68k.py
FILLER := python3 ./tools/fill.py
PROGRAMMER := minipro
CHECKSUMER := python3 ./tools/checksum_put.py
#CHIP_TYPE := SST39SF010 #28C64A  # Altere para o seu chip Flash
CHIP_TYPE := AT29C512
#Rom sizes
FLASH_SIZE := 65536  # 128KB (tamanho da 39SF010)
ROM_SIZE := 65536     # 128KB usados

TARGET := vbug2-1
TARGET_OBJ := $(TARGET).o
TARGET_ELF := $(TARGET).elf
TARGET_BIN := $(TARGET).bin
TARGET_LOW := $(TARGET)_low.bin
TARGET_HIGH := $(TARGET)_high.bin
 

.PHONY: all clean

all: vbug2-1.bin

%.o: %.c
	@./tools/build_counter.sh
	$(CC) $(CFLAGS) -c $< -o $@

# Regra específica para cada arquivo para evitar problemas com $<
#vbug2-1.elf:  $(OBJS) ../../obj/crt0.o ../../obj/vbu2-1_startup.o
#	$(LD) $(LDFLAGS) ../../obj/startup.o   vbug2-1.o -lmc68000 -o $@
#	@echo "Linked: $@"

vbug2-1.elf: ./tools/build_counter.sh $(OBJS)
	$(LD) $(LDFLAGS) ../../obj/vbu2-1_startup.o $(OBJS) -lmc68000 -o $@
	@echo "Linked: $@"
	@echo "╔══════════════════════════╗"
	@echo "║  COMPILACAO TERMINADA!   ║"
	@echo "╚══════════════════════════╝"

vbug2-1.bin: vbug2-1.elf
	$(OBJCOPY) -O binary $< $@
	@echo "Binary: $@"

clean:
	rm -f *.bin $(OBJS) $(ELFS) $(BINS) *.map
	@echo "Clean complete"

split: vbug2-1.elf
	@echo "Dividindo $(TARGET_BIN) em words 16-bit..."
	@$(SPLITTER) $(TARGET_BIN)
	@$(FILLER) $(TARGET_LOW) $(FLASH_SIZE)
	@$(FILLER) $(TARGET_HIGH) $(FLASH_SIZE)
	@echo "Arquivos gerados: $(TARGET_LOW) e $(TARGET_HIGH)"

fill: split
	@echo "Preenchendo arquivos para $(FLASH_SIZE) bytes..."
	#@$(FILLER) $(TARGET_LOW) $(FLASH_SIZE)
	#@$(FILLER) $(TARGET_HIGH) $(FLASH_SIZE)
	@echo "Preenchimento completo."

programL:
	@echo "Gravando parte LOW na Flash..."
	@$(PROGRAMMER) -p $(CHIP_TYPE) -w $(TARGET_LOW) -s
	@echo "Gravação concluída LOW! BOA SORTE"

programH: split
	@echo "Gravando parte HIGH na Flash..."
	@$(PROGRAMMER) -p $(CHIP_TYPE) -w $(TARGET_HIGH) -s
	@echo "Gravação concluída HIGH! GRAVE A MEMORIA LOW"