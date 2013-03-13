/**
 * @file pb/led.h
 * @brief Cerebellum Periph Board LED library
 * @author WebConn
 * @date 23-Feb-2013
 */

#ifndef PB_LED
#define PB_LED

#include <avr/io.h>
#include <stdint.h>

void led_init(void);
void led_on(uint8_t led);
void led_off(uint8_t led);
void led_toggle(uint8_t led);
uint8_t led_state(uint8_t led);


#endif
