CC = m68k-elf-gcc
LD = m68k-elf-ld
OBJCOPY = m68k-elf-objcopy
VASM = vasmm68k_mot 

CFLAGS = -Wall -Os -I ./includes -I../include -ffreestanding -nostdlib
CFLAGS += -DCHIP_$(CHIP_TYPE) -DROM_SIZE_BYTES=$(ROM_SIZE)
CFLAGS += -fomit-frame-pointer

VASMFLAGS = -Felf -m68000 -quiet -I ./includes -I../include

LDFLAGS = -T ./orioncore.ld -L./libc/lib -nostdlib  -Map=orioncore.map
LDLIBS = -lc68bios
# 1. ADICIONADO: Lista de arquivos em Assembly (.S)
ASMS = ./bootstrap/orion68_startup.S  ./bootstrap/vectors_asm.S \
./drivers_raw/duart/duartasm.S 


VASMS =

# Lista de arquivos C
SRCS = orioncore.c ./corelibs/printf.c ./bootstrap/jumptable.c \
./drivers_raw/kbd/ringbuffer.c \
./drivers_raw/duart/duart.c \
./drivers_raw/picoVga/drv_picoVga.c \
./drivers_raw/ide/ata.c \
./drivers_raw/ide/diskio.c \
./drivers_raw/ethernet/dhcp_client.c \
./drivers_raw/picow.c \
../libs/libc/libfileio/src/fatfs/ff.c \
../libs/libc/libfileio/src/fatfs/ffsystem.c \
../libs/libc/libfileio/src/fatfs/ffunicode.c \
./shell/elf_loader.c ./shell/shell.c ./shell/commands.c  ./shell/printerro.c ./shell/shell_history.c \
./receiver/receiver.c ./orionburner/orionbur.c



#./sys/ethernet/dhcp_client.c \
#./tst_fat/tstfat.c
#./picoVga/drv_picoVga.c \

# 2. ALTERADO: Agora a lista de objetos junta os .o vindos do .c e do .S
OBJS = $(SRCS:.c=.o) $(ASMS:.S=.o) $(VASMS:.asm=.o)

#Tools
SPLITTER := python3 ./tools/split68k.py
FILLER := python3 ./tools/fill.py
PROGRAMMER := minipro
CHECKSUMER := python3 ./tools/checksum_put.py

CHIP ?= AT28C256
ifeq ($(CHIP), AT29C512)
    CHIP_TYPE  := AT29C512
    FLASH_SIZE := 65536  # 64KB
    ROM_SIZE   := 65536  # 64KB
else ifeq ($(CHIP), AT28C256)
    CHIP_TYPE  := AT28C256
    FLASH_SIZE := 32768  # 32KB
    ROM_SIZE   := 32768  # 32KB
else
    $(error Chip desconhecido: $(CHIP). Escolha AT29C512 ou AT28C256)
endif

TARGET := orioncor
TARGET_OBJ := $(TARGET).o
TARGET_ELF := $(TARGET).elf
TARGET_BIN := $(TARGET).bin
TARGET_LOW := $(TARGET)_low.bin
TARGET_HIGH := $(TARGET)_high.bin

.PHONY: all clean split fill programL programH

all: $(TARGET).bin

# Regra para compilar os arquivos .c
%.o: %.c
	@./tools/build_counter.sh
	$(CC) $(CFLAGS) -c $< -o $@

# 3. ADICIONADO: Regra explícita para compilar arquivos Assembly (.S)
%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(VASM) $(VASMFLAGS) $< -o $@

$(TARGET).elf: ./tools/build_counter.sh $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@
	@echo "Linked: $@"
	@echo "╔══════════════════════════╗"
	@echo "║  COMPILACAO TERMINADA!   ║"
	@echo "╚══════════════════════════╝"

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@
	@echo "Binary: $@"

cplib:
	cp fatlib/lib/libfatfs.a ../../lib/.

clean:
	rm -f *.bin $(OBJS) *.map $(TARGET_LOW) $(TARGET_HIGH) $(TARGET_ELF)
	@echo "Clean complete"

split: orioncor.elf
	@echo "Dividindo $(TARGET_BIN) em words 16-bit..."
	@$(SPLITTER) $(TARGET_BIN)
	@$(FILLER) $(TARGET_LOW) $(FLASH_SIZE)
	@$(FILLER) $(TARGET_HIGH) $(FLASH_SIZE)
	@echo "Arquivos gerados: $(TARGET_LOW) e $(TARGET_HIGH)"

fill: split
	@echo "Preenchendo arquivos para $(FLASH_SIZE) bytes..."
	@echo "Preenchimento completo."

programL:
	@echo "Gravando parte LOW na Flash..."
	$(PROGRAMMER) -p $(CHIP_TYPE) -w $(TARGET_LOW) -s -u
	@echo "Gravação concluída LOW! BOA SORTE"

programH: split
	@echo "Gravando parte HIGH na Flash..."
	@echo $(CHIP_TYPE)
	$(PROGRAMMER) -p $(CHIP_TYPE) -w $(TARGET_HIGH) -s -u
	@echo "Gravação concluída HIGH! GRAVE A MEMORIA LOW"
