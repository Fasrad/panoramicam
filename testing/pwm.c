#define F_CPU 16000000UL //*16MHz*
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>


void delay(uint16_t);
void ustep(uint8_t);

int main(void){

uint8_t state;

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
	}

void delay(uint16_t me){
    while(me){
        _delay_ms(1);
        me--;
    }
}
