CC = m68k-elf-gcc
LD = m68k-elf-ld
OBJCOPY = m68k-elf-objcopy

CFLAGS = -Wall -Os -Wa,-adhln=shell.lst -I../../../include -ffreestanding -nostdlib -D __68000__ -D ALIGN_MEMORY
LDFLAGS = -T ./linker.ld -L../../libc/lib  -nostdlib -L../../lib
LDLIBS = -lc68bios

ASMS =call_with_a5.S

VASMS =

SRCS = loader_pic.c slots.c \
vfs_drivers.c vfs_null.c vfs_serial.c vfs_zero.c \
shell.c vfs.c vfs_fat.c vfs_proc.c vfs_tty.c \
../apps_libs/fatfs/ff.c \
../apps_libs/fatfs/ffsystem.c \
../apps_libs/fatfs/ffunicode.c \
../apps_libs/ide/ata.c \
../apps_libs/picoVga/drv_picoVga.c \
../apps_libs/ide/diskio.c \
./fileio/f_close.c ./fileio/f_eof.c ./fileio/f_makedir.c ./fileio/f_open.c \
./fileio/f_read.c ./fileio/f_rename.c ./fileio/f_stat.c  ./fileio/f_tell.c \
./fileio/f_write.c ./fileio/f_closedir.c ./fileio/f_lseek.c ./fileio/f_mount.c \
./fileio/f_opendir.c ./fileio/f_readdir.c ./fileio/f_size.c ./fileio/f_sync.c \
./fileio/f_unlink.c ./fileio/f_findfirst.c ./fileio/f_findnext.c



# 2. ALTERADO: Agora a lista de objetos junta os .o vindos do .c e do .S
OBJS = $(SRCS:.c=.o) $(ASMS:.S=.o) $(VASMS:.asm=.o)

TARGET := shell
TARGET_OBJ := $(TARGET).o
TARGET_ELF := $(TARGET).elf
TARGET_BIN := $(TARGET).bin

.PHONY: all clean

all: shell.bin

# Regra para compilar os arquivos .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 3. ADICIONADO: Regra explícita para compilar arquivos Assembly (.S)
%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(VASM) $(VASMFLAGS) $< -o $@

# Regra específica para cada arquivo para evitar problemas com $<
shell.elf: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)
	@echo "Linked: $@"
	@echo "╔══════════════════════════╗"
	@echo "║  COMPILACAO TERMINADA!   ║"
	@echo "╚══════════════════════════╝"

shell.bin: shell.elf
	$(OBJCOPY) -O binary $< $@
	@echo "Binary: $@"

clean:
	rm -f *.bin $(OBJS) *.map $(TARGET_LOW) $(TARGET_HIGH) $(TARGET_ELF)
	@echo "Clean complete"

