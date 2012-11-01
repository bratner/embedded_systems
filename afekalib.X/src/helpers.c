#include <p32xxxx.h>
#include "helpers.h"

void trigger(){
	     PORTDbits.RD4=1;
             PORTDbits.RD4=0;
}


char read_sw(){
        TRISE=0xff;
        PORTF=3;
        trigger();
        return PORTE;	
}
void buzzer(char on){
	TRISG = 0;
	if(on)
            
		PORTGbits.RG15=1;
	else
		PORTGbits.RG15=0;
	trigger();
}
void write_leds(char led){
	     TRISE=0; 
         PORTF=4;
         PORTE=led;
         trigger();
}

void update_tris(unsigned int * tris, unsigned int mask, char direction){
    /* if direction == 0 need to clear the mask if direction IN the need to set the mask*/
    unsigned int temp;
    temp = *tris;
    if(direction == INPUT ){
        temp |= mask;

    } else if (direction == OUTPUT ) {
        temp &= ~(mask);
    } else {
        return; /* unkown direction baby! */
    }

    *tris = temp; 
}