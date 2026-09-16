#include <stdint.h>
#include <sys/vfs.h>
#include <sys/syscall.h>


int trap0_handler(int num, long a1, long a2, long a3, long a4, long a5, long a6, long a7){
    switch(num){
    case    SYS_OPEN:
                    return vfs_open((const char *)a1,(int)a2);
    case    SYS_READ:
                    return vfs_read((int) a1,(void *) a2,(uint32_t)a3);
    case    SYS_WRITE:
                    return vfs_write((int) a1,(void *) a2,(uint32_t)a3);
    case    SYS_CLOSE:
                    return vfs_close((int) a1);
    case    SYS_IOCTL:
                    return vfs_ioctl((int) a1,(int)a2,(void *) a3);
    case    SYS_LSEEK:
                    return vfs_lseek((int) a1,(uint32_t)a2,(int) a3);
    }
    return 0;
}

