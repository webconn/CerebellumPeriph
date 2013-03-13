/*
 * @file include/pb/servo.h
 * @brief Headers for servo control library
 * @author WebConn 
 * @date 23-Feb-2013
 */

#ifndef PB_SERVO
#define PB_SERVO

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

// Queue size
#define MAX_SERVOS 16

// Error code
#define SERVO_QUEUE_FULL 255

// Queue item typedef
typedef struct {
    volatile uint8_t * port;
    uint8_t bitmask;
    uint8_t value;
} servo_t;

typedef uint8_t servo;

// Functions
void servo_init(void);
servo servo_add(volatile uint8_t * port, uint8_t pin);
void servo_remove(servo servo_id);
void servo_write(servo servo_id, uint8_t value);

#endif
