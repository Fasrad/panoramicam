//example program for testing a microstep algorith. The output from this
//is completely as expected (I dump to .csv and graph w/spreadsheet)

#include <stdio.h>

void ustep();
unsigned char state;

int main(){
    printf("%s,%s,%s,%s,%s\n","state","coil1","coil2","coil3","coil4");
    for (int i=0;i<260;i++){
	state ++;
	ustep();
    }
    for (int i=0;i<260;i++){
	state --;
	ustep();
    }
}//main

void ustep(){
    unsigned char sinewave[32] = {0, 25, 50, 74, 98, 120, 142, 162, 180,
    197, 212, 225, 236, 244, 250, 254, 255, 254, 250, 244, 236, 225,
    212, 197, 180, 162, 142, 120, 98, 74, 50, 25};
    printf("%d,", state);
    //write out the PWM compare register values
    //most significant nybble is used to choose pin state table
    switch ((state >> 4)&3){ 
	case 0:
	    printf("%d,", sinewave[(state+16)&31]);
	    printf("%d,", sinewave[state&31]);
	    printf("%d,",0);
	    printf("%d\n",0);
	    break;
	case 1:
	    printf("%d,",0);
	    printf("%d,", sinewave[state&31]);
	    printf("%d,", sinewave[(state+16)&31]);
	    printf("%d\n",0);
	    break;
	case 2:
	    printf("%d,",0);
	    printf("%d,",0);
	    printf("%d,", sinewave[(state+16)&31]);
	    printf("%d\n", sinewave[state&31]);
	    break;
	case 3:
	    printf("%d,", sinewave[(state+16)&31]);
	    printf("%d,",0);
	    printf("%d,",0);
	    printf("%d\n", sinewave[state&31]);
	    break;
	default:
	    printf("%d\n",1);
    }//switch
}//ustep




