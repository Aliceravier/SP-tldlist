#include <string.h>
#include <stdio.h>


void printLenString(char *str);

int main(){
	char * myString = "Alice";
	char a[20]="Lol";
	printf("%lu\n", strlen(myString));
	printf("%lu\n", strlen(a));
	printLenString(myString);
}


void printLenString(char * str){
	printf("%lu\n", strlen(str));
	}
