# ============================================
# MAKEFILE UNIFICADO - ORION68DOS
# ============================================
# Junta libc, libdiskio, libdrivers, libfileio,
# librtc e libvideo num UNICO .a -- sem mais
# sub-Makefiles chamados via $(MAKE) -C.
#
# PREMISSAS (ajuste se nao bater com sua estrutura real):
#  - cada lib continua em seu proprio diretorio
#    (libc/src, libdiskio/src, etc), so' o DESTINO
#    final mudou de 6 .a pra 1 .a so'.
#  - cada libX PODE ter seu proprio libX/include/
#    (opcional -- se nao existir, o -I dela e'
#    simplesmente ignorado pelo compilador sem erro).
#  - existe um include/ GLOBAL num nivel acima
#    deste Makefile (ajuste INC_DIR se for outro
#    caminho -- no Makefile de lib individual que
#    voce mandou, era "../../include" visto de
#    DENTRO de libX/, o que corresponde a "../include"
#    visto DAQUI, do nivel onde as libX/ moram).
# ============================================

CC       = m68k-elf-gcc
AR       = m68k-elf-ar
ARFLAGS  = rcs

LIBS     = libc libdiskio libdrivers libfileio librtc libvideo

OBJ_DIR  = obj
LIB_DIR  = ../lib
LIB_NAME = libc68k.a
INC_DIR  = ../include

CFLAGS   = -Wall -Os -ffreestanding -nostdlib \
           $(foreach d,$(LIBS),-I$(d)/include) \
           -I$(INC_DIR)

# --- descobre TODOS os fontes de TODAS as libs de uma vez ---
C_SRCS   := $(shell find $(LIBS:%=%/src) -name '*.c' 2>/dev/null)
ASM_SRCS := $(shell find $(LIBS:%=%/src) -name '*.S' 2>/dev/null)

# --- mantem o caminho completo (libX/src/.../arquivo) dentro de obj/,
#     assim arquivos de mesmo nome em libs diferentes nunca colidem ---
C_OBJS   := $(C_SRCS:%.c=$(OBJ_DIR)/%.o)
ASM_OBJS := $(ASM_SRCS:%.S=$(OBJ_DIR)/%.o)
OBJS     := $(C_OBJS) $(ASM_OBJS)

.PHONY: all lib clean info help

all: lib

lib: $(LIB_DIR)/$(LIB_NAME)

$(LIB_DIR)/$(LIB_NAME): $(OBJS)
	@mkdir -p $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^
	@echo "Biblioteca unificada criada: $@ ($(words $(OBJS)) objetos)"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR)/$(LIB_NAME)

info:
	@echo "=== CONTEUDO DA BIBLIOTECA ==="
	@m68k-elf-ar -t $(LIB_DIR)/$(LIB_NAME)
	@echo ""
	@echo "=== SIMBOLOS DISPONIVEIS (T = definido, U = indefinido) ==="
	@m68k-elf-nm $(LIB_DIR)/$(LIB_NAME) | grep " T " || echo "Nenhum simbolo encontrado"

help:
	@echo "Targets disponiveis:"
	@echo "  all     - Compila TODAS as libs num unico $(LIB_NAME)"
	@echo "  clean   - Remove obj/ e o .a final"
	@echo "  info    - Lista simbolos definidos no .a final"
	@echo "  help    - Mostra esta mensagem"
	@echo ""
	@echo "Libs incluidas: $(LIBS)"
