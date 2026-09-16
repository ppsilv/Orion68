/* fsattrib.h - OrionOS
 * Atributos de arquivos e diretórios para o OrionFS.
 *
 * Ambiente freestanding. Não depende de libc.
 */

#ifndef _ORION_FSATTRIB_H
#define _ORION_FSATTRIB_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================
 * Flags de atributos (bitmask de 32 bits)
 *
 * Organizadas por grupos. Os bits 0-15 são compatíveis com
 * FAT/NTFS (facilita interoperabilidade futura). Os bits 16-31
 * são específicos do OrionOS.
 * ============================================================ */

/* --- Grupo 1: básicos (compatíveis com DOS/FAT) --- */
#define FS_ATTR_READ_ONLY    (1u << 0)   /* 0x00000001 */
#define FS_ATTR_HIDDEN       (1u << 1)   /* 0x00000002 */
#define FS_ATTR_SYSTEM       (1u << 2)   /* 0x00000004 */
#define FS_ATTR_VOLUME_ID    (1u << 3)   /* 0x00000008 */
#define FS_ATTR_DIRECTORY    (1u << 4)   /* 0x00000010 */
#define FS_ATTR_ARCHIVE      (1u << 5)   /* 0x00000020 */

/* --- Grupo 2: NTFS-like --- */
#define FS_ATTR_DEVICE       (1u << 6)   /* 0x00000040 */
#define FS_ATTR_NORMAL       (1u << 7)   /* 0x00000080 */
#define FS_ATTR_TEMPORARY    (1u << 8)   /* 0x00000100 */
#define FS_ATTR_SPARSE_FILE  (1u << 9)   /* 0x00000200 */
#define FS_ATTR_REPARSE_PT   (1u << 10)  /* 0x00000400 */
#define FS_ATTR_COMPRESSED   (1u << 11)  /* 0x00000800 */
#define FS_ATTR_OFFLINE      (1u << 12)  /* 0x00001000 */
#define FS_ATTR_NOT_INDEXED  (1u << 13)  /* 0x00002000 */
#define FS_ATTR_ENCRYPTED    (1u << 14)  /* 0x00004000 */

/* --- Grupo 3: específicos OrionOS --- */
#define FS_ATTR_IMMUTABLE    (1u << 16)  /* não pode ser modificado nem removido */
#define FS_ATTR_APPEND_ONLY  (1u << 17)  /* só permite append */
#define FS_ATTR_NO_DUMP      (1u << 18)  /* ignorar em backup */
#define FS_ATTR_NO_ATIME     (1u << 19)  /* não atualizar atime */
#define FS_ATTR_SYNC         (1u << 20)  /* escrita síncrona */
#define FS_ATTR_NOCOW        (1u << 21)  /* desabilita copy-on-write */
#define FS_ATTR_EXECUTABLE   (1u << 22)  /* bit de execução (estilo Unix) */

/* ============================================================
 * Máscaras auxiliares
 * ============================================================ */
#define FS_ATTR_MASK_BASIC   (FS_ATTR_READ_ONLY | FS_ATTR_HIDDEN | \
                              FS_ATTR_SYSTEM    | FS_ATTR_ARCHIVE)

#define FS_ATTR_MASK_USER    (FS_ATTR_READ_ONLY | FS_ATTR_HIDDEN | \
                              FS_ATTR_SYSTEM    | FS_ATTR_ARCHIVE | \
                              FS_ATTR_IMMUTABLE | FS_ATTR_APPEND_ONLY)

#define FS_ATTR_MASK_ALL     (0xFFFFFFFFu)

/* ============================================================
 * Tipo do atributo
 * ============================================================ */
typedef uint32_t fs_attrib_t;

/* ============================================================
 * Permissões estilo Unix (bits 0-8) armazenadas separadamente
 * ============================================================ */
#define FS_PERM_OWNER_READ    (1u << 8)   /* 0x100 */
#define FS_PERM_OWNER_WRITE   (1u << 7)   /* 0x080 */
#define FS_PERM_OWNER_EXEC    (1u << 6)   /* 0x040 */
#define FS_PERM_GROUP_READ    (1u << 5)   /* 0x020 */
#define FS_PERM_GROUP_WRITE   (1u << 4)   /* 0x010 */
#define FS_PERM_GROUP_EXEC    (1u << 3)   /* 0x008 */
#define FS_PERM_OTHER_READ    (1u << 2)   /* 0x004 */
#define FS_PERM_OTHER_WRITE   (1u << 1)   /* 0x002 */
#define FS_PERM_OTHER_EXEC    (1u << 0)   /* 0x001 */

#define FS_PERM_DEFAULT_FILE  0644u
#define FS_PERM_DEFAULT_DIR   0755u

/* ============================================================
 * Estrutura de metadados de um arquivo
 * ============================================================ */
typedef struct fs_stat {
    uint64_t      inode;        /* número do inode */
    uint64_t      size;         /* tamanho em bytes */
    uint64_t      blocks;       /* blocos alocados (512B cada) */
    uint32_t      uid;          /* dono */
    uint32_t      gid;          /* grupo */
    uint16_t      mode;         /* permissões Unix */
    fs_attrib_t   attrib;       /* flags de atributo */
    uint64_t      atime;        /* último acesso (epoch) */
    uint64_t      mtime;        /* última modificação */
    uint64_t      ctime;        /* última mudança de metadados */
    uint64_t      btime;        /* criação (birth time) */
    uint32_t      link_count;   /* número de hard links */
    uint32_t      _reserved;    /* alinhamento */
} fs_stat_t;

/* ============================================================
 * API de manipulação de atributos
 * ============================================================ */

/* Retorna 1 se o atributo está setado, 0 caso contrário */
static inline int fs_attrib_has(fs_attrib_t a, fs_attrib_t flag)
{
    return (a & flag) != 0;
}

/* Seta os bits indicados */
static inline fs_attrib_t fs_attrib_set(fs_attrib_t a, fs_attrib_t flags)
{
    return a | flags;
}

/* Limpa os bits indicados */
static inline fs_attrib_t fs_attrib_clear(fs_attrib_t a, fs_attrib_t flags)
{
    return a & ~flags;
}

/* Alterna os bits indicados */
static inline fs_attrib_t fs_attrib_toggle(fs_attrib_t a, fs_attrib_t flags)
{
    return a ^ flags;
}

/* Verifica se é diretório */
static inline int fs_is_dir(fs_attrib_t a)
{
    return fs_attrib_has(a, FS_ATTR_DIRECTORY);
}

/* Verifica se é somente leitura */
static inline int fs_is_readonly(fs_attrib_t a)
{
    return fs_attrib_has(a, FS_ATTR_READ_ONLY) ||
           fs_attrib_has(a, FS_ATTR_IMMUTABLE);
}

/* Verifica se pode escrever (append ou escrita normal) */
static inline int fs_is_writable(fs_attrib_t a)
{
    return !fs_is_readonly(a);
}

/* ============================================================
 * Constantes de erro (usadas pelas funções do FS)
 * ============================================================ */
#define FS_OK              0
#define FS_ERR_NOTFOUND   -1
#define FS_ERR_ACCESS     -2
#define FS_ERR_EXISTS     -3
#define FS_ERR_NOSPACE    -4
#define FS_ERR_NOTDIR     -5
#define FS_ERR_ISDIR      -6
#define FS_ERR_INVAL      -7
#define FS_ERR_IO         -8
#define FS_ERR_CORRUPT    -9
#define FS_ERR_ROFS      -10   /* read-only filesystem */


#include <fatfs/ff.h>

typedef struct {
    char letra;
    BYTE bit;
} attr_map_t;

static const attr_map_t ATTR_TABLE[] = {
    { 'r', AM_RDO },
    { 'h', AM_HID },
    { 's', AM_SYS },
    { 'a', AM_ARC },
};
#define ATTR_TABLE_LEN (sizeof(ATTR_TABLE) / sizeof(ATTR_TABLE[0]))

static inline void fmt_attr_string(BYTE attr, char *out)
{
    uint8_t i;
    for (i = 0; i < ATTR_TABLE_LEN; i++) {
        out[i] = (attr & ATTR_TABLE[i].bit)
                     ? (char) (ATTR_TABLE[i].letra - 'a' + 'A')
                     : '-';
    }
    out[ATTR_TABLE_LEN] = '\0';
}



#ifdef __cplusplus
}
#endif

#endif /* _ORION_FSATTRIB_H */
