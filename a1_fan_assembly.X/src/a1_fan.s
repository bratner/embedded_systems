#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8
#include <p32xxxx.h>
.global main
.data
fan:
    .byte 0b00011000, 0b00100100, 0b01000010, 0b10000001

.text

.ent main
main:

    la $t0, TRISD
    andi $t1, $t1, 0
    sw $t1, 0($t0)
    la $t0, TRISF
    sw $t1, 0($t0)
    addiu $t1, $t1, 55
    beqz $t0, test
    j main
test:
    nop
    j main
.end main

