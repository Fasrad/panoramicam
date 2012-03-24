#define F_CPU 16000000UL //*16MHz*
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>


void delay(uint16_t);
void ustep(uint8_t);

int main(void){

//set up PWM timers
/*TCCR0A:
bit#    |--7---|--6---|--5---|--4---|--3--|--2--|--1--|--0--|
desc    |COM0A1|COM0A0|COM0B1|COM0B0|-Res-|-Res-|WGM01|WGM00|
set.    |--1---|--0---|--1---|--0---|--0--|--0--|--1--|--1--|
TCCR0B:
bit#    |--7---|--6---|--5---|--4---|--3--|--2--|--1--|--0--|
desc.   |FOC0A-|FOC0B-|-res--|--res-|WGM02|CS02-|CS01-|CS00-| 
set.    |--0---|--0---|--0---|--0---|--0--|--0--|--1--|--0--|*/
    TCCR0A = 0b10100011;     //fast PWM; page 103
    TCCR0B = 0b00000010;     //F_CPU/8; page 105:8kHz@8bit
/*TCCR2A:
bit#    |--7---|--6---|--5---|--4---|--3--|--2--|--1--|--0--|
desc    |COM0A1|COM0A0|COM0B1|COM0B0|-Res-|-Res-|WGM01|WGM00|
set.    |--1---|--0---|--1---|--0---|--0--|--0--|--1--|--1--|
TCCR2B:
bit#    |--7---|--6---|--5---|--4---|--3--|--2--|--1--|--0--|
desc.   |FOC0A-|FOC0B-|-res--|--res-|WGM02|CS02-|CS01-|CS00-| 
set.    |--0---|--0---|--0---|--0---|--0--|--0--|--1--|--0--|*/
    TCCR2A = 0b10100011;     //fast PWM; page 153
    TCCR2B = 0b00000010;     //F_CPU/8,  page 157

	//set up pin directions
	DDRB = 0xFF;
	DDRD = 0xFF;
	DDRC = 0xFF;

	PORTB = 0xFF;
	PORTC = 0xFF;
	PORTD = 0xFF;


	while (1){
	    static uint8_t dc; 
	    dc++;
	    OCR0A = dc;
	    OCR0B = dc+64;
	    OCR2A = dc+128;
	    OCR2B = dc+192;
	    delay(5);

	}
    }//main
    

void ustep(uint8_t me){
    /********************************************************
    Sinewave output routine for unipolar stepper motor
    1 is clockwise and 2 is anticlockwise 
    *********************************************************/
    static const uint8_t sinewave[32] PROGMEM = {0, 25, 50, 74, 98, 120, 142,
    162,180,197, 212, 225, 236, 244, 250, 254, 255, 254, 250, 244, 236, 225,
    212, 197, 180, 162, 142, 120, 98, 74, 50, 25};

    static uint8_t state;
    //I use the least significant nybble of byte 'state' to track PWM duty
    //cycle, and the most significant nybble to change output pin state
    //table every 90 degrees of the sinewave

    //increment the duty cycles to microstep the motor
    if(2==me){
	state--;
    }else if(1==me){
	state++;
    }

    //write out the PWM compare register values
   switch ((state >> 4)&3){ //X%N = X&(N-1) if X and N are powers of 2
        case 0:
            OCR0A = sinewave[(state+16)&31];
            OCR0B = sinewave[state&31];
            OCR2A = 0;
            OCR2B = 0;
            PORTB ^= (1 << 5);
            break;
        case 1:
            OCR0A = 0;
            OCR0B = sinewave[state&31];
            OCR2A = sinewave[(state+16)&31];
            OCR2B = 0;
            PORTB ^= (1 << 5);
            break;
        case 2:
            OCR0A = 0;
            OCR0B = 0;
            OCR2A = sinewave[(state+16)&31];
            OCR2B = sinewave[state&31];
            PORTB ^= (1 << 5);
            break;
        case 3:
            OCR0A = sinewave[(state+16)&31];
            OCR0B = 0;
            OCR2A = 0;
            OCR2B = sinewave[state&31];
            PORTB ^= (1 << 5);
            break;
	}
}//ustep
/*
	while (1){
	    static uint8_t dc; 
	    dc++;
	    OCR0A = dc;
	    OCR0B = dc+64;
	    OCR2A = dc+128;
	    OCR2B = dc+192;
	    delay(5);

	}

    for(;;){
   delay(1);
    }
}//main
*/
void delay(uint16_t me){
    while(me){
        _delay_ms(1);
        me--;
    }
}
