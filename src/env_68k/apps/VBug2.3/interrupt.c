#include "timers.h"
#include "interrupt.h"

/*
+--------------------+-----------------+--------------------------+--------------------------------------------------------+
|Nível de Interrupção| Prioridade      | Uso Típico no 68K        |    Por que essa posição?                               |
+--------------------+-----------------+--------------------------+--------------------------------------------------------+
|Nível 7             | Altíssima (NMI) | Botão de Abort           |    Não pode ser desabilitada.                          |
|                    |                 | Queda de Energia         |                                                        | 
+--------------------+-----------------+--------------------------+--------------------------------------------------------+
|Nível 5 ou 6        | Alta            | Controladora de Disquete |    O fluxo de dados do disco não pode esperar,         |
|                    |                 | (WD2793) / Rede          |    ou o setor passa e o dado é perdido.                |
+--------------------+-----------------+--------------------------+--------------------------------------------------------+
|Nível 3 ou 4        | Média           | Porta Serial / UART      |    A UART tem buffers pequenos (FIFO). Precisa         |
|                    |                 | (16C554 / Z8530)         |    ser atendida rápido para não dar Overflow.          |
+--------------------+-----------------+--------------------------+--------------------------------------------------------+
|Nível 1 ou 2        | Baixa           | System Tick              |    Se o Tick atrasar alguns microssegundos para dar    |
|                    |                 | (Timer de 5ms/10ms)      |    passagem à Serial,ninguém morre,nenhum dado perdido.|
+--------------------+-----------------+--------------------------+--------------------------------------------------------+

+----------------------------+----------------------------------+------------+---------------------+
|   Function                 |   Memory address                 |  Interrupt |        I/O          | 
+----------------------------+----------------------------------+------------+---------------------+
| #define ISR_VECT_EXP1(vec) |     (*((long *)0x64) = (long)vec)|      1     |        free         |
+----------------------------+----------------------------------+------------+---------------------+
| #define ISR_VECT_IDE(vec)  |     (*((long *)0x68) = (long)vec)|      2     |        IDE          |
+----------------------------+----------------------------------+------------+---------------------+
| #define ISR_VECT_PIT(vec)  |     (*((long *)0x6C) = (long)vec)|      3     |        tick         |
+----------------------------+----------------------------------+------------+---------------------+
| #define ISR_VECT_DUART(vec)|     (*((long *)0x70) = (long)vec)|      4     |        uart         |
+----------------------------+----------------------------------+------------+---------------------+
| #define ISR_VECT_PS2(vec)  |     (*((long *)0x74) = (long)vec)|      5     |     keyboard        |
+----------------------------+----------------------------------+------------+---------------------+
| #define ISR_VECT_EXP2(vec) |     (*((long *)0x78) = (long)vec)|      6     |        free         |
+----------------------------+----------------------------------+------------+---------------------+
| #define ISR_VECT_EXP3(vec) |     (*((long *)0x7C) = (long)vec)|      7     |        free         |
+----------------------------+----------------------------------+------------+---------------------+

Vou trocar IDE com tick, atualmente tick já está na int 2

1. Elas convivem juntas?
Sim, absolutamente. Elas não apenas convivem, como são projetadas para coexistir no mesmo ecossistema 
no m68k.

Autovector (níveis 1-7): O hardware (CPU) responde automaticamente a um sinal de interrupção (IRQ) em 
um dos pinos de nível (IPL0, IPL1, IPL2) e busca o vetor de interrupção em uma área fixa da tabela de 
vetores (endereços $64 a $7B). É a forma simples e rápida de tratar interrupções internas ou  perifé-
ricos simples.

Vectored (Não Autovector): O  dispositivo periférico  é quem  fornece o número do  vetor (de 0 a 255) 
durante o ciclo de reconhecimento de interrupção (Interrupt Acknowledge Cycle). Isso permite que você 
tenha centenas de fontes de interrupção diferentes usando o mesmo barramento.

Você  pode  ter  periféricos  simples usando o a utovector para economizar pinos/lógica e periféricos 
complexos (como controladores de rede  ou  comunicação)  usando  interrupções vectored para gerenciar 
múltiplos estados de forma eficiente.

2. Como funcionam as interrupções não-autovector (Vectored)?
O processo é um "diálogo" entre o periférico e a CPU através do barramento:

Sinalização: O periférico puxa as linhas IPL para um nível de prioridade (ex: 3).
                                                                                                    |
Reconhecimento (IACK): Quando a CPU decide aceitar a interrupção, ela inicia um ciclo  de  barramento 
especial chamado Interrupt Acknowledge Cycle. Ela coloca o nível da  interrupção  (3, neste caso) nas 
linhas de endereço A1-A3 e sinaliza FC0-FC2 como 111 (espaço de endereço de CPU).

Fornecimento  do  Vetor:  O dispositivo  periférico  vê  esse  ciclo,  sabe que a CPU quer saber quem 
interrompeu, e coloca um número de 8 bits (o vetor) no barramento de dados (D0-D7).

Ação da CPU:  A  CPU  lê esse byte  (ex: $40), multiplica por 4 para calcular o endereço na tabela de 
vetores (ex: $40 * 4 = $100), busca o endereço do manipulador (ISR) ali contido  e faz o salto para o 
código.

O protocolo no seu RP2040 (PIO) seria:

Estado de Espera: O PIO monitora os pinos FC0-FC2 e AS.
                                                                                                    | 
Detecção de IACK: Se FC0-FC2 == 111 e AS for ativado (nível baixo), o PIO sabe que o  68k  entrou  no 
modo de reconhecimento.

Ação IACK:
Se for Autovector: O PIO apenas libera as linhas IPL (coloca em 1).
Se for Vetorado: O PIO coloca o byte do vetor no barramento  de dados (D0-D7) e,  simultaneamente  ou 
logo após, libera as linhas IPL (1).
Finalização: O 68k terminará o ciclo IACK (desativando o AS). O  seu  PIO  volta a monitorar o estado 
normal.


O fluxo refinado no seu PIO:

Monitoramento: O PIO observa FC0-FC2 e /AS.

Detecção do IACK: 
Quando /AS desce e FC == 7 (111), o 68k está pedindo o vetor.

Ação de Escrita:
O PIO coloca o byte do vetor no barramento D0-D7.
O PIO sinaliza que o dado está pronto (gerando /DTACK, caso necessário).

Liberação (Reset):
Assim que o PIO detectar a subida de /AS (o fim do ciclo), ele coloca o barramento D0-D7 em Hi-Z tri-
state).
Simultaneamente (ou logo após a subida de /AS), ele sobe as linhas IPL0-IPL2 para 1.
Exatamente. O ponto crucial é garantir que a CPU já tenha capturado a requisição de interrupção.
Assim que o sinal de IACK é validado (ou seja, FC0-FC2 estão em 111 e o /AS desceu), a CPU internamen-
te trava aquele nível de prioridade para processamento. A partir desse instante,  a manutenção física 
das linhas IPL em nível baixo (0) torna-se redundante e, na verdade, perigosa.

Por que você tem essa liberdade?
O "Latching" interno: O 68000 possui um "latched priority interrupt" interno. Uma vez que o  ciclo  de 
IACK começa, ele não olha mais para as linhas IPL para  decidir  o que  fazer  com a quela interrupção 
específica. Ele já "assumiu" o nível de prioridade.

O perigo do atraso: Se você mantiver as linhas IPL em 0 por muito tempo (após o início do IACK), a CPU 
pode, ao finalizar o ciclo, re-amostrar as linhas IPL e,se ainda estiverem em 0,disparar um novo ciclo 
de interrupção idêntico, criando um loop infinito de IACK (conhecido como spurious interrupt).

A janela de oportunidade
O momento ideal para elevar as linhas  IPL  para  1 é  imediatamente após a detecção do IACK, e não há 
necessidade de esperar o /AS subir.

No modo Autovector: Você pode subir as linhas IPL assim que o IACK for detectado no PIO.

No modo Vetorado: Como você precisa fornecer o vetor no barramento de dados (D0-D7), o mais  prudente é 
subir as linhas IPL simultaneamente ao colocar o dado no barramento ou logo após garantir que a CPU leu 
o dado (gerando o /DTACK).

Resumo:Pode subir as linhas IPL logo na detecção do IACK.Isso mantém seu sistema limpo, evita interrup-
ções espúrias e garante que o processador termine o tratamento atual sem ser "atropelado"  por uma  re-
amostra errônea do hardware.

Você já tem a lógica de tri-state do barramento de dados implementada no seu código PIO? Isso é a parte 
onde o conflito de barramento costuma ocorrer se o timing for muito agressivo.


Como o 68000 original faz o Autovector então?
No 68000, o processo é "hard-wired" na lógica de barramento:
A CPU coloca FC0-FC2 = 111 e o nível da interrupção em A1-A3.
Para o 68000 saber que deve usar o Autovector, o hardware externo (a lógica que você está montando) deve 
sinalizar isso através do pino /VPA (Valid Peripheral Address), e não /AVEC.

Quando a CPU vê o /VPA ser puxado para baixo durante o ciclo de reconhecimento de interrupção (IACK),ela 
entende: "Opa, este é um periférico 'lento' (estilo família 6800) ou um caso de autovector, vouignorar o 
barramento de dados e usar o vetor interno pré-definido para este nível IPL".

Resumo para o seu projeto:
Se quiser Autovector no 68000: Você deve acionar o /VPA (Valid Peripheral Address) durante o ciclo IACK.
Se quiser Vetorado no 68000: Você não aciona o /VPA. Você coloca o número do vetor no barramento de dados 
e sinaliza com o /DTACK.

*/

