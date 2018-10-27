#include <stdio.h>
#include "date.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>


struct date{
	int day;
	int month;
	int year;
};

Date *date_duplicate(Date *d){
	Date * dateDuplicatePointer = (Date*) malloc(sizeof(int) * 3);
	if(dateDuplicate == NULL){
		return NULL;
	}
	Date dateDuplicate = *dateDuplicatePointer;
	dateDuplicate.day = d->day;
	dateDuplicate.month = d->month;
	dateDuplicate.year = d->year;
	return dateDuplicatePointer;
};

int main(){
	
}

