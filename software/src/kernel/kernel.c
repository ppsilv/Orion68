

#include <stdio.h>
#include <string.h>
#include <interrupt.h>
#include <critical.h>
#include "scheduler.h"
#include "./exceptions/exceptions.h"
#include <sys/vfs.h>

FATFS FatFs; 

/*
 * Teste isolado: duas tarefas bobas, cada uma so incrementando um
 * contador proprio e imprimindo de vez em quando. Se elas alternarem
 * no terminal, o context switch esta funcionando.
 *
 * Cada tarefa precisa da SUA PROPRIA pilha, alocada estaticamente
 * (sem malloc). O tamanho e' um chute razoavel pra uma tarefa simples
 * -- se sua tarefa usar buffers grandes na pilha (arrays locais, etc)
 * aumente esse valor. Estourar a pilha aqui e' silencioso (sem MMU),
 * entao se algo comportar estranho, suspeite disso primeiro.
 */
#define STACK_SIZE 1024

static uint8_t stack_task_a[STACK_SIZE];
static uint8_t stack_task_b[STACK_SIZE];
static uint8_t stack_task_c[STACK_SIZE];

void kernel_puts(const char *s);
void long_to_string(long value, char *str, int base);

static void TaskA(void)
{
    uint32_t count = 0;
    char str[32];
    for (;;) {
        count++;
        if (count >= 100000) {
            long_to_string(count, str,16);
            count = 0;
            kernel_puts("\nTask A: ");
            kernel_puts(str);
        }
    }
}

static int tty_fd = -1;

void TaskA_Entry(void)
{
    char str[16];
    long count = 0;
    long print_counter = 0;

    tty_fd = vfs_open("/dev/tty0", O_RDWR);
    if (tty_fd < 0) {
        kernel_puts("TaskC: falha ao abrir /dev/tty0\n");
        for (;;)
            ;   /* trava aqui mesmo, sem tty nao da pra nem avisar melhor */
    }

    for (;;) {
        count++;
        if (++print_counter >= 25000) {
            print_counter = 0;
            long_to_string(count, str, 10);
            vfs_write(tty_fd, "\nTask C: ", 9);
            vfs_write(tty_fd, str, strlen(str));
        }
    }
}
static void TaskB(void)
{
    uint32_t count = 0;
    char str[32];
    uint32_t roof=150000;
    for (;;) {
        count++;
        if (count >= roof) {
            roof-=1000;
            if(roof<=10000) roof = 50000;
            kernel_puts("\nTask B: ");
            long_to_string(count, str,10);
            kernel_puts(str);
            count = 0;
        }
    }
}

void run_scheduler_test(void)
{

    /* arg = NULL porque essas tarefas nao precisam de nenhum dado externo */
    OS_TaskCreate(TaskA, NULL, stack_task_a, STACK_SIZE, /*id=*/1);
    OS_TaskCreate(TaskB, NULL, stack_task_b, STACK_SIZE, /*id=*/2);
    OS_TaskCreate(TaskA_Entry, NULL, stack_task_c, STACK_SIZE, /*id=*/3);
    

    OS_Start();   /* nunca retorna -- a partir daqui quem manda e' o OS_TickISR */

    /* nunca chega aqui */
}

extern void run_scheduler_test(void);
extern uint32_t __kernel_end;   /* símbolo do seu linker script, fim do .bss do kernel */
extern uint32_t __estack; 
extern void kmalloc_init(void *base, uint32_t size);
extern void OS_TickISR();
extern void sys_setramvector(uint32_t stub_addr, uint32_t handler_addr);
void kernel_puts(const char *s);
extern void vfs_init(void);
extern int vfs_open(const char *path, int flags);


uint32_t systemTick=0;
uint32_t tick_count=0;
static void do_ideinit(int argc, char *argv[])
{
    FRESULT fr;
    fr = f_mount(&FatFs, "", 0);
    if (fr != FR_OK) {
        kputs("PANIC: Erro ao montar FAT\n");
    }else{
        kputs(": FAT success mounted!\n");
    }
}

int kernel_main(){
    kernel_puts("Kernel on line\n");
    short saved_sr;

    uint32_t heap_base = (uint32_t)&__kernel_end;
    uint32_t heap_size = __estack - heap_base - 0x4000;  /* 16 KB de folga pra pilha */
    kmalloc_init((void*)0x92000, 0x8000);

    kputs("    * IDE ");
    do_ideinit(0,NULL);

//    kmalloc_init(&__kernel_end, 0x10000);   /* ajuste o tamanho pro que sobrar de RAM ali */


    OS_Init();

    vfs_init();

    LOCK(saved_sr);
    sys_setramvector(0x00080090,(uint32_t)OS_TickISR);
    
    sys_setramvector(0x00080008, (uint32_t)exc_bus_error);
    sys_setramvector(0x0008000C, (uint32_t)exc_address_error);
    sys_setramvector(0x00080010, (uint32_t)exc_illegal);
    sys_setramvector(0x00080014, (uint32_t)exc_div0);
    sys_setramvector(0x00080020, (uint32_t)exc_priv);
    sys_setramvector(0x0008001C, (uint32_t)exc_trapv);
    sys_setramvector(0x00080028, (uint32_t)exc_linea);
    sys_setramvector(0x0008002C, (uint32_t)exc_linef);

    kernel_puts("1000 ");
    for(int i=0;i<0xFFFF; i++){
        volatile int j = 0; 
         (void)j;       
    }
    kernel_puts("1001 ");
    UNLOCK(saved_sr);
    kernel_puts("1002 ");

    run_scheduler_test();

    /*System never turn back from scheduler*/

    return 0;
}