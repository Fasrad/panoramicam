/*-------|---------|---------|---------|---------|---------|---------|---------|
topAllegro.c	
For the top half of panoramicam. Written for the ATMEGA 168 microcontroller
and avr-gcc compiler.
This is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License version 3 or any later
version. This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
******************************************************************/

#define F_CPU 16000000UL //*16MHz*
#include <avr/pgmspace.h>
#include <avr/io.h>

#define mdelay 40        //sets manual positioning speed; see delay()

void delay(uint16_t);
void die (uint8_t);
void blink (uint8_t);
void step (uint8_t);

int main(){
    uint16_t dly;    //delay between steps, in timer ticks
    //set up port pins 
    DDRB |= 0b00101000;
    DDRD |= 0b01101000;
    DDRC = 0;
    PORTB = 0xFF;
    PORTC = 0xFF;
    PORTD = 0xFF;
    //startup blinkenled
    for (int i=0; i<8; i++){
	PORTB |= (1<<5);
	delay(800);
	PORTB &= ~(1<<5);
	delay(800);
    }
    delay(8000);

    //aux. timer configs
    TCCR0B = 2;                   //F_CPU/8;  p.105  (7.84 kHz)

    //read in DIPswitch; set up TIMER1 per external calculation 
    //remember we step at twice the desired rate because allegro
    //only steps on a rising edge
    switch (PINC & 0b00000111){
	case 0:
	    dly=7143;             //8s revolution
	    TCCR1B |= (1);        //F_CPU/1; page 133; 
	    blink (0);
	    break;
	case 1:
	    dly=14286;            //16s revolution
	    TCCR1B |= (1);        //F_CPU/1
	    blink (1);
	    break;
	case 2:
	    dly=28571;            //32s revolution
	    TCCR1B |= (1);        //F_CPU/1
	    blink (2);
	    break;
	case 3:
	    dly=57142;             //64s revolution
	    TCCR1B |= (1);         //F_CPU/1
	    blink (3);
	    break;
	case 4:
	    dly=8370;              //10min revolution
	    TCCR1B |= (3);         //F_CPU/64
	    blink (4);
	    break;
	case 5:
	    dly=50223;             //1hr revolution
	    TCCR1B |= (3);         //F_CPU/256
	    blink (5);
	    break;
	case 6:
	    dly=25111;             //2hr revolution
	    TCCR1B |= (1<<2);      //F_CPU/256
	    blink (6);
	    break;
	case 7:
	    dly=50223;             //4hr revolution
	    TCCR1B |= (1<<2);      //F_CPU/256
	    blink (7);
	    break;
	default:
	    die (1);
    }

    uint8_t inflation = dly/833;
    uint8_t counter;              //sets inflation frequency 

    blink(inflation);

    //PORTD 3: enable when low
    PORTD &= ~(1<<3);

    while(PIND & 1<<1){           //allow user to pre-position camera
	if(!(PIND & 1<<2)){
	   PORTB |= (1<<3);
	} else if(!(PIND & 1)){
	   PORTB &= ~(1<<3);
	}
	   PORTB ^= (1<<4);
	delay(mdelay);
    }
    //button has been pressed; initiate pictionation 

    blink(10);

    TCNT1 = 0;
    PORTB |= 0b00000111;           //Allegro chip config ccw

    //PORTB 0-2: MS select, 3:dir, 4:step
    while(1){
	if (TCNT1 >= dly){die (2);}   //catch possible timer underrun
	while(TCNT1 < dly){}          //poll timer 
	TCNT1 = 0;          
	PORTB ^= (1<<4);
	if(!(counter)){
	    dly += inflation;      
	}
	counter++;
    }
}//main

void delay(uint16_t me){    //at F_CPU/8, each unit is 128us. 1ms is 8units. 
    while(me){
	while(TCNT0 < 128){}
	me--;
	while(TCNT0 > 128){}
    }
}

void die (uint8_t me){
    while(1){
	blink(me);
	delay(20000);
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
/*Set a bit
 bit_fld |= (1 << n)

Clear a bit
 bit_fld &= ~(1 << n)

Toggle a bit
 bit_fld ^= (1 << n)

Test a bit
 bit_fld & (1 << n)
*/ 
