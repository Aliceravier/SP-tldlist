#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


char *strlwr(char *str){
	char * newStr;
	newStr=(char*) malloc((strlen(str)+1)*sizeof(char));
	for(int i=  0; i<strlen(str); i++){			
		newStr[i] = tolower(str[i]);
	}
return newStr;
}




int main(){
	printf("%c\n", tolower('.'));
	char hostname[11] = "Alice.com";
	char * parsed = strlwr(strrchr(hostname, '.') + 1); //+1 to eliminate the dot
	printf("%s\n", parsed);
}
