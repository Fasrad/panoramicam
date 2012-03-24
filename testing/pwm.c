//example program to test microcontroller PWM. 
//this program works as expected, when each pin is tested via LED. 
//it's not a correct algorith, but it actually full-steps the motor as well
//at 1:35 per rev at 5ms delay

#define F_CPU 16000000UL //*16MHz*
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

void delay(uint16_t);
void ustep(uint8_t);
uint8_t state;

int main(void){
    //set up PWM timers
    TCCR0A = 0b10100011;     //fast PWM; datasheet page 103
    TCCR0B = 0b00000010;     //F_CPU/8; page 105
    TCCR2A = 0b10100011;     //fast PWM; page 153
    TCCR2B = 0b00000010;     //F_CPU/8,  page 157
    //set up pin directions
    DDRB = 0xFF;
    DDRD = 0xFF;
    DDRC = 0xFF;
    //set all pins high
    PORTB = 0xFF;
    PORTC = 0xFF;
    PORTD = 0xFF;
    while (1){
	state++;
	OCR0A = state;
	OCR0B = state+64;
	OCR2A = state+128;
	OCR2B = state+192;
	delay(100);
    }
}//main

void delay(uint16_t me){
    while(me){
        _delay_ms(1);
        me--;
    }
}
