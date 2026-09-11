CC = m68k-elf-gcc
LD = m68k-elf-ld
OBJCOPY = m68k-elf-objcopy
VASM = vasmm68k_mot 

CFLAGS = -Wall -O0 -I ./includes -I./include -I../include/sys -ffreestanding -nostdlib
CFLAGS += -DCHIP_$(CHIP_TYPE) -DROM_SIZE_BYTES=$(ROM_SIZE)
CFLAGS += -fomit-frame-pointer

VASMFLAGS = -Felf -m68000 -quiet -I ./includes -I../../include

LDFLAGS = -T ./kernel.ld -L../../lib -nostdlib -Map=kernel.map
LDLIBS = 

# 1. ADICIONADO: Lista de arquivos em Assembly (.S)
ASMS =  crt0.S ctx_switch_68000.S ./exceptions/exceptions_asm.S

VASMS =

# Lista de arquivos C (CORRIGIDO: não tinha um espaço)
SRCS =  kernel.c kmalloc.c scheduler.c ./syscall/trap_table.c task_create.c ./mm/slots.c kfunctions.c  \
./exceptions/exceptions.c \
./lib/klibc/inttostr.c \
./lib/klibc/klibc.c \
./lib/klibc/strtol.c	\
./lib/klibc/__divdi3.c  \
./lib/klibc/__divsi3.c  \
./lib/klibc/__moddi3.c  \
./lib/klibc/__modsi3.c  \
./lib/klibc/__muldi3.c  \
./lib/klibc/__mulsi3.c  \
./lib/klibc/__udivdi3.c \
./lib/klibc/__udivsi3.c \
./lib/klibc/__umoddi3.c \
./lib/klibc/__umodsi3.c \
./lib/klibc/__umuldi3.c \
./lib/klibc/__umulsi3.c \
./lib/stdio/kprintf.c \
./lib/stdio/ksprintf.c \
./lib/string/memchr.c ./lib/string/memcpy.c ./lib/string/memmove.c ./lib/string/strcat.c ./lib/string/strcmp.c \
./lib/string/strcpy.c ./lib/string/strdup.c ./lib/string/strlen.c ./lib/string/strncmp.c ./lib/string/strndup.c ./lib/string/strrchr.c \
./lib/string/strstr.c ./lib/string/memccpy.c ./lib/string/memcmp.c ./lib/string/memmem.c ./lib/string/memset.c ./lib/string/strchr.c \
./lib/string/strcspn.c ./lib/string/strerror.c ./lib/string/strncat.c ./lib/string/strncpy.c ./lib/string/strnlen.c ./lib/string/strspn.c ./lib/string/strtok.c \
./vfs/vfs.c  ./vfs/vfs_drivers.c  ./vfs/vfs_fat.c  ./vfs/vfs_null.c  ./vfs/vfs_proc.c  ./vfs/vfs_serial.c  ./vfs/vfs_tty.c  ./vfs/vfs_zero.c \
./drivers/duart/duart.c \
./drivers/ide/ata.c \
./drivers/ide/diskio.c \
./drivers/fatfs/ff.c \
./drivers/fatfs/ffsystem.c \
./drivers/fatfs/ffunicode.c


# 2. ALTERADO: Agora a lista de objetos junta os .o vindos do .c e do .S
OBJS =  $(ASMS:.S=.o) $(SRCS:.c=.o)$(VASMS:.asm=.o)

# Tools
TARGET := kernel
TARGET_OBJ := $(TARGET).o
TARGET_ELF := $(TARGET).elf
TARGET_BIN := $(TARGET).sys
TARGET_LOW := $(TARGET)_low.bin
TARGET_HIGH := $(TARGET)_high.bin

.PHONY: all clean split fill programL programH

all: $(TARGET_BIN)  # ← CORRIGIDO: usa TARGET_BIN

# Regra para compilar os arquivos .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 3. ADICIONADO: Regra explícita para compilar arquivos Assembly (.S)
%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(VASM) $(VASMFLAGS) $< -o $@

# 4. CORRIGIDO: Gera kernel.elf a partir dos objetos
$(TARGET_ELF): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@
	@echo "Linked: $@"
	@echo "╔══════════════════════════╗"
	@echo "║  COMPILACAO TERMINADA!   ║"
	@echo "╚══════════════════════════╝"

# 5. CORRIGIDO: Gera kernel.bin a partir de kernel.elf (NÃO de bios68.elf!)
$(TARGET_BIN): $(TARGET_ELF)
	$(OBJCOPY) -O binary $< $@
	@echo "Binary: $@"

clean:
	rm -f *.bin $(OBJS) *.map $(TARGET_LOW) $(TARGET_HIGH) $(TARGET_ELF)
	@echo "Clean complete"