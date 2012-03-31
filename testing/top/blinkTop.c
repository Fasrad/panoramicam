/*-------|---------|---------|---------|---------|---------|---------|---------|
blinkTop--blink 
******************************************************************/

#define F_CPU 16000000UL //*16MHz*
#include <avr/io.h>

#define mdelay 40        //sets manual positioning speed; see delay()

void delay(uint16_t);
void die (uint8_t);
void blink (uint8_t);
void step (uint8_t);

int main(){
    DDRB = 0xFF; 
    DDRC = 0;
    PORTB = 0xFF;
    PORTC = 0xFF;
    //startup blinkenled
    for (int i=0; i<8; i++){
	PORTB |= (1<<5);
	delay(800);
	PORTB &= ~(1<<5);
	delay(800);
    }
    delay(8000);

    //aux. timer configs
    TCCR0B = 2;                   //F_CPU/8;  p.105  (7.84 kHz)

    uint8_t counter;              //sets inflation frequency 
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
	delay(300);
	PORTB &= ~(1<<5);
	delay(300);
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
