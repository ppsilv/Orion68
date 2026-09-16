CC = m68k-elf-gcc
LD = m68k-elf-ld
OBJCOPY = m68k-elf-objcopy

ROOT= ..

CFLAGS = -Wall -Os -Wa,-adhln=shell.lst -I$(ROOT)/include -ffreestanding -nostdlib -D __68000__ -D ALIGN_MEMORY
LDFLAGS = -T ./linker.ld -L$(ROOT)/lib -nostdlib -L$(ROOT)/lib
LDLIBS = -lc68k

ASMS =

VASMS =

SRCS = elf_loader.c decodecmd.c commands.c  printerro.c shell_history.c 


# 2. ALTERADO: Agora a lista de objetos junta os .o vindos do .c e do .S
OBJS = $(SRCS:.c=.o) $(ASMS:.S=.o) $(VASMS:.asm=.o)

TARGET := shellv0
TARGET_OBJ := $(TARGET).o
TARGET_ELF := $(TARGET).elf
TARGET_BIN := $(TARGET).bin

.PHONY: all clean

all: $(TARGET).bin

# Regra para compilar os arquivos .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 3. ADICIONADO: Regra explícita para compilar arquivos Assembly (.S)
%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(VASM) $(VASMFLAGS) $< -o $@

# Regra específica para cada arquivo para evitar problemas com $<
$(TARGET).elf: $(OBJS) $(ROOT)/libs/libc/obj/crt0.o
	$(LD) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)
	@echo "Linked: $@"
	@echo "╔══════════════════════════╗"
	@echo "║  COMPILACAO TERMINADA!   ║"
	@echo "╚══════════════════════════╝"

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@
	@echo "Binary: $@"

clean:
	rm -f *.bin $(OBJS) *.map $(TARGET_LOW) $(TARGET_HIGH) $(TARGET_ELF)
	@echo "Clean complete"

