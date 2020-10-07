#include <defs.h>
#include <arch.h>
#include <stdio.h>
#include <string.h>
#include <picirq.h>
#include <intr.h>
#include <trap.h>
#include <memlayout.h>
#include <sync.h>

void bluetooth_init()
{
	kprintf("initbluetooth\n\r");
	//*WRITE_IO(UART_BASE + 8)= *READ_IO(UART_BASE + 8) | 0x00000001;
	delay();
    *WRITE_IO(UART_BASE + 12) = 0x00000080;
    delay();
	*WRITE_IO(UART_BASE + 0) = 69; 
	delay();
	*WRITE_IO(UART_BASE + 4) = 1;
    delay();
	*WRITE_IO(UART_BASE + 12) = 0x00000003;
    delay();
	*WRITE_IO(UART_BASE + 4)= 0x00000001;
    delay();
}