#define F_CPU 16000000UL //*16MHz*
#include <avr/io.h>
#include <util/delay.h>

void ustep(uint8_t);
void delay(uint16_t);

int main(){

   uint8_t state;

   while(1){
       state++;
       ustep(state);
       delay(5);
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



