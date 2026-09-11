|
| crt0_kernel.s -- startup do kernel, roda como primeiro codigo em 0x82000
|
| Diferente do crt0 das aplicacoes (que nem existe -- main() e' chamada
| direto pelo loader): o kernel e' carregado como binario flat puro
| (load_kernel_flat + jmp 0x82000), entao precisa desse stub pra:
|   1. desligar interrupcao (systick nao pode disparar antes do
|      scheduler estar inicializado -- ver bug do current_task nulo)
|   2. setar o SP do kernel (__estack, do linker script)
|   3. zerar o .bss na mao (f_read cru NAO faz isso sozinho)
|   4. so entao chamar kernel_main
|
| NAO religa interrupcao aqui -- quem faz isso e' o OS_StartHighRdy,
| no primeiro rte da primeira task.
|

.global _start
.extern kernel_main
.extern __bss_start
.extern __bss_end
.extern __estack

.text

_start:
    move.w  #0x2700,%sr        | desliga todas as interrupcoes
    lea     __estack,%sp       | seta o SP do kernel

    lea     __bss_start,%a0
    lea     __bss_end,%a1
.bss_clear_loop:
    cmp.l   %a1,%a0
    beq.s   .bss_clear_done
    clr.b   (%a0)+
    bra.s   .bss_clear_loop
.bss_clear_done:

    jsr     kernel_main

.halt:
    bra.s   .halt               | kernel_main nao deveria retornar -- rede de seguranca