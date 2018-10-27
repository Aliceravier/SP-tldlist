#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>


bool isCorrectDateForm(char *datestr);

int main(){
	printf("%d\n",isCorrectDateForm("12/15/1997"));
	printf("%d\n",isCorrectDateForm("12/15/197"));
	printf("%d\n",isCorrectDateForm("12/1501997"));
	printf("%d\n",isCorrectDateForm("12/99/1997"));
}

bool isCorrectDateForm(char *datestr){
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

