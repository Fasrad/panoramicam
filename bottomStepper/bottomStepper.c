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

void delay(uint16_t);
void die (uint8_t);
void blink (uint8_t);
void ustep(uint8_t);

int main(){
    uint16_t dly;    //delay between steps, in timer ticks

    //set up port pins 
    DDRB |= 0b00001000;
    DDRD |= 0b01101000;
    DDRC = 0;
    PORTB = 0xFF;
    PORTC = 0xFF;
    PORTD = 0xFF;

    //startup blinkenled
    for (int i=0; i<8; i++){
	PORTB |= (1<<5);
	delay(100);
	PORTB &= ~(1<<5);
	delay(100);
    }
    delay(1000);

    //timer configs
    TCCR0A |= (1<<7)|(1<<5)|(1)|(1<<1);     //fast PWM; page 103
    TCCR0B |= (1<<1);                       //F_CPU/8; page 105:8kHz@8bit
    TCCR2A |= (1<<7)|(1<<5)|(1)|(1<<1);     //fast PWM; page 153
    TCCR2B |= (1<<1);                       //F_CPU/8,  page 157
    OCR0A = 128;
    OCR0B = 128;
    OCR2A = 128;
    OCR2B = 128;


    //read in DIP; set up TIMER1 per spreadsheet calculation 
    switch (PINC & 0b00000111){
    case 0:
	dly=6667;             //8s revolution
	TCCR1B |= (1);        //F_CPU/1; page 133; 
	blink (0);
	break;
    case 1:
	dly=13333;            //16s revolution
	TCCR1B |= (1);        //F_CPU/1
	blink (1);
	break;
    case 2:
	dly=3333;             //32s revolution
	TCCR1B |= (1<<1);     //F_CPU/8
	blink (2);
	break;
    case 3:
	dly=6667;             //64s revolution
	TCCR1B |= (1<<1);     //F_CPU/8
	blink (3);
	break;
    case 4:
	dly=7812;             //10min revolution
	TCCR1B |= (1<<2);     //F_CPU/64
	blink (4);
	break;
    case 5:
	dly=46875;            //1hr revolution
	TCCR1B |= (1<<2);     //F_CPU/64
	blink (5);
	break;
    case 6:
	dly=5859;             //2hr revolution
	TCCR1B |= (5);        //F_CPU/1024
	blink (6);
	break;
    case 7:
	dly=11718;            //4hr revolution
	TCCR1B |= (5);        //F_CPU/1024
	blink (7);
	break;
    default:
	die (10);
    }

    while(PIND & 1<<1){   //allow user to pre-position camera
	if(!(PIND & 1<<2)){
	   ustep(1);
	}
	if(!(PIND & 1)){
	   ustep(2);
	}
	delay(1);
    }
    //start button pressed; initiate pictionation sequence

    blink(10);

    TCNT1 = 0;

    while(1){
	if (TCNT1 >= dly){       //poll timer and microstep anticlockwise
	    TCNT1 = 0;
	    ustep(2);
//	    PORTB ^= (1 << 5);
	}
    }
}//main

void ustep(uint8_t me){

    //1 is clockwise and 2 is anticlockwise 
    /********************************************************
    Sinewave output routine for unipolar stepper motor
    *********************************************************/
    uint8_t sinewave[32] = {0, 25, 50, 74, 98, 120, 142, 162, 180,\
    197, 212, 225, 236, 244, 250, 254, 255, 254, 250, 244, 236, 225,\
    212, 197, 180, 162, 142, 120, 98, 74, 50, 25}; 

    static uint8_t state; 

    //increment the microstate to step the motor
    if (2==me){
	state--;
    } else if (1==me){
	state++; 
    }

    /*I use the most significant nybble of byte 'state' to change
    output pin state table every 32 steps*/

    //write out the PWM compare register values
    switch ((state >> 4)&3){ //X%N same as X&(N-1) if X and N are power of 2
	case 0:
	    OCR0A = sinewave[state&31];
	    OCR0B = sinewave[(state+16)&31];
	    OCR2A = 0;
	    OCR2B = 0;
	    break;
	case 1:
	    OCR0A = 0;
	    OCR0B = sinewave[state&31];
	    OCR2A = sinewave[(state+16)&31];
	    OCR2B = 0;
	    break;
	case 2:
	    OCR0A = 0;
	    OCR0B = 0;
	    OCR2A = sinewave[state&31];
	    OCR2B = sinewave[(state+16)&31];
	    break;
	case 3:
	    OCR0A = sinewave[(state+16)&31];
	    OCR0B = 0;
	    OCR2A = 0;
	    OCR2B = sinewave[state&31];
	    break;
	default: 
	    die (5);
    }
}//ustep

void delay(uint16_t me){
    while(me){
	_delay_ms(1);
	me--;
    }
}

void die (uint8_t me){
    for (int i=0; i<10; i++){
	PORTB |= (1<<5);
	delay(40);
	PORTB &= ~(1<<5);
	delay(40);
    }
    while(1){
	for (int i=0; i<me; i++){
	    PORTB |= (1<<5);
	    delay(300);
	    PORTB &= ~(1<<5);
	    delay(300);
	}
	delay(2000);
    }
}
void blink (uint8_t me){
    for (int i=0; i<me; i++){
	PORTB |= (1<<5);
	delay(300);
	PORTB &= ~(1<<5);
	delay(300);
    }
    delay(600);
}
