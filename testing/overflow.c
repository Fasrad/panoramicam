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
    /*unsigned char sinewave[64] = {0,13,25,37,50,62,74,86,98,109,120,131,
    142,152,162,171,180,189,197,205,212,219,225,231,236,240,244,247,250,252,
    254,255,255,255,254,252,250,247,244,240,236,231,225,219,212,205,197,189,
    180,171,162,152,142,131,120,109,98,86,74,62,50,37,25,13};
    */
    unsigned char sinewave[64]= {0,59,86,106,122,136,148,158,168,177,185,
    192,199,205,210,216,221,225,229,233,236,239,242,245,247,249,250,252,253,
    254,254,255,254,254,253,252,250,249,247,245,242,239,236,233,229,225,221,
    216,210,205,199,192,185,177,168,158,148,136,122,106,86,59};

    printf("%d,", state);

    //most significant nybble is used to choose pin state table
    switch ((state >> 5)&3){ 
	case 0:
	    printf("%d,", sinewave[(state+32)&63]);
	    printf("%d,", sinewave[state&63]);
	    printf("%d,",0);
	    printf("%d\n",0);
	    break;
	case 1:
	    printf("%d,",0);
	    printf("%d,", sinewave[state&63]);
	    printf("%d,", sinewave[(state+32)&63]);
	    printf("%d\n",0);
	    break;
	case 2:
	    printf("%d,",0);
	    printf("%d,",0);
	    printf("%d,", sinewave[(state+32)&63]);
	    printf("%d\n", sinewave[state&63]);
	    break;
	case 3:
	    printf("%d,", sinewave[(state+32)&63]);
	    printf("%d,",0);
	    printf("%d,",0);
	    printf("%d\n", sinewave[state&63]);
	    break;
	default:
	    printf("%d\n",1);
    }//switch
}//ustep




