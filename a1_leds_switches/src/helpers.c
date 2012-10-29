#include <p32xxxx.h>

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