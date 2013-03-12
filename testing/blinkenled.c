//proof of concept
#include <math.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>

//    DDRC |= 1; //should be boarduino analog pin 0

void main () {

    uint8_t pulse_width=10;
    uint8_t space_width=100;

    DDRC = 0xFF;
    PORTC = 0xFF;

    while(1){

	for(int i=0; i<100; i++){
	    delay_ms(space_width); //max delay is 15ms at 16MHz so have to loop	
	}

	PORTC = 0;
	delay_ms(pulse_width);
    }
}
