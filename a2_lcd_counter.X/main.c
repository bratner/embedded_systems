/* 
 * File:   main.c
 * Author: bratner
 *
 * Created on October 31, 2012, 6:20 PM
 */

#include <p32xxxx.h>
#include <helpers.h>


#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8


struct str {
        char * s;
        unsigned char len;
    };

/* #define lcdt_dump_array(a) for(i=0; i<sizeof(a);i++){ lcdt_comm_write(1,a[i]);}
 */
void delay(unsigned int);
/*
 * LCD text
 *  RB15 - RS
 *  RD5  - R/~W
 *  RD4 - EN (fall)
 *  RE0-7 - DB0-7
 *
 *
 */
void init(){
    select_mux1();
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


   /*
    * manage tasks with queues
    * lets say i have a queue of tasks
    * each task has a time to be executed
    * lets say i have a clock that ticks every 1ms
    * if the processor is 80Mhz the 1ms is a lot of ticks
    *  a simple delay while leaving the time for other processes to
    *
    */



    char buzz = 0;
    int i;
    unsigned char leds = 0;
    unsigned char stage = 0;
    unsigned char mode = 'X';
    /* array to do the "fan" assignment */
    unsigned char fan[] = { bit(3)|bit(4), bit(2)|bit(5),
                               bit(1)|bit(6), bit(0)|bit(7) };
    unsigned char initlcdt[] = { 0x38, 0x38, 0x38, 0xC, 6,1};
    struct str strings[] = {{"Mode X:",7},{"Counter",7}};

    switches sw = {0}, prevsw = {0};
    init();
    lcdt_comm_setup();
    lcdt_begin(LCDT_TWOLINES);
   
   
   
   
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
            mode = '2';
                stage = stage % 4;
                leds = fan[stage];

        /* switch s1 does the shift bit thing */
        } else if(sw.s1) {
            mode = '1';
            stage = stage % 8;
            leds = 1 << stage;

        /* switch s0 does the counter */
        } else if(sw.s0) {
            mode = '0';
            leds = stage;
        } else {
            leds = 0;
            stage = 0;
            mode = 'X';
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
        /* set cursor at 0 */
        lcdt_comm_write(LCDT_INSTRUCTION, 2);
        /*lcdt_comm_write(LCDT_DATA, 0x30+leds%10) */
         for(i=0; i<strings[0].len;i++){
            lcdt_comm_write(LCDT_DATA,strings[0].s[i]);
         }
        /*goto(5,0) */
        lcdt_comm_write(LCDT_INSTRUCTION, 0x85);
        lcdt_comm_write(LCDT_DATA,mode);

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


