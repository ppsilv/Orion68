#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/vfs.h>
#include <sys/kmalloc.h>

// ============================================
// ESTRUTURA PRIVADA PARA ARQUIVOS FATFS
// ============================================
typedef struct {
    FIL *fat_file;
    BYTE mode;
    FSIZE_t size;
} FatPrivate;


// ============================================
// FAT_READ
// ============================================
static int fat_read(File *file, void *buffer, size_t size) {
    FatPrivate *priv = (FatPrivate*)file->private_data;
    if (!priv) return -1;
    
    UINT bytes_read;
    FRESULT result = f_read(priv->fat_file, buffer, (UINT)size, &bytes_read);
    
    if (result != FR_OK) return -1;
    file->position += bytes_read;
    return (int)bytes_read;
}

// ============================================
// FAT_WRITE
// ============================================
static int fat_write(File *file, const void *buffer, size_t size) {
    FatPrivate *priv = (FatPrivate*)file->private_data;
    if (!priv) return -1;
    
    // ============================================
    // ANTES DE ESCREVER, VAI PARA O FINAL DO ARQUIVO!
    // ============================================
    if (priv->mode & FA_OPEN_APPEND) {
        //kprintf("Appending...\n");
        f_lseek(priv->fat_file, f_size(priv->fat_file));  // ← VAI PARA O FINAL!
    }

    UINT bytes_written;
    FRESULT result = f_write(priv->fat_file, buffer, (UINT)size, &bytes_written);
    
    if (result != FR_OK) return -1;
    file->position += bytes_written;
    
    if (file->position > priv->size) {
        priv->size = file->position;
    }
    
    return (int)bytes_written;
}

// ============================================
// FAT_CLOSE
// ============================================
static int fat_close(File *file) {
    FatPrivate *priv = (FatPrivate*)file->private_data;
    if (!priv) return -1;
    
    FRESULT result = f_close(priv->fat_file);
    kfree(priv->fat_file);
    kfree(priv);
    file->private_data = NULL;
    
    return (result == FR_OK) ? 0 : -1;
}

// ============================================
// FAT_LSEEK
// ============================================
static size_t fat_lseek(File *file, size_t offset, int whence) {
    FatPrivate *priv = (FatPrivate*)file->private_data;
    if (!priv) return (size_t)-1;
    
    FSIZE_t new_pos;
    FSIZE_t current = f_tell(priv->fat_file);
    
    switch (whence) {
        case 0: new_pos = (FSIZE_t)offset; break;
        case 1: new_pos = current + (FSIZE_t)offset; break;
        case 2: new_pos = priv->size + (FSIZE_t)offset; break;
        default: return (size_t)-1;
    }
    
    FRESULT result = f_lseek(priv->fat_file, new_pos);
    if (result != FR_OK) return (size_t)-1;
    
    file->position = (size_t)new_pos;
    return (size_t)new_pos;
}

// ============================================
// FAT_OPEN
// ============================================
// vfs_fat.c - CORRIGIDO
// vfs_fat.c - fat_open()
int fat_open(File *file, const char *path, int flags) {
    //kprintf("fat_open: path='%s', flags=0x%x\n", path, flags);
    
    FIL *fat_file = (FIL*)kmalloc(sizeof(FIL));
    if (!fat_file) return -1;
    
    BYTE fat_mode = 0;
    if (flags & O_RDONLY) fat_mode |= FA_READ;
    if (flags & O_WRONLY) fat_mode |= FA_WRITE;
    if (flags & O_RDWR)   fat_mode |= FA_READ | FA_WRITE;
    
    // ============================================
    // CRIA O ARQUIVO SE NÃO EXISTIR
    // ============================================
    if (flags & O_CREAT) {
        //printf("fat_open: O_CREAT detectado! Criando arquivo...\n");
        fat_mode |= FA_CREATE_ALWAYS;
    }
    
    if (flags & O_TRUNC) {
        //printf("fat_open: O_TRUNC detectado!\n");
        fat_mode |= FA_CREATE_ALWAYS;
    }
    
    if (flags & O_APPEND) {
        //kprintf("fat_open: O_APPEND detectado!\n");
        fat_mode |= FA_OPEN_APPEND;
    }
    
    //kprintf("fat_open: fat_mode=0x%x\n", fat_mode);
    
    // CHAMA SUA fopen() (trap #12)
    FRESULT result = f_open(fat_file, path, fat_mode);
   //kprintf("fat_open: fopen result=%d\n", result);
    
    if (result != FR_OK) {
       //kprintf("fat_open: fopen falhou com erro %d!\n", result);
        kfree(fat_file);
        return -1;
    }
    
   //kprintf("fat_open: FATFS abriu!\n");
    
    // Configura o File
    FatPrivate *priv = (FatPrivate*)kmalloc(sizeof(FatPrivate));
    if (!priv) {
        f_close(fat_file);
        kfree(fat_file);
        return -1;
    }
    
    priv->fat_file = fat_file;
    priv->mode = fat_mode;
    priv->size = f_size(fat_file);
    
    file->private_data = priv;
    file->position = 0;
    file->read  = fat_read;
    file->write = fat_write;
    file->close = fat_close;
    file->lseek = fat_lseek;
    
    return 0;
}
