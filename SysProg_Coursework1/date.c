#include "date.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

struct date{
	int day;
	int month;
	int year;
};

int convertToDays(Date *date);

bool isCorrectDateForm(char *datestr);
Date * createDateFromString(char *datestr);

/*int main(){
	Date * validDate = date_create("12/10/2003");
	Date * validDate2 = date_create("13/10/2003");
	Date * validDate3= date_create("24/10/2001");

	Date * invalidDateA = date_create("000");
	Date * invalidDateB = date_create("00/0331997");
	printf("%d, %d, %d\n", validDate->day, validDate->month, validDate->year);
	if(invalidDateA == NULL){
		printf("it worked\n");
	}
	if(invalidDateB == NULL){
		printf("it worked again !!\n");
	}

	Date * dupDate = date_duplicate(validDate);
	printf("%d, %d, %d\n", dupDate->day, dupDate->month, dupDate->year);	
	
	printf("Should be positive : %d\n", date_compare(validDate, validDate3));
	printf("Should be negative: %d\n", date_compare(validDate, validDate2));
	printf("Should be zero: %d\n", date_compare(validDate, validDate));

	date_destroy(validDate); //TODO test that this works
	date_destroy(validDate2);
	date_destroy(validDate3);

}*/

/*
 *  * date_create creates a Date structure from `datestr`
 *   * `datestr' is expected to be of the form "dd/mm/yyyy"
 *    * returns pointer to Date structure if successful,
 *     *         NULL if not (syntax error)
 *      */
Date *date_create(char *datestr){
	if(isCorrectDateForm(datestr)){
		return createDateFromString(datestr);
	}
	else
		return NULL;
};

bool isCorrectDateForm(char *datestr){
//TODO check if need to check for legit date not just format
	if(strlen(datestr) != 10){
		return false;
	}
	if(!isdigit(datestr[0])){
		return false;
	}
	if(!isdigit(datestr[1])){
		return false;
	}
	if(datestr[2] != '/'){
		return false;
	}		
	if(!isdigit(datestr[3])){
		return false;
	}	
	if(!isdigit(datestr[4])){
		return false;
	}
	if(datestr[5] != '/'){
		return false;
	}	
	if(!isdigit(datestr[6])){
		return false;
	}	
	if(!isdigit(datestr[7])){
		return false;
	}	
	if(!isdigit(datestr[8])){
		return false;
	}	
	if(!isdigit(datestr[9])){
		return false;
	}			
	return true;
}

Date * createDateFromString(char *datestr){
	char strDay[3];
	strDay[0] = datestr[0];
	strDay[1] = datestr[1];

	char strMonth[3];
	strMonth[0] = datestr[3];
	strMonth[1] = datestr[4];

	char strYear[5];
	strYear[0] = datestr[6];
	strYear[1] = datestr[7];
	strYear[2] = datestr[8];
	strYear[3] = datestr[9];
	
	Date * createdDatePointer = (Date*) malloc(sizeof(Date));
	createdDatePointer->day = atoi(strDay);
	createdDatePointer->month = atoi(strMonth);
	createdDatePointer->year = atoi(strYear);
	return createdDatePointer;
}

/*
 *  * date_duplicate creates a duplicate of `d'
 *   * returns pointer to new Date structure if successful,
 *    *         NULL if not (memory allocation failure)
 *     */
Date *date_duplicate(Date *d){
	Date * dateDuplicatePointer = (Date*) malloc(sizeof(Date));
	if(dateDuplicatePointer == NULL){
		return NULL;
	}
	dateDuplicatePointer->day = d->day;
	dateDuplicatePointer->month = d->month;
	dateDuplicatePointer->year = d->year;
	return dateDuplicatePointer;
};

/*
 *  * date_compare compares two dates, returning <0, 0, >0 if
 *   * date1<date2, date1==date2, date1>date2, respectively
 *    */
int date_compare(Date *date1, Date *date2){
	return convertToDays(date1) - convertToDays(date2);
};

int convertToDays(Date *date){
	return date->day + date->month*30 + date->year*365;
}

/*
 *  * date_destroy returns any storage associated with `d' to the system
 *   */
void date_destroy(Date *d){
	free(d);
};
