#ifndef __DRVUART_H__
#define __DRVUART_H__

#define DRV_UART0_BASE 0xFF4000
#define RHR0   (*((volatile unsigned char *)(DRV_UART0_BASE + 0x01)))   //receive holding register (read)
#define THR0   (*((volatile unsigned char *)(DRV_UART0_BASE + 0x01)))   //transmit holding register (write)
#define IER0   (*((volatile unsigned char *)(DRV_UART0_BASE + 0x03)))   //interrupt enable register
#define ISR0   (*((volatile unsigned char *)(DRV_UART0_BASE + 0x05)))   //interrupt status register (read)
#define FCR0   (*((volatile unsigned char *)(DRV_UART0_BASE + 0x05)))   //FIFO control register (write)
#define LCR0   (*((volatile unsigned char *)(DRV_UART0_BASE + 0x07)))   //line control register
#define MCR0   (*((volatile unsigned char *)(DRV_UART0_BASE + 0x09)))   //modem control register
#define LSR0   (*((volatile unsigned char *)(DRV_UART0_BASE + 0x0B)))   //line status register
#define MSR0   (*((volatile unsigned char *)(DRV_UART0_BASE + 0x0D)))   //modem status register
#define SPR0   (*((volatile unsigned char *)(DRV_UART0_BASE + 0x0F)))   //scratchpad register (reserved for system use)
#define DLL0   (*((volatile unsigned char *)(DRV_UART0_BASE + 0x01)))   //divisor latch LSB
#define DLM0   (*((volatile unsigned char *)(DRV_UART0_BASE + 0x03)))   //divisor latch MSB
//aliases for register names (used by different manufacturers)cd ..       
#define RBR0   (*((volatile unsigned char *)(DRV_UART0_BASE + 0x01)))   //receive buffer register
#define IIR0   (*((volatile unsigned char *)(DRV_UART0_BASE + 0x05)))   //interrupt identification register
#define SCR0   (*((volatile unsigned char *)(DRV_UART0_BASE + 0x0F)))   //scratch register

#define DRV_UART1_BASE 0xFF4100
#define RHR1   (*((volatile unsigned char *)(DRV_UART1_BASE + 0x01)))   //receive holding register (read)
#define THR1   (*((volatile unsigned char *)(DRV_UART1_BASE + 0x01)))   //transmit holding register (write)
#define IER1   (*((volatile unsigned char *)(DRV_UART1_BASE + 0x03)))   //interrupt enable register
#define ISR1   (*((volatile unsigned char *)(DRV_UART1_BASE + 0x05)))   //interrupt status register (read)
#define FCR1   (*((volatile unsigned char *)(DRV_UART1_BASE + 0x05)))   //FIFO control register (write)
#define LCR1   (*((volatile unsigned char *)(DRV_UART1_BASE + 0x07)))   //line control register
#define MCR1   (*((volatile unsigned char *)(DRV_UART1_BASE + 0x09)))   //modem control register
#define LSR1   (*((volatile unsigned char *)(DRV_UART1_BASE + 0x0B)))   //line status register
#define MSR1   (*((volatile unsigned char *)(DRV_UART1_BASE + 0x0D)))   //modem status register
#define SPR1   (*((volatile unsigned char *)(DRV_UART1_BASE + 0x0F)))   //scratchpad register (reserved for system use)
#define DLL1   (*((volatile unsigned char *)(DRV_UART1_BASE + 0x01)))   //divisor latch LSB
#define DLM1   (*((volatile unsigned char *)(DRV_UART1_BASE + 0x03)))   //divisor latch MSB
//aliases for register names (used by different manufacturers)cd   ..
#define RBR1   (*((volatile unsigned char *)(DRV_UART1_BASE + 0x01)))   //receive buffer register
#define IIR1   (*((volatile unsigned char *)(DRV_UART1_BASE + 0x05)))   //interrupt identification register
#define SCR1   (*((volatile unsigned char *)(DRV_UART1_BASE + 0x0F)))   //scratch register

#define DRV_UART2_BASE 0xFF4200
#define RHR2   (*((volatile unsigned char *)(DRV_UART2_BASE + 0x01)))   //receive holding register (read)
#define THR2   (*((volatile unsigned char *)(DRV_UART2_BASE + 0x01)))   //transmit holding register (write)
#define IER2   (*((volatile unsigned char *)(DRV_UART2_BASE + 0x03)))   //interrupt enable register
#define ISR2   (*((volatile unsigned char *)(DRV_UART2_BASE + 0x05)))   //interrupt status register (read)
#define FCR2   (*((volatile unsigned char *)(DRV_UART2_BASE + 0x05)))   //FIFO control register (write)
#define LCR2   (*((volatile unsigned char *)(DRV_UART2_BASE + 0x07)))   //line control register
#define MCR2   (*((volatile unsigned char *)(DRV_UART2_BASE + 0x09)))   //modem control register
#define LSR2   (*((volatile unsigned char *)(DRV_UART2_BASE + 0x0B)))   //line status register
#define MSR2   (*((volatile unsigned char *)(DRV_UART2_BASE + 0x0D)))   //modem status register
#define SPR2   (*((volatile unsigned char *)(DRV_UART2_BASE + 0x0F)))   //scratchpad register (reserved for system use)
#define DLL2   (*((volatile unsigned char *)(DRV_UART2_BASE + 0x01)))   //divisor latch LSB
#define DLM2   (*((volatile unsigned char *)(DRV_UART2_BASE + 0x03)))   //divisor latch MSB
//aliases for register names (used by different manufacturers)cd   ..
#define RBR2   (*((volatile unsigned char *)(DRV_UART2_BASE + 0x01)))   //receive buffer register
#define IIR2   (*((volatile unsigned char *)(DRV_UART2_BASE + 0x05)))   //interrupt identification register
#define SCR2   (*((volatile unsigned char *)(DRV_UART2_BASE + 0x0F)))   //scratch register

#define DRV_UART3_BASE 0xFF4300
#define RHR3   (*((volatile unsigned char *)(DRV_UART3_BASE + 0x01)))   //receive holding register (read)
#define THR3   (*((volatile unsigned char *)(DRV_UART3_BASE + 0x01)))   //transmit holding register (write)
#define IER3   (*((volatile unsigned char *)(DRV_UART3_BASE + 0x03)))   //interrupt enable register
#define ISR3   (*((volatile unsigned char *)(DRV_UART3_BASE + 0x05)))   //interrupt status register (read)
#define FCR3   (*((volatile unsigned char *)(DRV_UART3_BASE + 0x05)))   //FIFO control register (write)
#define LCR3   (*((volatile unsigned char *)(DRV_UART3_BASE + 0x07)))   //line control register
#define MCR3   (*((volatile unsigned char *)(DRV_UART3_BASE + 0x09)))   //modem control register
#define LSR3   (*((volatile unsigned char *)(DRV_UART3_BASE + 0x0B)))   //line status register
#define MSR3   (*((volatile unsigned char *)(DRV_UART3_BASE + 0x0D)))   //modem status register
#define SPR3   (*((volatile unsigned char *)(DRV_UART3_BASE + 0x0F)))   //scratchpad register (reserved for system use)
#define DLL3   (*((volatile unsigned char *)(DRV_UART3_BASE + 0x01)))   //divisor latch LSB
#define DLM3   (*((volatile unsigned char *)(DRV_UART3_BASE + 0x03)))   //divisor latch MSB
//aliases fOr register names (used by different manufacturers)cd   ..
#define RBR3   (*((volatile unsigned char *)(DRV_UART3_BASE + 0x01)))   //receive buffer register
#define IIR3   (*((volatile unsigned char *)(DRV_UART3_BASE + 0x05)))   //interrupt identification register
#define SCR3   (*((volatile unsigned char *)(DRV_UART3_BASE + 0x0F)))   //scratch register

void uart_init(void);
void uart_putchar(unsigned char ch);

void uart0_init();
unsigned char uart0_read();
void uart0_write(unsigned char ch);
int _inbyte(int delay);
void _outbyte(unsigned char ch);

#endif