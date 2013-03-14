#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <pb/servo.h>
#include <pb/led.h>

int main(void)
{
    led_init();
    servo_init();
    
    sei();

    servo first = servo_add(&PORTA, 0);
    servo second = servo_add(&PORTA, 1);
    
    servo third = servo_add(&PORTA, 2);
    servo fourth = servo_add(&PORTA, 3);
    servo fifth = servo_add(&PORTA, 4);

    servo_write(third, 30);
    servo_write(fourth, 150);
    servo_write(fifth, 200);
    
    while(1)
    {
        led_on(1);
        uint8_t i;
        for(i=5; i<=240; i+=5)
        {
            servo_write(first, i);
            servo_write(second, 245-i);
            _delay_ms(30);
        }
        led_off(1);
        for(i=235; i>=5; i-=5)
        {
            servo_write(first, i);
            servo_write(second, 245-i); 
            _delay_ms(30);
        }
    }
    while(1);;;

    return 0;
}
