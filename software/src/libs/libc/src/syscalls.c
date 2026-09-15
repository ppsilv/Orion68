#include <stdint.h>
#include "sys/syscall.h"
#include "fatfs/ff.h"
#include "orion_rtc.h"

/* Faz a TRAP #0 com o número da syscall em D0, argumentos em D1/D2/D3 -
   ajusta aqui pra bater exatamente com o dispatch table que você já tem
   no lado do kernel (convenção de registrador por syscall). */
static inline int do_trap3(int num, long a1, long a2, long a3) {
    register int d0 asm("d0") = num;
    register long d1 asm("d1") = a1;
    register long d2 asm("d2") = a2;
    register long d3 asm("d3") = a3;
    asm volatile ("trap #0" : "+r"(d0) : "r"(d1),"r"(d2),"r"(d3) : "memory");
    return d0;
}

static inline int do_trap4(int num, long a1, long a2, long a3, long a4) {
    register int d0 asm("d0") = num;
    register long d1 asm("d1") = a1;
    register long d2 asm("d2") = a2;
    register long d3 asm("d3") = a3;
    register long d4 asm("d4") = a4;
    asm volatile ("trap #0" : "+r"(d0) : "r"(d1),"r"(d2),"r"(d3),"r"(d4) : "memory");
    return d0;
}

static inline int do_trap6(int num, long a1, long a2, long a3, long a4, long a5, long a6) {
    register int  d0 asm("d0") = num;
    register long d1 asm("d1") = a1;
    register long d2 asm("d2") = a2;
    register long d3 asm("d3") = a3;
    register long d4 asm("d4") = a4;
    register long d5 asm("d5") = a5;
    register long d6 asm("d6") = a6;
    asm volatile ("trap #0"
        : "+r"(d0)
        : "r"(d1),"r"(d2),"r"(d3),"r"(d4),"r"(d5),"r"(d6)
        : "memory");
    return d0;
}

static inline int do_trap7(int num, long a1, long a2, long a3, long a4, long a5, long a6, long a7) {
    register int  d0 asm("d0") = num;
    register long d1 asm("d1") = a1;
    register long d2 asm("d2") = a2;
    register long d3 asm("d3") = a3;
    register long d4 asm("d4") = a4;
    register long d5 asm("d5") = a5;
    register long d6 asm("d6") = a6;
    register long d7 asm("d7") = a7;
    asm volatile ("trap #0"
        : "+r"(d0)
        : "r"(d1),"r"(d2),"r"(d3),"r"(d4),"r"(d5),"r"(d6),"r"(d7)
        : "memory");
    return d0;
}
/*
 *  VFS calls.
 */
int open(const char *path, int flags){
    return do_trap3(SYS_OPEN, (long)path, flags, 0);
}
int read(int fd, void *buf, int count){
    return do_trap3(SYS_READ, fd, (long)buf, count );
}
int write(int fd, const void *buf, int count){
    return do_trap3(SYS_WRITE, fd, (long)buf, count );
}
int close(int fd){
    return do_trap3(SYS_CLOSE, fd, 0, 0);
}
int ioctl(int fd, int cmd, void *arg){
    return do_trap3(SYS_IOCTL, fd, cmd,(long)arg);
}
int lseek(int fd, uint32_t offset, int whence){
    return do_trap3(SYS_LSEEK, fd, offset, whence);
}
/*
 *  VIDEO calls for text
 */
int video_writechar(int fd, char ch){
    return do_trap6(SYS_VGAPUTCHAR,fd, ch, 0, 0, 0, 0);
}

int gotoxy(int fd, uint16_t x,uint16_t y){
    return do_trap6(SYS_GOTOXY, fd, x, y, 0, 0, 0);
}
int gohome(int fd){
    return do_trap6(SYS_GOHOME, fd, 0, 0, 0, 0, 0);
}
int setcolor(int fd, uint8_t txtcolor,uint8_t bgcolor){
    uint8_t color;
    color  = (txtcolor << 4) & 0xF0;
    color |= (bgcolor & 0x0F);
    return do_trap6(SYS_SETTEXTCOLOR, fd, color, 0, 0, 0, 0);
}
int clrscr(int fd){
    return do_trap6(SYS_CLRSCR, fd, 0, 0, 0, 0, 0);
}
/*
 *  VIDEO calls for graphics
 */
int drawVLine(int fd, uint16_t x, uint16_t y, uint16_t h, char color){
    return do_trap6(SYS_DRAWVLINE, fd, x, y, h, color, 0);
}
int drawHLine(int fd, int x, int y, int w, char color){
    return do_trap6(SYS_DRAWHLINE, fd, x, y, w, color, 0);
}
int drawLine(int fd, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, char color){
    return do_trap6(SYS_DRAWLINE, fd, x0, y0, x1, y1, color);
}
int drawCircle(int fd, uint16_t x0, uint16_t y0, uint16_t r, char color){
    return do_trap6(SYS_DRAWCIRCLE, fd, x0, y0, r, color, 0);
}
int drawCircleHelper(int fd, uint16_t x0, uint16_t y0, uint16_t r, unsigned char cornername, char color){
    return do_trap6(SYS_DRAWCIRCLEHELPER, fd, x0, y0, r, cornername, color);
}
int fillCircle(int fd, uint16_t x0, uint16_t y0, uint16_t r, char color){
    return do_trap6(SYS_FILLCIRCLE, fd, x0, y0, r, color, 0);
}
int fillCircleHelper(int fd, uint16_t x0, uint16_t y0, uint16_t r, unsigned char cornername, uint16_t delta, char color){
    return do_trap7(SYS_FILLCIRCLEHELPER, fd, x0, y0, r,cornername, delta, color);
}
int drawRoundRect(int fd, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, char color){
    return do_trap7(SYS_DRAWROUNDRECT, fd, x, y, w, h, r, color);
}
int fillRoundRect(int fd, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, char color){
    return do_trap7(SYS_FILLROUNDRECT, fd, x, y, w, h, r, color);
}
int fillRect(int fd, uint16_t x, uint16_t y, uint16_t w, uint16_t h, char color){
    return do_trap6(SYS_FILLRECT, fd, x, y, w, h, color);
}
/*
 *  FILE SYSTEM calls
 */
int fopen(int fd, char* path, BYTE mode) {
    return do_trap3(SYS_FOPEN, fd, (long)path,(long)mode);
}
int fclose(int fd) {
    return do_trap3(SYS_FCLOSE, fd, 0, 0);
}
int fread(int fd, void* buff, long btr, long* br) {
    return do_trap4(SYS_FREAD, fd, (long)buff, btr, (long)br);
}
/* lá no receptor
 case SYS_FREAD: {
    int   fd  = (int)a1;
    void* buf = (void*)a2;
    UINT  btr = (UINT)a3;
    UINT* br  = (UINT*)a4;
    ...
}
*/
int fwrite(int fd, void* buff, long btw, long* br) {
    return do_trap4(SYS_FWRITE, fd, (long)buff, btw, (long)br);
}
int fsync(int fd ) {
    return do_trap3(SYS_FSYNC, fd, 0, 0);
}
int funlink(int fd ) {
    return do_trap3(SYS_FUNLINK, fd, 0, 0);
}
int flseek(int fd , FSIZE_t offset) {
    return do_trap3(SYS_FUNLINK, fd, (long)offset, 0);
}
/* =========================================================================
 * 2. Posição, Tamanho e Status
 * ========================================================================= */

int ftell(int fd ) {
    return do_trap3(SYS_FTELL, fd, 0, 0);
}

int fsize(int fd ) {
    return do_trap3(SYS_FSIZE, fd, 0, 0);
}

int feof(int fd ) {
    return do_trap3(SYS_FEOF, fd, 0, 0);
}
/* =========================================================================
 * 3. Renomear e Metadados
 * ========================================================================= */
int frename(int fd, char* path_old, const char* path_new) {
    return do_trap3(SYS_FRENAME, fd, (long)path_old, (long)path_new);
}
int fstat(int fd, char* path, FILINFO* fno) {
    return do_trap3(SYS_FSTAT, fd, (long)path, (long)fno);
}
/* =========================================================================
 * 4. Diretórios
 * ========================================================================= */
int fopendir(int fd, DIR* dp, char* path) {
    return do_trap3(SYS_FOPENDIR, fd, (long)dp, (long)path);
}
int freaddir(int fd, DIR* dp, FILINFO* fno) {
    return do_trap3(SYS_FREADDIR, fd, (long)dp, (long)fno);
}
int fclosedir(int fd, DIR* dp) {
    return do_trap3(SYS_FCLOSEDIR, fd, (long)dp, 0);
}
int fmkdir(int fd, char* path) {
    return do_trap3(SYS_FMKDIR, fd, (long)path, 0);
}
/* =========================================================================
 * 5. Montagem e Desmontagem
 * ========================================================================= */
int fmount(FATFS* fs, const char* path, BYTE opt) {
    return do_trap3(SYS_FMOUNT, (long)fs, (long)path, opt);
}
int funmount(const char* path) {
    return do_trap3(SYS_FUNMOUNT, (long)path, 0, 0);
}
/* =========================================================================
 * 6. Procura de arquivos
 * ========================================================================= */

int findfirst(DIR* dp, FILINFO* fno, char* path, char* pattern) {
    return do_trap4(SYS_FFINDFIRST, (long) dp, (long) fno, (long) path, (long) pattern);
}

int findnext(DIR* dp, FILINFO* fno) {
    return do_trap3(SYS_FFINDNEXT, (long) dp, (long) fno, 0);
}
/*
 *  RTC calls
 */

void rtc_read_time(rtc_time_t *rrtc) {
    do_trap3(SYS_READRTC, (long) rrtc, 0, 0);
}
void rtc_write_time(rtc_time_t *wrtc) {
    do_trap3(SYS_WRITERTC, (long) wrtc, 0, 0);
}

