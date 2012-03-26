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

#define mdelay 4         //sets manual positioning speed; see delay()
#define F_CPU 16000000UL //*16MHz*
#include <avr/pgmspace.h>
#include <avr/io.h>

void delay(uint16_t);
void die (uint8_t);
void blink (uint8_t);
void step(uint8_t);

uint16_t dly;    //delay between steps, in timer ticks

int main(){
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

    //read in DIPswitch; set up TIMER1 per external calculation 
    //remember we step at twice the desired rate because allegro
    //only steps on a rising edge
    switch (PINC & 0b00000111){
	case 0:
	    dly=3333;             //8s revolution
	    TCCR1B |= (1);        //F_CPU/1; page 133; 
	    blink (0);
	    break;
	case 1:
	    dly=6666;             //16s revolution
	    TCCR1B |= (1);        //F_CPU/1
	    blink (1);
	    break;
	case 2:
	    dly=1667;             //32s revolution
	    TCCR1B |= (1<<1);     //F_CPU/8
	    blink (2);
	    break;
	case 3:
	    dly=3333;             //64s revolution
	    TCCR1B |= (1<<1);     //F_CPU/8
	    blink (3);
	    break;
	case 4:
	    dly=3906;             //10min revolution
	    TCCR1B |= (1<<2);     //F_CPU/64
	    blink (4);
	    break;
	case 5:
	    dly=23437;            //1hr revolution
	    TCCR1B |= (1<<2);     //F_CPU/64
	    blink (5);
	    break;
	case 6:
	    dly=2929;             //2hr revolution
	    TCCR1B |= (5);        //F_CPU/1024
	    blink (6);
	    break;
	case 7:
	    dly=5859;            //4hr revolution
	    TCCR1B |= (5);        //F_CPU/1024
	    blink (7);
	    break;
	default:
	    die (1);
    }

    while(PIND & 1<<1){           //allow user to pre-position camera
	if(!(PIND & 1<<2)){
	   step(1);
	} else if(!(PIND & 1)){
	   step(2);
	}
	delay(mdelay);
    }
    //button has been pressed; initiate pictionation 

    blink(10);

    TCNT1 = 0;

    while(1){
	if (TCNT1 >= dly){die (2);}   //catch possible timer underrun
	while(TCNT1 < dly){}          //poll timer 
	TCNT1 = 0;          
	if(2==me){
	    PORTB |= 0xFF;
	} else if (1==me){
	    PORTB = 0xFF;
	} else {die (2);}
	inflation += bernanke;
	dly += inflation;
	PORTB ^= (1<<3);
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
