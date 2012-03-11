/*-------|---------|---------|---------|---------|---------|---------|---------|
bottomStepper.c	

uses a unipolar stepper 
rotates a platform at various speeds

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

int main(){
    uint16_t dly;           //delay between pulses, in timer clockticks
    uint8_t macrostate;     //which 'whole' step the motor is on 
    uint8_t microstate;     

    //set up pin directions
    DDRB = 0b00000011;
    DDRD = 0xFF;
    DDRC = 1<<3;
    
    //read in DIP; set speed setting; 
    if(PINC & 0b00000111 == 0){
	dly=64;//.6283rad/s, 8s revolution
    }
    if(PINC & 0b00000111 == 1){
	dly=32;//.3142rad/s, 16s revolution
    }
    if(PINC & 0b00000111 == 2){
	dly=16;//.1571rad/s, 32s revolution
    }
    if(PINC & 0b00000111 == 3){
	dly=8;//.07854rad/s, 62s revolution
    }
    if(PINC & 0b00000111 == 4){
	dly=600;//.07854rad/s, 10min revolution
    }
    if(PINC & 0b00000111 == 5){
	dly=3200;//.07854rad/s, 1hr revolution
    }
    if(PINC & 0b00000111 == 6){
	dly=7200;//.07854rad/s, 2hr revolution
    }
    if(PINC & 0b00000111 == 7){
	dly=7200;//.07854rad/s, 4hr revolution
    }

    TCCR1B |= ((1 << CS10) | (1 << CS11)); // Set up timer at Fcpu/256
    //16us; .016ms per tick; 1 second till overflow; .2% max error 
    //check datasheet on this timer setup!  
    //TCNT1 stores the value

    while(!(PINC &= 1<<7)){     //allow user to position camera
	if(PINC &= 1<<3){
	   step(1);
	}
	if(PINC &= 1<<4){
	   step(2);
	}
	delay(1);
    }//start button pressed, initiate pictionation sequence

    TCNT1 = 0;

    if (;;){
	if (TCNT1 >= dly){       //poll timer
	    TCNT1 = 0;
	    ustep(1);
	}
    }
}//main

void ustep(uint8_t me){

    /********************************************************
    Sinewave microstepping routine for unipolar stepper motor
    using an array instead of a giant switch statement. Unsure if this
    is actually any better at runtime. There is a good bit of overhead
    here but it shouldn't matter at step frequencies < 1000hz. I 
    liked the state machine better
    *********************************************************/
    //array of sinusoidal 8-bit PWM duty cycle values
    //note; only indices up to 31 are legal for the microstate variable
    //hardcoded duty cycles--index 0=0; 15=255; 31=0 again, and so on.
    uint8_t sinewave[32] = {0, 25, 50, 74, 98, 120, 142, 162, 180,\
    197, 212, 225, 236, 244, 250, 254, 255, 254, 250, 244, 236, 225,\
    212, 197, 180, 162, 142, 120, 98, 74, 50, 25, 0}; 
    static uint8_t microstate;
    static uint8_t macrostate;

    //increment the microstate to step the motor
    microstate++;
    if (microstate > 31){
	microstate=0;
	macrostate++;
    }
    if (macrostate > 4){
	macrostate=0;
    }
    if (1==macrostate){
	TCCRwire1 = sinewave[microstate];
	TCCRwire2 = sinewave[(microstate+15)%31];
	TCCRwire3 = 0;
	TCCRwire4 = 0;
    } else if (2==macrostate){
	TCCRwire1 = 0;
	TCCRwire2 = sinewave[microstate];
	TCCRwire3 = sinewave[(microstate+15)%31];
	TCCRwire4 = 0;
    } else if (3==macrostate){
	TCCRwire1 = 0;
	TCCRwire2 = 0;
	TCCRwire3 = sinewave[microstate];
	TCCRwire4 = sinewave[(microstate+15)%31];
    } else if (4==macrostate){
	TCCRwire1 = sinewave[(microstate+15)%31];
	TCCRwire2 = 0;
	TCCRwire3 = 0;
	TCCRwire4 = sinewave[microstate];
    } else {die (5);}
}//ustep

void step(uint8_t me){

    /**************************************
    regular old fashioned stepping routine.
    1=clockwise 0=anticlockwise
    **************************************/

    static uint8_t state = 1;

    if (1==me){
	state++;
    } else if (0==me){
	state--;
    } else {die (6);}

    if (0==state){
	state=4;
    } else if (state > 4){
	state=1;
    } else {die (6);}

    switch (state){
	case 1:
	PORTC = 0b000000001;
	break;
	case 2:
	PORTC = 0b000000010;
	break;
	case 3:
	PORTC = 0b000000100;
	break;
	case 4:
	PORTC = 0b000001000;
	break;
    }
}//step

void die (int me){
    for (;;){
	//

    }

}
