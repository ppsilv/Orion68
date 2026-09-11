
#include <stdio.h>
#include <interrupt.h>
#include <critical.h>
#include "scheduler.h"

long sys_exit(long code){}
long sys_spawn(char *path){}
long sys_sleep(long ticks){}
long sys_yield(void){}

/* arquivo / vfs */
long sys_open(char *path, long flags){}
long sys_close(long fd){}
long sys_read(long fd, char *buf, long len){}
long sys_write(long fd, char *buf, long len){}
long sys_sinc(long fd){}
long sys_lseek(long fd, long offset, long whence){}
long sys_ioctl(long fd, long cmd, void *arg){}
long sys_dup(long fd_old, long fd_new){}
long sys_unlink(char *path){}
long sys_chmod(char *path, long attrs){}
long sys_tell(long fd){}
long sys_size(long fd){}
long sys_eof(long fd){}
long sys_rename(char *old_path, char *new_path){}
long sys_stat(char *path, void *stat_buf){}
long sys_opendir(char *path, void *dir){}
long sys_readdir(void *dir, void *file_info){}
long sys_mount(char *path){}
long sys_closedir(void *dir){}
long sys_fmkdir(char *path){}
long sys_unmount(char *path){}
long sys_findfirst(void *dir, void *file_info, char *path, char *pattern){}
long sys_findnext(void *dir, void *file_info){}

/* picovga */
long sys_vga_putchar(long c){}
long sys_gotoxy(long x, long y){}
long sys_gohome(void){}
long sys_settextcolor(long color){}
long sys_clrscr(void){}
long sys_drawvline(long x, long y, long h, long color){}
long sys_drawhline(long x, long y, long w, long color){}
long sys_drawline(long x0, long y0, long x1, long y1, long color){}
long sys_drawcircle(long x0, long y0, long r, long color){}
long sys_drawcirclehelper(long x0, long y0, long r, long cornername, long color){}
long sys_fillcircle(long x0, long y0, long r, long color){}
long sys_fillcirclehelper(long x0, long y0, long r, long cornername, long delta, long color){}
long sys_drawroundrect(long x, long y, long w, long h, long r, long color){}
long sys_fillroundrect(long x, long y, long w, long h, long r, long color){}
long sys_fillrect(long x, long y, long w, long h, long color){}

/* rtc */
long sys_readtime(void *tm_out){}
long sys_writetime(void *tm_in){}

/* eeprom */
long sys_eepromreadbyte(long addr){}
long sys_eepromwritebyte(long addr, long value){}

/* ring buffer */
long sys_ringbufferread(void *rb){}
long sys_ringbufferwrite(void *rb, long value){}

/* duart */
long sys_duartacfg(long baud, long params){}
long sys_duartaread(void){}
long sys_duartawrite(long value){}
long sys_duartbcfg(long baud, long params){}
long sys_duartbread(void){}
long sys_duartbwrite(long value){}

/* ppi */
long sys_ppiacfg(long mode){}
long sys_ppiportaread(void){}
long sys_ppiportawrite(long value){}
long sys_ppibcfg(long mode){}
long sys_ppiportbread(void){}
long sys_ppiportbwrite(long value){}
long sys_ppiccfg(long mode){}
long sys_ppiportcread(void){}
long sys_ppiportcwrite(long value){}

/* sockets (w5100 / lwip) */
long sys_connect(long sockfd, void *addr, long addrlen){}
long sys_bind(long sockfd, void *addr, long addrlen){}
long sys_listen(long sockfd, long backlog){}
long sys_accept(long sockfd, void *addr, long *addrlen){}
long sys_setsockopt(long sockfd, long level, long optname, void *optval, long optlen){}
long sys_getpid(void){}


void sys_setramvector(uint32_t stub_addr, uint32_t handler_addr)
{
    uint8_t *p = (uint8_t *) stub_addr;

    *p++ = 0x4E;
    *p++ = 0xF9;   /* JMP absoluto de 32 bits */

    *p++ = (uint8_t)((handler_addr >> 24) & 0xFF);
    *p++ = (uint8_t)((handler_addr >> 16) & 0xFF);
    *p++ = (uint8_t)((handler_addr >> 8)  & 0xFF);
    *p++ = (uint8_t)( handler_addr        & 0xFF);
}