/*example program to test microstepping on the microcontroller hardware.
since pwm.c and overflow.c both work, this should too. But it doesn't; it
seems to full-step the motor every 16 increments, instead of smoothly
microstepping. it goes 1 rev in 1:35 at 5ms delay, just like pwm.c at the
same delay. Step sequence seems to be as follows with no intermediate voltages
1100
0110
0011
1001
It would do this if all PWM values are always 255. But why...pwm.c works
fine--with pwm.c you can multimeter each pin smoothly move from 0 to 4.8V.
timer setups are identical so wtf?
*/

#define F_CPU 16000000UL //*16MHz*
#include <avr/io.h>
#include <util/delay.h>

void ustep();
void delay(uint16_t);
uint8_t state;

int main(){
    //same timers as pwm.c
    TCCR0A = 0b10100011;     //fast PWM; page 103
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
    while(1){
	state++;
	ustep();
	delay(5);
    }
}//main

//ustep() copied from overflow.c; printf's just replaced with OCR assigns
void ustep(){
    static uint8_t sinewave[32] = {0, 25, 50, 74, 98, 120, 142, 162, 180,
    197, 212, 225, 236, 244, 250, 254, 255, 254, 250, 244, 236, 225,
    212, 197, 180, 162, 142, 120, 98, 74, 50, 25};
    //write out the PWM compare register values
    switch ((state >> 4)&3){ //X%N = X&(N-1) if X and N are powers of 2
	case 0:
	    OCR0A = sinewave[(state+16)&31];
	    OCR0B = sinewave[state&31];
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
	    OCR2A = sinewave[(state+16)&31];
	    OCR2B = sinewave[state&31];
	    break;
	case 3:
	    OCR0A = sinewave[(state+16)&31];
	    OCR0B = 0;
	    OCR2A = 0;
	    OCR2B = sinewave[state&31];
	    break;
    }//switch
}//ustep
void delay(uint16_t me){
    while(me){
        _delay_ms(1);
        me--;
    }
}



