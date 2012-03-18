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
    TCCR0A |= (1<<7)|(1<<5)|(1)|(1<<1);     //fast PWM; page 103
    TCCR0B |= (1<<1);                       //F_CPU/8; page 105:8kHz
    TCCR2A |= (1<<7)|(1<<5)|(1)|(1<<1);     //fast PWM; page 153
    TCCR2B |= (3<<1);                       //F_CPU/8,  page 157


    //set up pin directions
    DDRB = 0xFF;
    DDRD = 0xFF;
    DDRC = 0;
    
    //read in DIP; set appropriate timer setup; 
    if(PINC << 4  == 0){
	dly=6667;//8s revolution
	TCCR1B |= (1);        //F_CPU/1; page 133; 
    } else if (PINC << 4 == 16){
	dly=13333;//16s revolution
	TCCR1B |= (1);        //F_CPU/1
    } else if (PINC << 4 == 32){
	dly=3333;//32s revolution
	TCCR1B |= (1<<1);        //F_CPU/8
    } else if (PINC << 4 == 48){
	dly=6667;//64s revolution
	TCCR1B |= (1<<1);        //F_CPU/8
    } else if (PINC << 4 == 64){
	dly=7812;//10min revolution
	TCCR1B |= (1<<2);        //F_CPU/64
    } else if (PINC << 4 == 80){
	dly=46875;//1hr revolution
	TCCR1B |= (1<<2);        //F_CPU/64
    } else if (PINC << 4 == 96){
	dly=5859;//2hr revolution
	TCCR1B |= (5);        //F_CPU/1024
    } else if (PINC << 4 == 112){
	dly=11718;//4hr revolution
	TCCR1B |= (5);        //F_CPU/1024
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
    Sinewave output routine for unipolar stepper motor
    Effectively equates to PCM output with 8-bit word depth and
    dynamic sample rate of f * 64 
    *********************************************************/
    uint8_t sinewave[32] = {0, 25, 50, 74, 98, 120, 142, 162, 180,\
    197, 212, 225, 236, 244, 250, 254, 255, 254, 250, 244, 236, 225,\
    212, 197, 180, 162, 142, 120, 98, 74, 50, 25}; 

    static uint8_t state; 
    //least significant nybble of state tracks the microstate. Most significant
    //nybble tracks which 'macrostate' the system is in. This is necessary
    //because we have to switch output pin polarity every 90 degrees or 32 
    //microstates

    //increment the microstate to step the motor
    if (1==me){
	state++;
    } else if (2==me){
	state--; 
    }

    //write out the PWM; depending on the macrostate
    if (0==((state>>4)%4)){
	OCR0A = sinewave[microstate%16];
	OCR0B = sinewave[(microstate+16)%32];
	OCR2A = 0;
	OCR2B = 0;
    } else if (1==((state>>4)%4)){
	OCR0A = 0;
	OCR0B = sinewave[microstate%16];
	OCR2A = sinewave[(microstate+16)%32];
	OCR2B = 0;
    } else if (2==((state>>4)%4)){
	OCR0A = 0;
	OCR0B = 0;
	OCR2A = sinewave[microstate%16];
	OCR2B = sinewave[(microstate+16)%32];
    } else if (3==((state>>4)%4)){
	OCR0A = sinewave[(microstate+16)%32];
	OCR0B = 0;
	OCR2A = 0;
	OCR2B = sinewave[microstate%16];
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
