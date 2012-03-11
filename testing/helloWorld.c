#define F_CPU 16000000UL //*16MHz*
#include <avr/io.h>
#include <util/delay.h>

/* function for long delay */
void delay(uint16_t ms) {
    while (ms){
	    _delay_ms(1);
	    ms--;
    }
}

int main(void){

uint16_t dly = 1000; //ms between blinks

DDRB=0xFF;
DDRC=0xFF;
DDRD=0xFF;

    for(;;){
	PORTB=0;
	PORTC=0;
	PORTD=0;

	delay(dly);
	
	PORTB=0xFF;
	PORTC=0xFF;
	PORTD=0xFF;

	delay(dly);
    }//for infty

}//main






