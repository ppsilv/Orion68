#TODO:
1 - Um meio de enviar arquivo para o OrionDOS 
    18-07-2026: quase lá já consigo enviar um arquivo do pc para
                o picow via tcp-ip. Agora falta testar a interface
                entre o pico e o Orion68DOS.

2 - Desenhar a interface TRAP #XX
3 - Implementar a nova forma de teclado PS2
4 - Implementar wiznet
5 - Implementar pi pico:
    interface com o m68k
    leitura do keyboard USB
    leitura do keyboard PS2
    leitura sdcard
6 - Validar a verificação de quantidade de memoria
7 - 




Rascunhos

0xFF9100 + 1 = DATA REGISTER
0xFF9100 + 3 = STATUS RESGISTER
0xFF9100 + 5 = SIZE HIGH BYTE
0xFF9100 + 7 = SIZE LOW BYTE