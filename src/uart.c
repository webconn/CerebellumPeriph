#include <pb/uart.h>

void uart_init(uint8_t uart, uint16_t ubrr, uint8_t doublespeed)
{
    // Select UART
    if(uart)    // UART1
    {
        // 1. Set baudrate
        UBRR1H = (uint8_t) ubrr >> 8;
        UBRR1L = (uint8_t) ubrr & 0xFF;

        // 1.1. If user wants to use DoubleSpeed, set it
        if(doublespeed)
            UCSR1A |= (1<<U2X1);

        // 2. Enable Rx and Tx
        UCSR1B = (1<<RXEN1)|(1<<TXEN1);
        
        // 3. Frame format: 8 bit, 1 stop-bit, no parity
        UCSR1C = (3<<UCSZ10);
    }
    else        // UART0
    {
        // 1. Set baudrate
        UBRR0H = (uint8_t) ubrr >> 8;
        UBRR0L = (uint8_t) ubrr & 0xFF;

        // 1.1. If user wants to use DoubleSpeed, set it
        if(doublespeed)
            UCSR1A |= (1<<U2X1);

        // 2. Enable Rx and Tx
        UCSR0B = (1<<RXEN0)|(1<<TXEN0);
        
        // 3. Frame format: 8 bit, 1 stop-bit, no parity
        UCSR0C = (3<<UCSZ00);
    }
}

void uart_write(uint8_t uart, uint8_t data)
{
    if(uart)    // UART1
    {
        // 1. Wait until we could write something
        while(!(UCSR1A & (1<<UDRE1)));

        // 2. Send data
        UDR1 = data;
    }
    else
    {
        // 1. Wait until we could write something
        while(!(UCSR0A & (1<<UDRE0)));

        // 2. Send data
        UDR0 = data;
    }
}

uint8_t uart_read(uint8_t uart)
{
    if(uart)
    {
        // 1. Wait until we could write something
        while(!(UCSR1A & (1<<RXC1)));

        // 2. Send data
        return UDR1;
    }
    else
    {
        // 1. Wait until we could write something
        while(!(UCSR0A & (1<<RXC0)));

        // 2. Send data
        return UDR0;
    }
}
