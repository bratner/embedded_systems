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
void select_mux1(){
     TRISFCLR = bit(0)|bit(1)|bit(2)|bit(8); //(1 << 8) | 7 ; //MUX1 control pins     
     PORTFCLR = bit(0)|bit(1)|bit(2)|bit(8); // enable the mux
     TRISDCLR = bit(4); /* RD4 used for triggers */
}

void lcdt_comm_setup(){
    select_mux1();
    TRISBCLR = bit(15);  /* set bit B15 to output */
    TRISDCLR = bit(5);
}

void  lcdt_comm_write(unsigned char comm_type, unsigned char data){
    lcdt_comm_setup();
    PORTF=0;
    PORTBbits.RB15 = comm_type;
    PORTDbits.RD5 = 0; /* write */
    TRISECLR = 0xFF; /*PORTE low 8 bits to output*/
   
    PORTE = data;
    trigger();
    lcdt_busy_loop();
}

unsigned char lcdt_comm_read(){
    
    lcdt_comm_setup;
    TRISESET = 0xFF;
    PORTBbits.RB15 = 0; /*instruction*/
    PORTDbits.RD5 = 1; /* read */
    trigger();
    
    return PORTE;
}
void lcdt_function_set(unsigned char bus_size, unsigned char display_lines,
                       unsigned font_type){
    unsigned char command = bit(5) | bit(4)*bus_size | bit(3)*display_lines |
                            bit(3)*font_type;
    lcdt_comm_write(LCDT_INSTRUCTION, command);
}
void lcdt_begin(unsigned char lcdt_line_mode){
    int i;

    lcdt_comm_setup();
    /* send function_set 3 times */
    for(i=0;i<3;i++) lcdt_function_set(LCDT_BUS8, lcdt_line_mode, 0);
    lcdt_comm_write(LCDT_INSTRUCTION, 0xC);
    lcdt_comm_write(LCDT_INSTRUCTION, 0x6);
    lcdt_comm_write(LCDT_INSTRUCTION, 0x1);
        
}

void lcdt_onoff(unsigned char onoff, unsigned char cursor, unsigned char blinking){
    unsigned char command = bit(3) | bit(2)*onoff | bit(1)*cursor | bit(0)*blinking;
    lcdt_comm_write(LCDT_INSTRUCTION, command);
}
void lcdt_cls(){
    lcdt_comm_write(LCDT_INSTRUCTION, 0x1);
}
void lcdt_busy_loop(){
    while(lcdt_comm_read() & bit(7));

}
void busy_loop(unsigned long loops){
    int i;
    for(i=0; i<loops; i++);

}