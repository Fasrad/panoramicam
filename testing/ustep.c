#define F_CPU 16000000UL //*16MHz*
#include <avr/io.h>
#include <util/delay.h>

void ustep(uint8_t);
void delay(uint16_t);

uint8_t state;

int main(){

    TCCR0A = 0b10100011;     //fast PWM; page 103
    TCCR0B = 0b00000010;     //F_CPU/8; page 105
    TCCR2A = 0b10100011;     //fast PWM; page 153
    TCCR2B = 0b00000010;     //F_CPU/8,  page 157

        //set up pin directions
        DDRB = 0xFF;
        DDRD = 0xFF;
        DDRC = 0xFF;

        PORTB = 0xFF;
        PORTC = 0xFF;
        PORTD = 0xFF;


   while(1){
       state++;
       ustep(state);
       delay(500);
   }

}//main

    void ustep(uint8_t state){
	uint8_t sinewave[32] = {0, 25, 50, 74, 98, 120, 142, 162, 180,
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



