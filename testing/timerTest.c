/*-------|---------|---------|---------|---------|---------|---------|---------|
timertest
Tests a unipolar stepper
connected to Arduino pins 1-4
using timer logic instead of _delay_ms()
*/ 
#define F_CPU 16000000UL //*16MHz*
#include <avr/io.h>
#include <util/delay.h>

/* function for long delay */
void delay(uint16_t ms) {
    while (ms){
            _delay_ms(1);
            ms--;
    }
}

void die (int me){
    //this function is for debug; various errors will cause a blinkenled
    while (1){
        for (int i=0; i<me;i++){
            PORTB |= 1<<6; //twiddle LED pin
            delay(300);
            PORTB |= 1<<6;
            delay(300);
        }
    delay(1500);
    }
}



int main(){

    uint8_t state; //stepper state

    //set up pin directions
    DDRC = 0xFF;

    //set up timer at 250kHz, max 250 ms before overflow @ 65536
    TCCR1B |= ((1 << CS10) | (1 << CS11));

    while (1){
	if (TCNT1 > 250) {// 250 clockticks /ms
	    TCNT1 = 0;      
	    switch (state){
            case 0:
            PORTC = 0b00000001;
            break;
            case 1:
            PORTC = 0b00000010;
            break;
            case 2:
            PORTC = 0b00000100;
            break;
            case 3:
            PORTC = 0b00001000;
            break;
            default:
            die(2);
        }
        state++;
        if(state>3){state=0;}
	} //compare if 
    }//infty if
}//main

