#include "basehead.h"
#include <stdio.h>


int functionA(){
 	int result = functionB();
	return result;
}

int functionB(){
	return 3;
}
