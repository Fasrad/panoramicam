#define F_CPU 16000000UL //*16MHz*
#include <avr/io.h>
#include <util/delay.h>


void delay(uint16_t);

int main(void){

//set up PWM timers
    TCCR0A |= (1<<7)|(1<<5)|(1)|(1<<1);     //fast PWM; page 103
    TCCR0B |= (1<<1);                       //F_CPU/8; page 105:8kHz@8bit
    TCCR2A |= (1<<7)|(1<<5)|(1)|(1<<1);     //fast PWM; page 153
    TCCR2B |= (1<<1);                       //F_CPU/8,  page 157

	//set up pin directions
	DDRB = 0xFF;
	DDRD = 0xFF;
	DDRC = 0;

	OCR0A = 100;
	OCR0B = 128;
	OCR2A = 150;
	OCR2B = 190;

    for(;;){
    delay(1);
    }
}//main

void delay(uint16_t me){
    while(me){
        _delay_ms(1);
        me--;
    }
}

