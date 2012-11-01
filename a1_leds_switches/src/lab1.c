#include <p32xxxx.h>
#include "include/helpers.h"
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8

#define LONG_DELAY 64000
#define SHORT_DELAY 32000

void init() {
    TRISF = 0; /* main MUX control bits are output*/
    PORTFbits.RF8 = 0; /* enable the main MUX */
    TRISD = 0; /* BUS D used for triggers */

}

void delay(unsigned int delay){
    int i;
    switches sw,oldsw;
    sw.all = read_sw();
    oldsw = sw;
    for(i=0; i<delay; i++){
        sw.all = read_sw();
        if(sw.all != oldsw.all)
            break; /* break the loop if someone touched the switches */
        else
            oldsw = sw;
    }

}


int main() {        
    char buzz = 0;
    unsigned char leds = 0;
    unsigned char stage = 0;
    /* array to do the "fan" assignment */
    unsigned char fan[] = { bit(3)|bit(4), bit(2)|bit(5),
                               bit(1)|bit(6), bit(0)|bit(7) };
    switches sw = {0}, prevsw = {0};
    init();

    while (1) {
        prevsw = sw; /* set the previous switches aside */
        sw.all = read_sw(); /* read switches */
        
        /* if function changed - start new function from scratch */
        /*     if( (sw.all & 7) != (prevsw.all & 7))
            stage = 0; */

        /* switch 5 stops all motion*/
        if(sw.s5)
            continue;

        /* switch logic */
        /* switch s2 does the fan thing */
        if (sw.s2){

                stage = stage % 4;
                leds = fan[stage];

        /* switch s1 does the shift bit thing */
        } else if(sw.s1) {

            stage = stage % 8;
            leds = 1 << stage;

        /* switch s0 does the counter */
        } else if(sw.s0) {

            leds = stage;
        }

        /* setup the next stage */
        if(sw.s3)
            stage--;
        else
            stage++;

        
        if (sw.s6) {
            buzz = ~buzz;
            buzzer(buzz);
        } else {
            buzzer(OFF);
        }

        write_leds(leds);

        if (sw.s4)
            delay(LONG_DELAY);
        else
            delay(SHORT_DELAY);

        /* exit the main loop */
        if(sw.s7)
            break;
    }

    /* blink the leds before dying */
    write_leds(0xFF);
    delay(LONG_DELAY);
    write_leds(OFF);

    return 0;
}
