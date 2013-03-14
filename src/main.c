#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <pb/servo.h>
#include <pb/led.h>
#include <pb/uart.h>

int main(void)
{
    led_init();
    servo_init();
    uart_init(1, baudrate2x(57600), 1);
    
    sei();

    servo servos[5];

    servos[0] = servo_add(&PORTA, 4);
    servos[1] = servo_add(&PORTA, 5);
    servos[2] = servo_add(&PORTA, 6);
    servos[3] = servo_add(&PORTA, 7);
    servos[4] = servo_add(&PORTG, 2);
    
    while(1)
    {
        led_off(2);
        // We are waiting for 1st incoming byte - mode
        uint8_t byte = uart_read(1);
        if(byte == 's') // servo control
        {
            led_on(2);
            // Wait for 2nd byte - number of periph
            byte = uart_read(1);
            uint8_t index;
            if(byte > 0 && byte < 6)
                index = byte - 1;
            else // wrong value
            {
                // wait for 3rd byte
                byte = uart_read(1);
                continue;
            }

            // Wait for 3rd byte - value
            byte = uart_read(1);
            servo_write(servos[index], byte);
            continue;
        }
    }

    return 0;
}
