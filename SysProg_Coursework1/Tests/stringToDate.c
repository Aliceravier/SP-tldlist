#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>


typedef struct date{
	int day;
	int month;
	int year;
}Date;


Date createDateFromString(char *datestr);

int main(){
	Date oneDate = createDateFromString("12/11/2000");	
	Date twoDate = createDateFromString("02/00/1997");
	printf("%d, %d, %d\n", oneDate.day, oneDate.month, oneDate.year);
	printf("%d, %d, %d\n", twoDate.day, twoDate.month, twoDate.year);
}

Date createDateFromString(char *datestr){
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
	
	Date createdDate;
	createdDate.day = atoi(strDay);
	createdDate.month = atoi(strMonth);
	createdDate.year = atoi(strYear);
	return createdDate;
}
