/*-------|---------|---------|---------|---------|---------|---------|---------|
testTop.c
test top
******************************************************************/

#define mdelay 5         //sets manual positioning speed; see delay()
#define F_CPU 16000000UL //*16MHz*
#include <avr/io.h>

void delay(uint16_t);
void die (uint8_t);
void blink (uint8_t);
void ustep(uint8_t);

uint16_t dly=120;

int main(){
    //set up port pins 

    DDRB = 0xFF;
    DDRC = 0;
    PORTB = 0b00011101;
    PORTC = 0xFF;
    //PWM timer configs
    TCCR0A = 0b00000000;     //fastPWM;  p.103 in datasheet
    TCCR0B = 2;              //F_CPU/8;  p.105  (7.84 kHz)
    TCCR1A = 0;
    TCCR1B = 0b00000010;

    //startup blinkenled
    for (int i=0; i<8; i++){
	PORTB |= (1<<5);
	delay(800);
	PORTB &= ~(1<<5);
	delay(800);
    }
    delay(8000);

    blink(10);

    TCNT1 = 0;

    while(1){
	if (TCNT1 >= dly){die (2);}   //catch possible timer underrun
	while(TCNT1 < dly){}          //poll timer 
	TCNT1 = 0;          
//	PORTB ^= (0b00100010);
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
