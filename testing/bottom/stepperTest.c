//-------|---------|---------|---------|---------|---------|---------|---------|
/*
StepperTest
Tests a unipolar stepper
connected to portC pins 0 through 3

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

*/ 

#define blob 12
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
    uint16_t dly = blob;           //delay between pulses, in ms
    uint8_t state;

    //set up pin directions
    DDRC = 0xFF;
    
   while (1){
	switch (state){
	    case 0:
	    PORTC = 0b00000001;
	    break;
	    case 1:
	    PORTC = 0b00000011;
	    break;
	    case 2:
	    PORTC = 0b00000010;
	    break;
	    case 3:
	    PORTC = 0b00000110;
	    break;
	    case 4:
	    PORTC = 0b00000100;
	    break;
	    case 5:
	    PORTC = 0b00001100;
	    break;
	    case 6:
	    PORTC = 0b00001000;
	    break;
	    case 7:
	    PORTC = 0b00001001;
	    break;
	    default:
	    die(2);
	}
	/*j
    while (1){
	switch (state){
	    case 0:
	    PORTC = 0b00000001;
	    break;
	    case 1:
	    PORTC = 0b00000010;
	    break;
	    case 2:
	    PORTC = 0b00000100;
	    break;
	    case 3:
	    PORTC = 0b00001000;
	    break;
	    default:
	    die(2);
	}*/
	delay(dly);
	state++;
	if(state>7){state=0;}
    }//infty if
}//main
