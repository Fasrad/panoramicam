/*-------|---------|---------|---------|---------|---------|---------|---------|
topAllegro.c
for the top of panoramicam
uses an Allegro multistepping stepper driver

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

This program written by chaz miller circa 2012
******************************************************************
For derivation and explanation of all the magic numbers,
see documentation panoramicalcs.ods
PORTD maps to Arduino digital pins 0 to 7 
PORTB maps to Arduino digital pins 8 to 13 
PORTC maps to Arduino analog pins 0 to 5
On boarduino: 
a0-a5: dip 1,2,3, input buttons and start button
Pins 8 step, 9 dir, 10ms1, 11ms2, 12ms3, 13 blinkenled 

Set a bit
 bit_fld |= (1 << n)

Clear a bit
 bit_fld &= ~(1 << n)

Toggle a bit
 bit_fld ^= (1 << n)

Test a bit
 bit_fld & (1 << n)
*/ 

#define F_CPU 16000000UL //*16MHz*
#include <avr/io.h>
#include <util/delay.h>

/* function for long delay */
void delay_ms(uint16_t ms) {
    while ( ms ){
            _delay_ms(1);
            ms--;
    }
}

void die (int me){
    //this function is for debug; various errors will cause a blinkenled
    while (1){
	for (int i=0; i<me;i++){
	    PORTB |= 1<<6; //twiddle LED pin
	    delay_ms(300);
	    PORTB |= 1<<6; 
	    delay_ms(300);
	    i++;
	}
    delay_ms(999);
    }
}//die
int main(){
    uint8_t delay;           //delay between pulses, in timer clockticks
    uint8_t maxdelay;
    uint8_t inflation;
    uint8_t bernanke;

    //set up pin directions
    DDRB = 0b00111111;
    DDRC = 0;
    
    //read in DIP; set speed setting; 
    if(PINC & 0b00000111 == 0){
	delay=10;//fastest speed 
    }
    if(PINC & 0b00000111 == 1){
	delay=20;//next slowest 
    }
    if(PINC & 0b00000111 == 2){
	delay=40;//etc
    }
    if(PINC & 0b00000111 == 3){
	delay=80;
    }
    if(PINC & 0b00000111 == 4){
	delay=100;
    }
    if(PINC & 0b00000111 == 5){
	delay=100;
    }

    TCCR1B |= ((1 << CS10) | (1 << CS11)); // Set up timer at Fcpu/256
    //16us; .016ms per tick; 1 second till overflow; .2% max error 
    //check datasheet on this timer setup!  
    //TCNT1 stores the value

    while(!(PINC &= 1<<7)){     //wait for user to pres button
	delay_ms(1);
    }

    TCNT = 0;
    PORTB |= 1<<1; //camera only works anticlockwise
//Pins 8 step, 9 dir, 10ms1, 11ms2, 12ms3, 13 blinkenled 

    if (;;){
	if (TCNT1 >= delay){       //poll timer
	    TCNT = 0;
	    PORTB ^= 1;            //loop runs 2x step speed
	    inflation += bernanke; //introduce nonlinearity
	    delay += inflation;    //slow motor in accordance w/prophecy
	    if (delay > maxdelay){die (1);} //no point in running forever
	}//timer loop
    }//infty loop
}//end main

