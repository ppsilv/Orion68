#include <stdio.h>
#include <stdlib.h>
#include <mc68000.h>



void delay(unsigned int time) {
    for (volatile unsigned int i = 0; i < time; i++);
}

// Função para verificar stack
void check_stack(void) {
    unsigned long stack_val;
    asm volatile (
        "move.l %%sp, %0\n\t"
        : "=r" (stack_val)
    );
    printf("Stack pointer: 0x%08X\n", stack_val);
}

unsigned char get_key();
void init_kbd();
void main1();
void show_menu(){
    int choice;
    unsigned char ch;
    choice = 'A';
    while (choice != 1000){
        printf("\n1 - getkey from keyboard\n");
        printf("0 - sai do programa\n");
        printf("\nEscolha: ");
        choice = getchar();
        printf("choice [%02x]\n",choice);
        switch(choice){
            case '1':
                printf("Getting a keyboard key\n");
                ch = get_key();
                printf("Key [%c] ",ch);
                break;
            case '2':
                main1();
                break;
            case '0':
                return;

                break;
        }
    }
}


int main() {
    check_stack();  // ✅ Verificar stack no início
    init_kbd();

    printf("Vou testar a leitura de teclado com ch9350 \n");
    show_menu();

    return 0;
}


