/*-------|---------|---------|---------|---------|---------|---------|---------|
topAllegro.c	
For the top half of panoramicam. Written for the ATMEGA 168 microcontroller
and avr-gcc compiler, by chaz miller circa 2012.

this is designed to run a stepper motor using an Allegro A4983 microstepping
driver IC. 

This is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License version 3 or any later
version. This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
******************************************************************/

#define mdelay 5         //sets manual positioning speed; see delay()
#define F_CPU 16000000UL //*16MHz*
#include <avr/io.h>

void delay(uint16_t);
void die (uint8_t);
void blink (uint8_t);
void step (uint8_t);

int main(){
    /****************************************
    ***** initialize and setup camera  ******
    ****************************************/
    uint16_t dly;         //delay between steps, in timer ticks
    uint8_t inflation;    //slowing parameter, in timer ticks
    uint8_t counter = 1;  //counter var for dly updating
    DDRB = 0xFF;
    DDRC = 0;
    PORTB = 0b00011100;
    PORTC = 0xFF;
    //8 bit Timer 0 is the pwm timer. Also used by delay()
    TCCR0A = 0;                //fast pwm mode (page 103)
    TCCR0B = 2;                //scaler 2 (7.84 kHz) (page105)
    TCCR1A = 0;                //16 bit Timer 1: main program timer 
    for (int i=0; i<8; i++){   //startup blinkenled for user
	PORTB |= (1<<5);
	delay(800);
	PORTB &= ~(1<<5);
	delay(800);
    }
    delay(8000);
    //read in DIPswitch; set up TIMER1 per external calculation 
    //see allegro datasheet page 6
    if (!(PINC & (1<<3))){            //case for 28mm lens
	switch (PINC & 0b00000111){  
	    case 0:
		dly=7857;             //8s platform revolution
		inflation = 67;
		TCCR1B |= (1);        //F_CPU/1; page 133; 
		PORTB = 0b00010000;   // 1/2 step
		blink (0);
		break;
	    case 1:
		dly=7857;             //16s revolution
		inflation = 34;
		TCCR1B |= (1);        //F_CPU/1; page 133; 
		PORTB = 0b00001000;    //4th-step ms1,2,3 LHL 
		blink (1);
		break;
	    case 2:
		dly=10000;            //32s revolution
		inflation = 17;
		TCCR1B |= (1);        //F_CPU/1; page 133; 
		PORTB = 0b00011000;   // 1/8 step
		blink (2);
		break;
	    case 3:
		dly=15714;              //64s revolution
		inflation = 34;
		TCCR1B |= (1);        //F_CPU/1; page 133; 
		PORTB = 0b00011000;   // 1/8 step
		blink (3);
		break;
	    case 4:
		dly=4603;              //10min revolution
		inflation = 20;
		TCCR1B |= (3);         //F_CPU/64
		PORTB = 0b00001000;    //4th-step ms1,2,3 LHL 
		blink (4);
		break;
	    case 5:
		dly=3453;              //1hr revolution
		inflation = 30;
		TCCR1B |= (5);         //F_CPU/1024
		PORTB = 0b00010000;    //1/2 step
		blink (5);
		break;
	    case 6:
		dly=3453;             //2hr revolution
		inflation = 15;
		TCCR1B |= (5);        //F_CPU/1024
		PORTB = 0b00001000;    //4th-step ms1,2,3 LHL 
		blink (6);
		break;
	    case 7:
		dly=6905;             //4hr revolution
		inflation = 30;
		TCCR1B |= (5);         //F_CPU/1024
		PORTB = 0b00001000;    //4th-step ms1,2,3 LHL 
		blink (7);
		break;
	    default:
		die (1);
	} //28mm
    } else {
	switch (PINC & 0b00000111){   //for 50mm lens
	    case 0:
		dly=8800;             //8s revolution
		inflation = 76;
		TCCR1B |= (1);        //F_CPU/1; page 133; 
		PORTB = 0b00000000;   // 1th-step
		blink (0);
		break;
	    case 1:
		dly=8800;             //16s revolution
		inflation = 38;
		TCCR1B |= (1);        //F_CPU/1; page 133; 
		PORTB = 0b00010000;   // half-step
		blink (1);
		break;
	    case 2:
		dly=8800;             //32s revolution
		inflation = 18;
		TCCR1B |= (1);        //F_CPU/1; page 133; 
		PORTB = 0b00001000;   // quarter-step
		blink (2);
		break;
	    case 3:
		dly=17600;              //64s revolution
		inflation = 38;
		TCCR1B |= (1);        //F_CPU/1; page 133; 
		PORTB = 0b00001000;   // quarter-step
		blink (3);
		break;
	    case 4:
		dly=5156;              //10min revolution
		inflation = 22;
		TCCR1B |= (3);         //F_CPU/64
		PORTB = 0b00010000;   // half-step
		blink (4);
		break;
	    case 5:
		dly=3867;              //1hr revolution
		inflation = 34;
		TCCR1B |= (5);        //F_CPU/1024
		PORTB = 0b00000000;   // 1th-step
		blink (5);
		break;
	    case 6:
		dly=3867;             //2hr revolution
		inflation = 18;
		TCCR1B |= (5);        //F_CPU/1024
		PORTB = 0b00010000;   // half-step
		blink (6);
		break;
	    case 7:
		dly=7734;             //4hr revolution
		inflation = 34;
		TCCR1B |= (5);        //F_CPU/1024
		PORTB = 0b00010000;   // half-step
		blink (7);
		break;
	    default:
		die (1);
	}
    }//timer setups if
    //blink(inflation);         //debug
    while(PINC & 1<<5){       //manual positioning routine
	if(!(PINC & 1<<4)){
	    PORTB ^= (1<<1);
	    delay(mdelay);
	}
    }
    /****************************************
    *****this is where the magic happens*****
    ****************************************/
    blink(10); //countdown for user convenience
    TCNT1 = 0;
    PORTB &= ~(1<<5);           //clear blinkenled (kludge)
    while(1){  
	if (TCNT1 >= dly){die (2);}   //catch timer underrun/overflow
	while(TCNT1 < dly){}          //poll timer 
	TCNT1 = 0;          
	PORTB |= (1<<1);              //toggle Allegro input pin 
	if(!counter){                 //to save precision, only calculate
	    dly += inflation;         //inflation every 256 steps 
	}
	counter++;
	PORTB &= ~(1<<1);      //hopefully, this's been high 16 cycles (1us)
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
	delay(3000);
	PORTB &= ~(1<<5);
	delay(3000);
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
