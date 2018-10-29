#include <stdio.h>
#include <string.h>
#include <ctype.h>
 
char *strlwr(char *str){
	int i = 0;	
	while(str[i]){
		str[i] = tolower(str[i]);
	i++;
	}
return str;
}

int main()
{
   char string[1000];
   
   printf("Input a string to convert to lower case\n");
   fgets(string, 100, stdin);
   
   printf("The string in lower case: %s\n", strlwr(string));
   
   return  0;
}
