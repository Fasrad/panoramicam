

#include <stdio.h>
void ustep(unsigned char);

int main(){

    unsigned char state;
    //printf("%s,%s,%s,%s,%s,%s,%s,%s,%s\n","state>>4","state>>4&3","sine&31","sine+1&21","state","coil1","coil2","coil3","coil4");
    printf("%s,%s,%s,%s,%s\n","state","coil1","coil2","coil3","coil4");

    for (int i =0;i<260;i++){
	state ++;
	ustep(state);
    }
    for (int i =0;i<260;i++){
	state --;
	ustep(state);
    }
}//main

    void ustep(unsigned char state){
	unsigned char sinewave[32] = {0, 25, 50, 74, 98, 120, 142, 162, 180,
	197, 212, 225, 236, 244, 250, 254, 255, 254, 250, 244, 236, 225,
	212, 197, 180, 162, 142, 120, 98, 74, 50, 25};
//	printf("%d,", (state>>5));
//	printf("%d,", ((state>>5)&3));
//	printf("%d,", (state&31));
//	printf("%d,", ((state+16)&31));
	printf("%d,", state);
	//write out the PWM compare register values
	switch ((state >> 4)&3){ //X%N = X&(N-1) if X and N are powers of 2
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




