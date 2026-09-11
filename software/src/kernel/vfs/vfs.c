#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/vfs.h>
#include <sys/kmalloc.h>
#include "fileio.h" 

// ============================================
// TABELA DE DESCRITORES GLOBAL
// ============================================
File *fd_table[256];
static int fd_count = 0;

extern DeviceDriver drivers[];

// ============================================
// FUNÇÕES DE ACESSO À TABELA DE DESCRITORES
// ============================================

void vfs_set_fd(int fd, File *file) {
    if (fd >= 0 && fd < 256) {
        fd_table[fd] = file;
    }
}

File *vfs_get_fd(int fd) {
    if (fd >= 0 && fd < 256) {
        return fd_table[fd];
    }
    return NULL;
}

void vfs_restore_fd(int fd) {
    if (fd >= 0 && fd < 256) {
        // Restaura para /dev/tty
        int tty_fd = vfs_open("/dev/tty", O_RDWR);
        if (tty_fd >= 0) {
            fd_table[fd] = fd_table[tty_fd];
            fd_table[tty_fd] = NULL;
        }
    }
}

// ============================================
// ALLOCATE_FD
// ============================================
int allocate_fd(File *file) {
    for (int i = 3; i < 256; i++) {
        if (fd_table[i] == NULL) {
            fd_table[i] = file;
            fd_count++;
            return i;
        }
    }
    return -1;
}

// ============================================
// GET_FILE_FROM_FD
// ============================================
File *get_file_from_fd(int fd) {
    if (fd < 0 || fd >= 256) return NULL;
    return fd_table[fd];
}

// ============================================
// kfree_FD
// ============================================
void kfree_fd(int fd) {
    if (fd >= 0 && fd < 256) {
        fd_table[fd] = NULL;
        fd_count--;
    }
}

// ============================================
// VFS_OPEN
// ============================================
// vfs.c - vfs_open()
extern int fat_open(File *file, const char *path, int flags);
int vfs_open(const char *path, int flags) {
    //kprintf("path[%s] flags[%x]\n",path,flags);
    File *file = (File*)kmalloc(sizeof(File));
    //kprintf("kmalloc\n");
    if (!file) {
            //kprintf("essa merda deu erro logo no malloc\n");
        return -1;
    }
    //kprintf("89\n");
    memset(file, 0, sizeof(File));
    //kprintf("91\n");
    file->name = strdup(path);
    //kprintf("93\n");
    
    if (fat_open(file, path, flags) == 0) {
        int fd = allocate_fd(file);
        if (fd >= 0) {
            //kprintf("essa merda funcionou\n");
            return fd;
        }
        kfree(file->name);
        kfree(file);

        return -1;
    }
    //kprintf("104\n");
    
    for (int i = 0; drivers[i].path != NULL; i++) {
        //kprintf("vfs_open: comparando '%s' com '%s'\n", path, drivers[i].path);
        if (strcmp(path, drivers[i].path) == 0) {
           // printf("vfs_open: ENCONTROU! i=%d\n", i);
            if (drivers[i].open(file, path, flags) == 0) {
                int fd = allocate_fd(file);
                if (fd >= 0) {
                    //kprintf("vfs_open: fd=%d (SUCESSO!)\n", fd);
                    return fd;
                }
            }
        }
    }
    //kprintf("119\n");
    
    kfree(file->name);
    kfree(file);
    //kprintf("essa merda deu erro\n");

    return -1;
}

// ============================================
// VFS_READ
// ============================================
int vfs_read(int fd, void *buffer, size_t size) {
    File *file = get_file_from_fd(fd);
    if (!file) return -1;
    if (file->read) return file->read(file, buffer, size);
    return -1;
}

// ============================================
// VFS_WRITE
// ============================================
int vfs_write(int fd, const void *buffer, size_t size) {
    File *file = get_file_from_fd(fd);
    if (!file) return -1;
    if (file->write) return file->write(file, buffer, size);
    return -1;
}

// ============================================
// VFS_CLOSE
// ============================================
int vfs_close(int fd) {
    File *file = get_file_from_fd(fd);
    if (!file) return -1;
    
    int result = 0;
    if (file->close) result = file->close(file);
    
    kfree(file->name);
    kfree(file);
    kfree_fd(fd);
    return result;
}

// ============================================
// VFS_IOCTL
// ============================================
int vfs_ioctl(int fd, int cmd, void *arg) {
    File *file = get_file_from_fd(fd);
    if (!file) return -1;
    if (file->ioctl) return file->ioctl(file, cmd, arg);
    return -1;
}

// ============================================
// VFS_LSEEK
// ============================================
size_t vfs_lseek(int fd, size_t offset, int whence) {
    File *file = get_file_from_fd(fd);
    if (!file) return (size_t)-1;
    
    if (file->lseek) return file->lseek(file, offset, whence);
    
    switch (whence) {
        case 0: file->position = offset; break;
        case 1: file->position += offset; break;
        case 2: return (size_t)-1;
        default: return (size_t)-1;
    }
    return file->position;
}

// ============================================
// VFS_INIT
// ============================================
void vfs_inita(void) {
    memset(fd_table, 0, sizeof(fd_table));
    fd_count = 0;
    
    int fd0 = vfs_open("/dev/tty", O_RDWR);
    int fd1 = vfs_open("/dev/tty", O_RDWR);
    int fd2 = vfs_open("/dev/tty", O_RDWR);
    
    if (fd0 != 0) {
        fd_table[0] = fd_table[fd0];
        fd_table[fd0] = NULL;
        fd_count--;
    }
    if (fd1 != 1) {
        fd_table[1] = fd_table[fd1];
        fd_table[fd1] = NULL;
        fd_count--;
    }
    if (fd2 != 2) {
        fd_table[2] = fd_table[fd2];
        fd_table[fd2] = NULL;
        fd_count--;
    }
    
    //kprintf("VFS inicializado: stdin=0, stdout=1, stderr=2\n");
}


void vfs_init(void) {
    //kprintf("vfs_init: memset\n");
    memset(fd_table, 0, sizeof(fd_table));
    fd_count = 0;

    //kprintf("vfs_init: abrindo fd0\n");
    int fd0 = vfs_open("/dev/tty", O_RDWR);
    //kprintf("vfs_init: fd0 =[%d] ",fd0);

    //kprintf("vfs_init: abrindo fd1\n");
    int fd1 = vfs_open("/dev/tty", O_RDWR);
    //kprintf("vfs_init: fd2 =[%d] ",fd1);

    //kprintf("vfs_init: abrindo fd2\n");
    int fd2 = vfs_open("/dev/tty", O_RDWR);
    //kprintf("vfs_init: fd2 =[%d] ",fd2);

    //kprintf("vfs_init: ajustando\n");
    /* ... */
    //kprintf("vfs_init: OK\n");
}