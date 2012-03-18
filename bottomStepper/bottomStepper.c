/*-------|---------|---------|---------|---------|---------|---------|---------|
bottomStepper.c	

written for the ATMEGA 168 microcontroller and avr-gcc compiler
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
******************************************************************
see documentation panoramicalcs.ods

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
    uint16_t dly;           //delay between steps, in timer clockticks
    uint8_t macrostate;     //which 'whole' step the motor is on 
    uint8_t microstate;     

    //set up timers
    TCCR0A |= (1<<7)|(1<<5)|(1);     //phase correct PWM; page 103
    TCCR0B |= (1<<2);                //F_CPU/256; page 105
    TCCR2A |= (1<<7)|(1<<5)|(1);     //phase correct PWM; page 150sh
    TCCR2B |= (3<<1);                //F_CPU/256, page 157

    TCCR1B |= (1<<2);                //F_CPU/256; page 133

    //set up pin directions
    DDRB = 0xFF;
    DDRD = 0xFF;
    DDRC = 0;
    
    //read in DIP; set speed setting; 
    if(PINC & 0b00000111 == 0){
	dly=64;//.6283rad/s, 8s revolution
    } else if (PINC & 0b00000111 == 1){
	dly=32;//.3142rad/s, 16s revolution
    } else if (PINC & 0b00000111 == 2){
	dly=16;//.1571rad/s, 32s revolution
    } else if (PINC & 0b00000111 == 3){
	dly=8;//.07854rad/s, 62s revolution
    } else if (PINC & 0b00000111 == 4){
	dly=600;//.07854rad/s, 10min revolution
    } else if (PINC & 0b00000111 == 5){
	dly=3200;//.07854rad/s, 1hr revolution
    } else if (PINC & 0b00000111 == 6){
	dly=7200;//.07854rad/s, 2hr revolution
    } else if (PINC & 0b00000111 == 7){
	dly=7200;//.07854rad/s, 4hr revolution
    } else {die 10;}

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
	if (TCNT1 >= dly){       //poll timer and microstep anticlockwise
	    TCNT1 = 0;
	    ustep(2);
	}
    }
}//main

void ustep(uint8_t me){

    //1 is clockwise and 2 is anticlockwise 
    /********************************************************
    Sinewave microstepping routine for unipolar stepper motor
    using an array instead of a giant switch statement. Unsure if this
    is actually any better at runtime. There is a good bit of overhead
    here but it shouldn't matter at step frequencies < 1000hz. I think I
    liked the state machine better
    *********************************************************/
    uint8_t sinewave[32] = {0, 25, 50, 74, 98, 120, 142, 162, 180,\
    197, 212, 225, 236, 244, 250, 254, 255, 254, 250, 244, 236, 225,\
    212, 197, 180, 162, 142, 120, 98, 74, 50, 25}; 

    static uint8_t microstate;
    static uint8_t macrostate;

    //increment the microstate to step the motor
    if (1==me){
	microstate++;
    } else if (2==me){
	microstate--; 
    }
    if (microstate > 31){
	microstate=0;
	macrostate++;
    }
    if (macrostate > 3){
	macrostate=0;
    }
    if (0==macrostate){
	OCR0A = sinewave[microstate];
	OCR0B = sinewave[(microstate+15)%31];
	OCR2A = 0;
	OCR2B = 0;
    } else if (1==macrostate){
	OCR0A = 0;
	OCR0B = sinewave[microstate];
	OCR2A = sinewave[(microstate+15)%31];
	OCR2B = 0;
    } else if (2==macrostate){
	OCR0A = 0;
	OCR0B = 0;
	OCR2A = sinewave[microstate];
	OCR2B = sinewave[(microstate+15)%31];
    } else if (3==macrostate){
	OCR0A = sinewave[(microstate+15)%31];
	OCR0B = 0;
	OCR2A = 0;
	OCR2B = sinewave[microstate];
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
    } else if (2==me){
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

    }

}
