

#include <stdio.h>

int main(){

    unsigned char index;

    for (int i =0;i<257;i++){
	printf("index %d\n", index);
	printf("index mod32 %d\n", index%32);
	index ++;
    }
    for (int i =0;i<257;i++){
	printf("index %d\n", index);
	printf("index mod32 %d\n", index%32);
	index --;
    }
}



