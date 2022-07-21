#include <limits.h> /*for char_max and char_min*/
#include "main.h"
#include "first_step.h"
#include "cmd_check.h"


/*Receives a pointer to the string and removes blankspaces before and after it. Returns a pointer to the new string without blankspaces by sides*/
char* remove_blanks(char* word){
	int i, start=0, end=0;
	int len;
	char *p; 
	
	len = strlen(word);
	p = (char *)malloc(len+1);
	if(p==NULL)
		exit(0);

	for(i=0; i<len; i++){/*start index of word*/
		if(!isspace(word[i])){
			start=i;	
			break;
		}

	}
	for(i=len-1; i>=0; i--){/*end index of word*/
		if(!isspace(word[i])){
			end=i;	
			break;
		}
	}
	memcpy(p,&word[start],end-start+1);/*copy string*/		
	p[end-start+1] = '\0';	
	word=(char *)realloc(p,strlen(p));/*reallocate to the source string and free p*/
	return word;			
}

/*Receives a pointer to the whole command line string and checks commas errors. If no errors returns 0, ERROR otherwise*/
int check_commas (char *word, int line_num){
	int comma = ',';
	int i=0,j=0;	
	if(strchr(word,comma) == NULL){/*no commas*/
		return 0;
	}
	if(word[0]==comma){
		printf("Extraneous text before command in line number: %d\n", line_num);
		return ERROR;
	}	
	while(word[i]!='\0'){
		if(word[i]==comma){/*if comma was found*/
			j=i+1;
			while(word[j]!='\0'){
				if(isalnum(word[j])){/*if some text after - ok*/
					i=j;
					break;
				}
				if(word[j]==comma){/*another comma was found  without text before*/
					printf("Multiple consecutive commas in line number: %d\n", line_num);
					return ERROR;
				}
				j++;
			}
		}
		i++;	
	}
	return 0;
}

/*Function receives command line and line number and checks typo errors, if there is error in line returns ERROR, 0 otherwise*/
int line_typo_errors_check(char* command, int line_num){
	
	if(strlen(command)==1 && command[0]==' ')/*check if line is all whitespaces*/
		return ERROR;

	if(command[0] == ';')/*if this is comment line - ignore and go to next*/
		return ERROR;

	if(ispunct(command[strlen(command)-1]) && (command[strlen(command)-1]!='"')){/*not a " punctuation mark at the end of command*/ 
		printf("Extraneous punctuation mark at the end of line, in line number: %d\n", line_num);
		return ERROR;
	}

	if((check_commas(command, line_num))==ERROR)/*check consecutive commas*/
		return ERROR;
	return 0;
}



/*Function receives a number and checks if it legal*/
int check_one_num(char *num){	
	char *ptr;
	long int value;
	char *number = (char *)malloc(strlen(num)+1);
	if(number == NULL)
		return ERROR;
	printf("Inside check num: %s\n", num);
	if(num[0]!='#'){
		free(number);
		return ERROR;
	}
	num++; /*pointer to number after #*/
	if(num[0]=='+' || num[0]=='-'){
		num++;
		if(!isdigit(num[0])){
			free(number);
			return ERROR;
		}
	}

	
	strcpy(number,num);

	value = strtol(number, &ptr, 10);			
	if(*ptr!='\0' && *ptr!='\n'){
		free(number);
		return ERROR;
	}
	if(value<CHAR_MIN || value>CHAR_MAX){/*if number is beyond the limits of signed 8 bits - can't be translated to machine code*/
		free(number);
		return ERROR;
	}
		
	free(number);
	return (int)value;
}

/*Receives pointer to the command line string and checks if numbers are legal for .data arguments. If no errors returns amount of numbers, ERROR otherwise*/
int check_nums(char *line, int isLabel){				
	char *word;	
	char *ptr;
	int count = 0;
	long int num;
	char *separator = " \t\v\f\r";
	char *numbers = (char *)malloc(strlen(line)+1);
	if(numbers == NULL)
		return ERROR;

	strcpy(numbers,line);
	if(isLabel){/*if label in line -  take label and command pointers*/
		word = strtok(numbers, separator);
		word = strtok(NULL, separator);
	}
	else word = strtok(numbers, separator);

	word = strtok(numbers, ",");/*take pointer to the first number*/
	while(word !=NULL){
		
		word = remove_blanks(word);/*removes blank spaces from both sides of num*/
		printf("After removing blanks: %s\n", word);
		if(word[0]=='+' || word[0]=='-'){
			word++;
			if(!isdigit(word[0])){
				printf("Argument is not an integer\n");
				return ERROR;
			}
		}
		printf("After moving +-: %s\n", word);
		
		strtol(word, &ptr, 10);		
		if(*ptr!='\0'){
			if((num = strtol(word, &separator,10))!=0){/*error inside 'word' parameter*/
				if(num == 0){
					printf("Extraneous text after end of command\n");
					return ERROR;
				}
				else {
					printf("%s\n", word);			
					printf("Argument is not an integer\n");
					return ERROR;
				}
			}
		}
		count++;
		word = strtok(NULL, ",");			
	}

	if(!count){
		if(word==NULL){
			printf("Missing argument\n");
			return ERROR;
		}
	}
	printf("Is ok\n");
	free(numbers);	
	return count;
}

/*Function checks if string is legal and returns 1, ERROR otherwise*/
int check_string_islegal(char* line, int isLabel){
	char *separator = " \t\v\f\r";
	char *string = (char *)malloc(strlen(line)+1);
	if(string == NULL)
		return ERROR;

	strcpy(string, line);

	if(isLabel){/*if label in line -  take label and command pointers*/
		string = strtok(string, separator);
		string = strtok(NULL, separator);
	}
	else string = strtok(string, separator);

	string = strtok(NULL, separator);/*take pointer to the string*/
	string = remove_blanks(string); /*remove blanks*/

	if(string[0] == '"' && string[strlen(string)-1] == '"'){/*if string has "" by sides*/
		free(string);
		return 1;
	}
	else {
		free(string); 
		return ERROR; 
	}

}




