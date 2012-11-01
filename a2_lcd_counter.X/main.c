/* 
 * File:   main.c
 * Author: bratner
 *
 * Created on October 31, 2012, 6:20 PM
 */

#include <p32xxxx.h>
#include "helpers.h"
void cls(){

}

int main(int argc, char** argv) {
    int i;
    unsigned int temp;
    temp = TRISG;
    temp &= 0xFFFF7FFF;
    TRISG = temp;
    temp = TRISB;
    temp &= 0xFFFF7FFF;
    TRISB = temp;

    TRISD=0;
    TRISF=0;
    TRISE=0;

 

    PORTF = 0; /* select the lcd*/
    PORTDbits.RD5 = 0; /* write */
    PORTBbits.RB15 = 0; /* instruction  */

    PORTE = 1; /* clear the damn lcd !*/

    trigger();

    return (0);
}

