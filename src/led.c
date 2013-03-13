#include <pb/led.h>

void led_init(void)
{
    DDRB |= (1<<4)|(1<<5)|(1<<6);
    PORTB |= (1<<6); // turn off inversed led
    PORTB &= ~((1<<4)|(1<<5)); // turn off all leds
}

void led_on(uint8_t led)
{
    if(led < 2)
        PORTB |= (1<<(led+4));
    else if(led == 2)
        PORTB &= ~(1<<6);
}

void led_off(uint8_t led)
{
    if(led < 2)
        PORTB &= ~(1<<(led+4));
    else if(led == 2)
        PORTB |= (1<<6);
}

uint8_t led_state(uint8_t led)
{
    uint8_t state = PINB & (1<<(4+led));
    if(led < 2)
        return state;
    else if(led == 2)
        return ~state & 1;
    else
        return 0;
}

void led_toggle(uint8_t led)
{
    if(led_state(led))
        led_off(led);
    else
        led_on(led);
}
