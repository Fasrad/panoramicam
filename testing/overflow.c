

#include <stdio.h>

int main(){

    int8_t sinewave[32] = {0, 25, 50, 74, 98, 120, 142, 162, 180,\
    197, 212, 225, 236, 244, 250, 254, 255, 254, 250, 244, 236, 225,\
    212, 197, 180, 162, 142, 120, 98, 74, 50, 25};
    unsigned char state;
    printf("%s\t%s\t%s\t%s\n",
    "state%32\t","state+16%32\t","state>>4%4\t","state\n");

    for (int i =0;i<260;i++){
	printf("%d\t", state%32);
	printf("%d\t", (state+16)%32);
	printf("%d\t", (state>>4)%4);
	printf("%d\n", state);
	state ++;
    }
    for (int i =0;i<260;i++){
	printf("%d\t", state%32);
	printf("%d\t", (state+16)%32);
	printf("%d\t", (state>>4)%4);
	printf("%d\n", state);
	state --;
    }
}



