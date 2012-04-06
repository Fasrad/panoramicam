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

#define mdelay 5        //sets manual positioning speed; see delay()
#define F_CPU 16000000UL //*16MHz*
#include <avr/io.h>

void delay(uint16_t);
void die (uint8_t);
void blink (uint8_t);
void step (uint8_t);

int main(){
    uint16_t dly;         //delay between steps, in timer ticks
    uint8_t inflation;    //increase in dly, in timer ticks
    uint8_t counter = 1;      //sets inflation update frequency 
    //set up port pins 
    DDRB = 0xFF;
    DDRC = 0;
    PORTB = 0b00011100;
    PORTC = 0xFF;
    // timer configs
    //TCCR0A = 0b00000011;     //fastPWM;  p.103 in datasheet
    //TCCR0B = 2;              //F_CPU/8;  p.105  (7.84 kHz)
    TCCR0A = 0;              // p.103 in datasheet
    TCCR0B = 2;              //scaler,  p.105  (7.84 kHz)
    TCCR1A = 0;
    //startup blinkenled
    for (int i=0; i<8; i++){
	PORTB |= (1<<5);
	delay(800);
	PORTB &= ~(1<<5);
	delay(800);
    }
    delay(8000);
    //read in DIPswitch; set up TIMER1 per external calculation 
    if (!(PINC & (1<<3))){            //for 28mm lens
	switch (PINC & 0b00000111){  
	    case 0:
		dly=7143;             //8s revolution
		inflation = 17;
		TCCR1B |= (1);        //F_CPU/1; page 133; 
		PORTB = 0b00011100;   //16th-step
		blink (0);
		break;
	    case 1:
		dly=1786;             //16s revolution
		inflation = 4;
		TCCR1B |= (2);        //F_CPU/8
		PORTB = 0b00011100;   //16th-step
		blink (1);
		break;
	    case 2:
		dly=3571;             //32s revolution
		inflation = 9;
		TCCR1B |= (2);        //F_CPU/8
		PORTB = 0b00011100;   //16th-step
		blink (2);
		break;
	    case 3:
		dly=7843;              //64s revolution
		inflation = 17;
		TCCR1B |= (2);         //F_CPU/8
		PORTB = 0b00011100;    //16th-step
		blink (3);
		break;
	    case 4:
		dly=8371;              //10min revolution
		inflation = 20;
		TCCR1B |= (3);         //F_CPU/64
		PORTB = 0b00011100;    //16th-step
		blink (4);
		break;
	    case 5:
		dly=3139;              //1hr revolution
		inflation = 8;
		TCCR1B |= (5);         //F_CPU/1024
		PORTB = 0b00011100;    //16th-step
		blink (5);
		break;
	    case 6:
		dly=6278;             //2hr revolution
		inflation = 15;
		TCCR1B |= (5);        //F_CPU/1024
		PORTB = 0b00011100;   //16th-step
		blink (6);
		break;
	    case 7:
		dly=12556;             //4hr revolution
		inflation = 30;
		TCCR1B |= (5);         //F_CPU/1024
		PORTB = 0b00011100;    //16th-step
		blink (7);
		break;
	    default:
		die (1);
	}
    } else {
	switch (PINC & 0b00000111){   //for 50mm lens
	    case 0:
		dly=4000;             //8s revolution
		inflation = 10;
		TCCR1B |= (1);        //F_CPU/1; page 133; 
		PORTB = 0b00011100;    //16th-step
		blink (0);
		break;
	    case 1:
		dly=2000;             //16s revolution
		inflation = 10;
		TCCR1B |= (2);        //F_CPU/8
		PORTB = 0b00011100;    //16th-step
		blink (1);
		break;
	    case 2:
		dly=4000;             //32s revolution
		inflation = 19;
		TCCR1B |= (2);        //F_CPU/8
		PORTB = 0b00011000;    //8th-step ms1,2,3 HHL 
		blink (2);
		break;
	    case 3:
		dly=2000;              //64s revolution
		TCCR1B |= (3);         //F_CPU/8
		inflation = 19;
		PORTB = 0b00001000;    //4th-step ms1,2,3 LHL 
		blink (3);
		break;
	    case 4:
		dly=4687;              //10min revolution
		inflation = 11;
		TCCR1B |= (3);         //F_CPU/64
		PORTB = 0b00011100;    //16th-step
		blink (4);
		break;
	    case 5:
		dly=7031;              //1hr revolution
		inflation = 17;
		TCCR1B |= (4);         //F_CPU/256
		PORTB = 0b00011100;    //16th-step
		blink (5);
		break;
	    case 6:
		dly=3515;             //2hr revolution
		inflation = 8;
		TCCR1B |= (5);        //F_CPU/1024
		PORTB = 0b00011100;    //16th-step
		blink (6);
		break;
	    case 7:
		dly=7031;             //4hr revolution
		inflation = 17;
		TCCR1B |= (5);        //F_CPU/1024
		PORTB = 0b00011100;    //16th-step
		blink (7);
		break;
	    default:
		die (1);
	}
    }//if

   // delay(8000);
    //blink(inflation);  //debug

    while(PINC & 1<<5){           //allow user to manually turn motor
	if(!(PINC & 1<<4)){
	    PORTB ^= (1<<1);
	    delay(mdelay);
	    }
    }
    //button has been pressed; initiate pictionation 

    blink(10); //countdown

    TCNT1 = 0;
    PORTB &= ~(1<<5);           //clear blinkenled

    while(1){
	if (TCNT1 >= dly){die (2);}   //catch timer underrun/overflow
	while(TCNT1 < dly){}          //poll timer 
	TCNT1 = 0;          
	PORTB |= (1<<1);
	if(!counter){             //to save precision, only update 
		dly += inflation;     //every 256 steps 
	}
	counter++;
	PORTB &= ~(1<<1);         //think it's been high 16 cycles (1us)?
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
