#ifndef _PB_UART_H
#define _PB_UART_H

#include <avr/io.h>
#include <stdint.h>

#define baudrate(b) (F_CPU / 16 / b - 1)
#define baudrate2x(b) (F_CPU / 8 / b - 1)

void uart_init(uint8_t uart, uint16_t ubbr, uint8_t doublespeed);
void uart_write(uint8_t uart, uint8_t data);
uint8_t uart_read(uint8_t uart);

#endif
