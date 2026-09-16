/*
 * loader_pic.c -- SUBSTITUI o arquivo inteiro.
 *
 * Unica mudanca real de comportamento: no final, em vez de
 * call_with_a5(...) (chamada sincrona, kernel "trava" esperando o
 * programa terminar), agora chama OS_TaskCreatePIC(...) (cria uma
 * task nova e volta na hora -- quem decide quando ela roda e' o
 * scheduler dali pra frente).
 *
 * Tudo mais (leitura do header ELF, PT_LOAD, patch de GOT,
 * relocations) e' identico ao loader original que ja funcionava.
 *
 * LEMBRETE: OS_TaskCreatePIC precisa existir em algum .c do kernel
 * (cole o conteudo de scheduler_addition.c la) e estar declarada no
 * scheduler.h (cole scheduler_h_addition.txt la).
 */
#include <stddef.h>
#include <stdio.h>
#include "slots.h"
#include <string.h>
#include "sys/elf.h"
#include <fileio.h>
#include "scheduler.h"
#include <sys/reloc.h>

typedef struct {
    uint32_t name, type, flags, addr, offset, size, link, info, addralign, entsize;
} elf32_section_header;

//#define kprintf(...) printf(__VA_ARGS__)

/* AJUSTE: confirme que bate com o STACK_SIZE que voce ja usa no
 * run_scheduler_test / SLOT_SIZE do seu slots.h */
#define TASK_STACK_SIZE 8192
#define SLOT_SIZE       (256 * 1024)

static int find_got_section(FIL *fd, uint32_t shoff, uint16_t shnum,
                             uint16_t shentsize, uint16_t shstrndx,
                             uint32_t *got_addr, uint32_t *got_size)
{
    uint8_t hdr_buf[40];
    unsigned int bytesRead;
    uint32_t strtab_offset;
    uint32_t min_addr = 0xFFFFFFFF, max_end = 0;
    int found = 0;

    /* <<< COLA AQUI, antes do flseek original >>> */
    kprintf("shnum=%d shstrndx=%d shentsize=%d shoff=0x%lx\n",
            shnum, shstrndx, shentsize, (unsigned long)shoff);

    f_lseek(fd, shstrndx * shentsize + shoff);
    if (f_read(fd, hdr_buf, sizeof(hdr_buf), &bytesRead) != FR_OK || bytesRead != sizeof(hdr_buf))
        return 0;

    /* <<< E COLA AQUI, logo depois desse fread, antes de calcular strtab_offset >>> */
    kprintf("hdr_buf raw: ");
    for (int k = 0; k < 40; k++) kprintf("%02x ", hdr_buf[k]);
    kprintf("\n");

    strtab_offset = ((uint32_t)hdr_buf[0x10] << 24) | ((uint32_t)hdr_buf[0x11] << 16) |
                    ((uint32_t)hdr_buf[0x12] << 8)  |  (uint32_t)hdr_buf[0x13];

    /* <<< E ESSA LINHA, logo depois do calculo de strtab_offset >>> */
    kprintf("strtab_offset calculado=0x%lx\n", (unsigned long)strtab_offset);


    //f_lseek(fd, shstrndx * shentsize + shoff);
    //if (f_read(fd, hdr_buf, sizeof(hdr_buf), &bytesRead) != FR_OK || bytesRead != sizeof(hdr_buf))
    //    return 0;
    strtab_offset = ((uint32_t)hdr_buf[0x10] << 24) | ((uint32_t)hdr_buf[0x11] << 16) |
                    ((uint32_t)hdr_buf[0x12] << 8)  |  (uint32_t)hdr_buf[0x13];

    kprintf("shnum=%d shstrndx=%d strtab_offset=0x%x\n", shnum, shstrndx, (unsigned long)strtab_offset);

    for (uint16_t i = 0; i < shnum; i++) {
        f_lseek(fd, i * shentsize + shoff);
        if (f_read(fd, hdr_buf, sizeof(hdr_buf), &bytesRead) != FR_OK || bytesRead != sizeof(hdr_buf))
            return 0;

        uint32_t name_idx = ((uint32_t)hdr_buf[0] << 24) | ((uint32_t)hdr_buf[1] << 16) |
                             ((uint32_t)hdr_buf[2] << 8)  |  (uint32_t)hdr_buf[3];

        char name_buf[10];
        unsigned int nread;
        f_lseek(fd, strtab_offset + name_idx);
        FRESULT fr = f_read(fd, name_buf, sizeof(name_buf) - 1, &nread);
        name_buf[nread] = 0;

        /* LINHA NOVA DE DEBUG */
        kprintf("  sec[%d] name_idx=%d fr=%d nread=%d name='%s'\n",
                i, (unsigned long)name_idx, fr, nread, name_buf);

        if (strncmp(name_buf, ".got", 4) == 0) {
            uint32_t addr = ((uint32_t)hdr_buf[0x0C] << 24) | ((uint32_t)hdr_buf[0x0D] << 16) |
                             ((uint32_t)hdr_buf[0x0E] << 8)  |  (uint32_t)hdr_buf[0x0F];
            uint32_t size = ((uint32_t)hdr_buf[0x14] << 24) | ((uint32_t)hdr_buf[0x15] << 16) |
                             ((uint32_t)hdr_buf[0x16] << 8)  |  (uint32_t)hdr_buf[0x17];
            if (addr < min_addr) min_addr = addr;
            if (addr + size > max_end) max_end = addr + size;
            found = 1;
        }
    }

    if (!found) return 0;
    *got_addr = min_addr;
    *got_size = max_end - min_addr;
    return 1;
}


static int find_got_sectionw(FIL *fd, uint32_t shoff, uint16_t shnum,
                             uint16_t shentsize, uint16_t shstrndx,
                             uint32_t *got_addr, uint32_t *got_size)
{
    uint8_t hdr_buf[40];
    unsigned int bytesRead;
    uint32_t strtab_offset;
    uint32_t min_addr = 0xFFFFFFFF, max_end = 0;
    int found = 0;

    f_lseek(fd, shstrndx * shentsize + shoff);
    if (f_read(fd, hdr_buf, sizeof(hdr_buf), &bytesRead) != FR_OK || bytesRead != sizeof(hdr_buf))
        return 0;
    strtab_offset = ((uint32_t)hdr_buf[0x10] << 24) | ((uint32_t)hdr_buf[0x11] << 16) |
                    ((uint32_t)hdr_buf[0x12] << 8)  |  (uint32_t)hdr_buf[0x13];

    for (uint16_t i = 0; i < shnum; i++) {
        f_lseek(fd, i * shentsize + shoff);
        if (f_read(fd, hdr_buf, sizeof(hdr_buf), &bytesRead) != FR_OK || bytesRead != sizeof(hdr_buf))
            return 0;

        uint32_t name_idx = ((uint32_t)hdr_buf[0] << 24) | ((uint32_t)hdr_buf[1] << 16) |
                             ((uint32_t)hdr_buf[2] << 8)  |  (uint32_t)hdr_buf[3];

        char name_buf[10];
        unsigned int nread;
        f_lseek(fd, strtab_offset + name_idx);
        f_read(fd, name_buf, sizeof(name_buf) - 1, &nread);
        name_buf[nread] = 0;

        if (strncmp(name_buf, ".got", 4) == 0) {
            uint32_t addr = ((uint32_t)hdr_buf[0x0C] << 24) | ((uint32_t)hdr_buf[0x0D] << 16) |
                             ((uint32_t)hdr_buf[0x0E] << 8)  |  (uint32_t)hdr_buf[0x0F];
            uint32_t size = ((uint32_t)hdr_buf[0x14] << 24) | ((uint32_t)hdr_buf[0x15] << 16) |
                             ((uint32_t)hdr_buf[0x16] << 8)  |  (uint32_t)hdr_buf[0x17];
            if (addr < min_addr) min_addr = addr;
            if (addr + size > max_end) max_end = addr + size;
            found = 1;
        }
    }

    if (!found) return 0;
    *got_addr = min_addr;
    *got_size = max_end - min_addr;
    return 1;
}

static void apply_data_relocations(FIL *fd, uint32_t shoff, uint16_t shnum,
                                    uint16_t shentsize, uint16_t shstrndx,
                                    uint32_t task_base)
{
    uint8_t hdr_buf[40];
    unsigned int bytesRead;
    uint32_t strtab_offset;

    f_lseek(fd, shstrndx * shentsize + shoff);
    if (f_read(fd, hdr_buf, sizeof(hdr_buf), &bytesRead) != FR_OK || bytesRead != sizeof(hdr_buf))
        return;
    strtab_offset = ((uint32_t)hdr_buf[0x10] << 24) | ((uint32_t)hdr_buf[0x11] << 16) |
                    ((uint32_t)hdr_buf[0x12] << 8)  |  (uint32_t)hdr_buf[0x13];

    for (uint16_t i = 0; i < shnum; i++) {
        f_lseek(fd, i * shentsize + shoff);
        if (f_read(fd, hdr_buf, sizeof(hdr_buf), &bytesRead) != FR_OK || bytesRead != sizeof(hdr_buf))
            continue;

        uint32_t name_idx = ((uint32_t)hdr_buf[0] << 24) | ((uint32_t)hdr_buf[1] << 16) |
                             ((uint32_t)hdr_buf[2] << 8)  |  (uint32_t)hdr_buf[3];

        char name_buf[16];
        unsigned int nread;
        f_lseek(fd, strtab_offset + name_idx);
        f_read(fd, name_buf, sizeof(name_buf) - 1, &nread);
        name_buf[nread] = 0;

        if (strcmp(name_buf, ".rela.data") != 0)
            continue;

        uint32_t rela_offset = ((uint32_t)hdr_buf[0x10] << 24) | ((uint32_t)hdr_buf[0x11] << 16) |
                                ((uint32_t)hdr_buf[0x12] << 8)  |  (uint32_t)hdr_buf[0x13];
        uint32_t rela_size   = ((uint32_t)hdr_buf[0x14] << 24) | ((uint32_t)hdr_buf[0x15] << 16) |
                                ((uint32_t)hdr_buf[0x16] << 8)  |  (uint32_t)hdr_buf[0x17];

        uint8_t rela_buf[12];
        uint32_t num_relas = rela_size / 12;

        for (uint32_t r = 0; r < num_relas; r++) {
            f_lseek(fd, rela_offset + r * 12);
            if (f_read(fd, rela_buf, 12, &bytesRead) != FR_OK || bytesRead != 12)
                continue;

            uint32_t r_offset = ((uint32_t)rela_buf[0] << 24) | ((uint32_t)rela_buf[1] << 16) |
                                 ((uint32_t)rela_buf[2] << 8)  |  (uint32_t)rela_buf[3];
            uint32_t r_info   = ((uint32_t)rela_buf[4] << 24) | ((uint32_t)rela_buf[5] << 16) |
                                 ((uint32_t)rela_buf[6] << 8)  |  (uint32_t)rela_buf[7];
            uint32_t r_type = r_info & 0xFF;

            if (r_type != 1) /* so' R_68K_32 */
                continue;

            uint32_t *patch_addr = (uint32_t *)(task_base + r_offset);
            *patch_addr += task_base;
        }
        return;
    }
}

/*
 * Retorna 0 em sucesso (task criada e na fila do scheduler),
 * -1 em erro. NAO retorna mais o "valor de saida do programa" --
 * isso nao existe mais aqui, porque o loader nao espera a task
 * terminar.
 */
int load_pic_elf_standalone(FIL *fd, int argc, char *argv[])
{
    uint8_t header_buf[52];
    uint32_t *got;
    uint32_t got_addr_saved = 0, got_size_saved = 0;
    elf32_program_header progHeader;
    unsigned int bytesRead;
    uint32_t progIndex = 0;
    int pt_load_seen = 0;
    uint32_t text_load_addr = 0;
    uint32_t data_load_addr = 0;

    int slot_index = Slots_Alloc();
    if (slot_index < 0) {
        kprintf("Sem slots de memoria livres pra carregar o programa.\n");
        return -1;
    }
    uint32_t task_base = Slots_BaseAddr(slot_index);

    f_lseek(fd, 0);
    if (f_read(fd, header_buf, sizeof(header_buf), &bytesRead) != FR_OK || bytesRead != sizeof(header_buf)) {
        kprintf("Nao foi possivel ler o header do ELF.\n");
        goto fail;
    }

    uint8_t *ident_magic = &header_buf[0];
    uint8_t ident_class   = header_buf[4];
    uint8_t ident_data    = header_buf[5];
    uint8_t ident_version = header_buf[6];
    uint16_t type    = (header_buf[0x10] << 8) | header_buf[0x11];
    uint16_t machine = (header_buf[0x12] << 8) | header_buf[0x13];
    uint32_t entry = ((uint32_t)header_buf[0x18] << 24) | ((uint32_t)header_buf[0x19] << 16) |
                      ((uint32_t)header_buf[0x1A] << 8)  | (uint32_t)header_buf[0x1B];
    uint32_t phoff = ((uint32_t)header_buf[0x1C] << 24) | ((uint32_t)header_buf[0x1D] << 16) |
                      ((uint32_t)header_buf[0x1E] << 8)  | (uint32_t)header_buf[0x1F];
    uint32_t shoff = ((uint32_t)header_buf[0x20] << 24) | ((uint32_t)header_buf[0x21] << 16) |
                      ((uint32_t)header_buf[0x22] << 8)  | (uint32_t)header_buf[0x23];
    uint16_t phentsize = (header_buf[0x2A] << 8) | header_buf[0x2B];
    uint16_t phnum     = (header_buf[0x2C] << 8) | header_buf[0x2D];
    uint16_t shentsize = (header_buf[0x2E] << 8) | header_buf[0x2F];
    uint16_t shnum     = (header_buf[0x30] << 8) | header_buf[0x31];
    uint16_t shstrndx  = (header_buf[0x32] << 8) | header_buf[0x33];

    if (ident_magic[0] != 0x7F || ident_magic[1] != 'E' || ident_magic[2] != 'L' ||
        ident_magic[3] != 'F' || ident_version != 1) {
        kprintf("Header ELF invalido.\n");
        goto fail;
    }
    if (ident_class != ID_32BIT || ident_data != ID_BIG_ENDIAN) {
        kprintf("Nao e' um ELF 32-bit big-endian.\n");
        goto fail;
    }
    if (type != ET_EXEC) {
        kprintf("ELF nao e' executavel.\n");
        goto fail;
    }
    if (machine != EM_68K) {
        kprintf("ELF nao e' pra 68k.\n");
        goto fail;
    }

    while (progIndex < phnum) {
        f_lseek(fd, progIndex * phentsize + phoff);
        if (f_read(fd, &progHeader, sizeof(progHeader), &bytesRead) != FR_OK ||
            bytesRead != sizeof(progHeader)) {
            kprintf("Nao foi possivel ler program header do ELF.\n");
            goto fail;
        }

        if (progHeader.type == PT_LOAD) {
            uint32_t actual_addr = task_base + progHeader.paddr;

            f_lseek(fd, progHeader.offset);
            if (f_read(fd, (char *)actual_addr, progHeader.filesz, &bytesRead) != FR_OK ||
                bytesRead != progHeader.filesz) {
                kprintf("Falha lendo segmento PT_LOAD do ELF.\n");
                goto fail;
            }
            if (progHeader.memsz > progHeader.filesz) {
                memset((char *)actual_addr + progHeader.filesz, 0,
                       progHeader.memsz - progHeader.filesz);
            }

            if (pt_load_seen == 0) {
                text_load_addr = actual_addr;
            } else if (pt_load_seen == 1) {
                data_load_addr = actual_addr;

                uint32_t got_addr, got_size;
                if (find_got_section(fd, shoff, shnum, shentsize, shstrndx, &got_addr, &got_size)) {
                    got = (uint32_t *)(task_base + got_addr);
                    uint32_t got_words = got_size / 4;
                    for (uint32_t w = 0; w < got_words; w++)
                        got[w] += task_base;

                    got_addr_saved = got_addr;
                    got_size_saved = got_size;
                } else {
                    kprintf("Erro: secao .got nao encontrada -- binario nao parece ser PIC valido.\n");
                    goto fail;   /* antes era so' um aviso e seguia em frente -- isso e' que travava depois */
                }                
                apply_data_relocations(fd, shoff, shnum, shentsize, shstrndx, task_base);
            }
            pt_load_seen++;
        } else if (progHeader.type == PT_DYNAMIC || progHeader.type == PT_SHLIB) {
            kprintf("ELF dinamicamente linkado -- nao suportado.\n");
            goto fail;
        } else if (progHeader.type == PT_INTERP) {
            kprintf("ELF exige interpretador -- nao suportado.\n");
            goto fail;
        }

        progIndex++;
    }

    if (pt_load_seen != 2) {
        kprintf("ELF PIC esperava exatamente 2 segmentos PT_LOAD, achou %d.\n", pt_load_seen);
        goto fail;
    }

    kprintf("slot=%02d task_base=0x%08x text=0x%08x data=0x%08x entry=0x%08x\n",
            slot_index, (unsigned long)task_base, (unsigned long)text_load_addr,
            (unsigned long)data_load_addr, (unsigned long)(task_base + entry));

    /* ========================================================
     * PARTE NOVA: em vez de call_with_a5 (sincrono), cria task
     * de verdade via OS_TaskCreatePIC e devolve na hora.
     * ======================================================== */
    {
        void (*entry_fn)(void) = (void (*)(void))(task_base + entry);
        uint32_t a5_value = task_base + got_addr_saved;
        uint8_t *task_stack = (uint8_t *)(task_base + SLOT_SIZE - TASK_STACK_SIZE);

        tcb_t *tcb = OS_TaskCreatePIC(entry_fn, NULL, task_stack, TASK_STACK_SIZE,
                                       /*id=*/slot_index, a5_value);
        if (!tcb) {
            kprintf("Falha ao criar task.\n");
            goto fail;
        }

        /* NAO chama Slots_Free aqui -- o slot pertence a' task agora.
         * So' libera quando a task der exit (isso ainda falta
         * implementar em outro lugar -- quem chama Slots_Free quando
         * uma task termina). */

        return 0;
    }

fail:
    Slots_Free(slot_index);
    return -1;
}
