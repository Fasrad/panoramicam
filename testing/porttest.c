//-------|---------|---------|---------|---------|---------|---------|---------|
/*
Porttest
Tests output ports

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

*/ 

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

void die (int me){
    //this function is for debug; various errors will cause a blinkenled
    while (1){
        for (int i=0; i<me;i++){
            PORTB |= 1<<6; //twiddle LED pin
            delay(300);
            PORTB |= 1<<6;
            delay(300);
	}
    delay(100);
    }
}


int main(){

   while (1){
    //set up pin directions
    DDRB = 0;
    DDRC = 0;
    DDRD = 0;

    //set up port statuses
    PORTB = 0xFF;
    PORTC = 0xFF;
    PORTD = 0xFF;

    delay(5000);
    
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;

    delay(5000);

    }//infty if
}//main
