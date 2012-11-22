
#ifndef helpers_h
#define helpers_h

/* constants */
#define ON 1
#define OFF 0

#define INPUT 1
#define OUTPUT 0

#define bit(a) (1 << (a))

#define LONG_DELAY 64000
#define SHORT_DELAY 32000


/* LCD Text constants */
#define LCDT_DELAY 5000
#define LCDT_LINE_SINGLE 0
#define LCDT_LINE_DOUBLE 1
#define LCDT_INSTRUCTION 0
#define LCDT_DATA        1
#define LCDT_BUS8        1
#define LCDT_TWOLINES    1
#define LCDT_ONELINE     0
#define LCDT_FONT        0

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
char read_sw();
/* turn buzzer on/off */
void buzzer(char on);
/* write leds */
void write_leds(char led);

/* change TRIS specific bits 
 usage update_tris(&TRISB, bit(0)|bit(1), OUT) */
void update_tris(unsigned int * tris, unsigned int mask, char direction);

/* selects the main mux as state*/
void select_mux1();

void busy_loop(unsigned long loops);

/* lcdt functions */
void lcdt_comm_setup();
void lcdt_comm_write(unsigned char comm_type, unsigned char data);
unsigned char lcdt_comm_read();
void lcdt_cls();
void lcdt_onoff(unsigned char onoff, unsigned char cursor, unsigned char blinking);
void lcdt_busy_loop();
void lcdt_function_set(unsigned char bus_size, unsigned char display_lines,
                      unsigned font_type);
void lcdt_begin(unsigned char lcdt_line_mode);

#endif