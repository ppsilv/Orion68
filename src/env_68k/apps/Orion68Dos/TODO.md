#TODO:
1 - Um meio de enviar arquivo para o OrionDOS 
    22-07-2026: O arquivo já está indo para 0x82000 falta agora
                gravar o programa no disco.
    18-07-2026: quase lá já consigo enviar um arquivo do pc para
                o picow via tcp-ip. Agora falta testar a interface
                entre o pico e o Orion68DOS.

2 - Desenhar a interface
    TRAP #XX------------------------------------------>
3 - Validar interruções.
    systick ------------------------------------------> OK
    serial teclado------------------------------------>
    SuperMultiIO-------------------------------------->
4 - Implementar wiznet-------------------------------> CANCELADO
5 - Implementar pi picoW:
    interface com o m68k------------------------------> OK
    leitura do keyboard USB--------------------------->
    leitura do keyboard PS2--------------------------->
    leitura sdcard------------------------------------>
6 - Validar a verificação de quantidade de memoria.
    total de memoria inicial 0x100000 ----------------> OK
    Inserido mais 0x100000 total 2Mb------------------>
7 - Implementar
    leitura ------------------------------------------>
    escrita no RTC------------------------------------>
