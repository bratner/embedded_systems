
#ifndef helpers_h
#define helpers_h

/* constants */
#define ON 1
#define OFF 0
#define bit(a) (1 << (a))
/* types */

union switch_u {
    struct{
        unsigned char s0:1;
        unsigned char s1:1;
        unsigned char s2:1;
        unsigned char s3:1;
        unsigned char s4:1;
        unsigned char s5:1;
        unsigned char s6:1;
        unsigned char s7:1;
    };
    unsigned char all;
};

typedef union switch_u switches;

/* All functions assume RF8 set to 0 to enable the main MUX */
/* Trigger RD4 */
void trigger();
/* read switches */
int read_sw();
/* turn buzzer on/off */
void buzzer(char on);
/* write leds */
void write_leds(char led);

#endif