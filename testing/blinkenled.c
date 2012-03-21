//proof of concept
#include <math.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>

void main () {
//    DDRC |= 1; //should be boarduino analog pin 0
    while(1){
	DDRC = 0xFF;
	PORTC = 0xFF;

	for(int i=0; i<100; i++){
	    delay_ms(10); //max delay is 15ms at 16MHz so have to loop	
	}

	PORTC = 0;

	for(int i=0; i<100; i++){
	    delay_ms(10); //max delay is 15ms at 16MHz so have to loop	
	}
    }
}
