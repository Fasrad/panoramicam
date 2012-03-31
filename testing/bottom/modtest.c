#define F_CPU 16000000UL //*16MHz*
#include <avr/io.h>
#include <util/delay.h>


int main (void){

    uint8_t blarg;

while(1){

    blarg++;

    uint8_t foo = blarg/4%4;

}}
