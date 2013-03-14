#include <pb/servo.h>
#include <pb/led.h>

// Main servos queue
volatile servo_t queue[MAX_SERVOS];

// Sorted servos queue
volatile servo_t * p_queue[MAX_SERVOS];

// Current queue length
volatile uint8_t queue_len = 0;

// Number of items to remove
volatile uint8_t num_add = 0;

// Current compared item
volatile uint8_t curItem = 0;

// Flags
volatile struct s_flags {
    unsigned update : 1;
    unsigned active : 1;
} servo_flags;

// Inline functions
void _servo_sort(void);
inline void _servo_timerOverflow(void);
inline void _servo_setPassive(void);
inline void _servo_setActive(void);
inline void _servo_timerCompare(void);
inline void _servo_switch(void);
inline void _servo_prepare(void);

/*
 * Boot-up init of servos
 */
void servo_init(void)
{
    /*
     * Servo library uses 8-bit Timer0 for managing
     * many servos together. Servo could be connected to any
     * pin without problems (except to ULN pins because they
     * have no pull-up, and PORTF pins).
     *
     * Servo queue stored in array of structures, contains:
     *  - port
     *  - pinmask
     *  - PWM value
     *
     * Array should be sorted on-the-fly, when we add new item - just move it forward by 1 value
     */

    // 1. Init timer
    TIMSK |= (1<<OCIE0)|(1<<TOIE0); // enable both interrupts 
    
    // 2. Switch to passive mode - wait for first servos
}

/**
 * Register new servo in queue
 */
servo servo_add(volatile uint8_t * port, uint8_t pin)
{
    uint8_t new_id = queue_len + num_add;
    
    // 0. Check queue size - if overloaded, ignore this call
    if(new_id == MAX_SERVOS)
        return SERVO_QUEUE_FULL; // error code

    // 1. Init port DDR (if port is correct and is not PORTF, DDR reg is next by pointer
    *(port + 1) |= (1<<pin);

    // 2. Insert description into table
    queue[new_id].port = port;
    queue[new_id].bitmask = (1<<pin);
    queue[new_id].value = 20; // servo-safe, I hope

    // 3. Add servo to the end of real queue
    p_queue[new_id] = &queue[new_id];
    servo_flags.update = 1;

    // 4. Check if we add the first servo - only now we should turn on the timer
    if(new_id == 0)
        _servo_setPassive();

    // 5. Set increaser
    num_add++;

    // 6. Return servo ID
    return new_id; // and auto-increment
}

/**
 * Set new value to the servo
 */
void servo_write(servo servo_id, uint8_t value)
{
    if(value == 0) value = 1;
    queue[servo_id].value = value;
    _servo_sort();
}

/**
 * Sort servo array
 */
void _servo_sort(void)
{
    // In this function we just need to sort servo queue, according to number of items
    uint8_t a, b;
    
    for(a=0; a < queue_len; a++)
    {
        uint8_t limit = queue_len - a;
        for(b=0; b < limit; b++)
        {
            // While sorting, we modify array of pointers, so
            // Let's compare params
            if(p_queue[b]->value > p_queue[b+1]->value)
            {
                volatile servo_t * temp = p_queue[b];
                p_queue[b] = p_queue[b+1];
                p_queue[b+1] = temp;
            }
        }
    }
}

inline void _servo_prepare(void)
{
    if(num_add > 0)
    {
        queue_len += num_add;
        num_add = 0;
    }

    // 1. Update queue if required
    if(servo_flags.update)
    {
        _servo_sort();
        servo_flags.update = 0;
    }

    // 2. Set logical "1" on all servo outputs
    uint8_t i;
    for(i=0; i<queue_len; i++)
        *(queue[i].port) |= queue[i].bitmask;

    // Now we are waiting for timer overflow to run in active mode
}

/**
 * Switch to active mode
 */
inline void _servo_setActive(void)
{
    servo_flags.active = 1;

    // Configure timer
    // Prescaler = F_CPU / 128
    TCCR0 &= ~(7 << CS00); // clear old value
    TCCR0 |= (5 << CS00);

    // Set 1st output compare and pointer to compare
    OCR0 = p_queue[0]->value;
    curItem = 0;
}

/**
 * Switch to passive mode
 */
inline void _servo_setPassive(void)
{
    servo_flags.active = 0;

    // Configure timer
    // Prescaler = F_CPU / 1024
    // Value to turn all servo on = 245
    // It means minimal pulse 0.6 ms
    TCCR0 |= (7 << CS00);

    OCR0 = 247;
}

/**
 * Servo switcher
 */
inline void _servo_switch(void)
{
    // Turn off all current servos
    do
    {
        *(p_queue[curItem]->port) &= ~(p_queue[curItem]->bitmask);
        curItem++;
        if(curItem == queue_len) return; // we reached end of queue
    }
    while(p_queue[curItem]->value == OCR0);

    // Here we have an index of next servo item
    // Set OCR0 value to next servo
    OCR0 = p_queue[curItem]->value;
}

/**
 * Servo timer overflow
 */
inline void _servo_timerOverflow(void)
{
    // 1. Check if we have some items in queue
    if(queue_len > 0)
    {
        // Check mode of operation
        if(servo_flags.active) // if we were in active mode
            _servo_setPassive(); // switch to passive
        else                   // if we were in passive mode
        {
            _servo_setActive();  // switch to active
        }
    }
    // 2. If there are no items, let's try to get 'em
    else if(num_add > 0) 
    {
        _servo_prepare();
        _servo_setActive();
    }
    // 3. If there is nothing to add, stay in passive for saving resourses
    else
    {
        _servo_setPassive();
    }
    TCNT0 = 0; // clear timer
}

/**
 * Servo timer output compare
 */
inline void _servo_timerCompare(void)
{
    // Check mode of operation
    if(servo_flags.active) // in active mode, change levels
    {
        _servo_switch();
    }
    else                   // in passive mode, it could be only 235 interrupt
        _servo_prepare();  // prepare servos to run
}

// BIND FUNCTIONS INTO INTERRUPTS
ISR(TIMER0_COMP_vect)
{
    _servo_timerCompare();
}

ISR(TIMER0_OVF_vect)
{
    _servo_timerOverflow();
}
