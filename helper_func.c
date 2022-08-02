#include "helper_func.h"
#include <limits.h> /*for char_max and char_min*/
#include "main.h"
#include "first_step.h"
#include "cmd_check.h"


/*Receives a pointer to the string and removes blankspaces before and after it. Returns a pointer to the new string without blankspaces by sides*/
char* remove_blanks(char* word){
	int i, start=0, end=0;
	int len;
	char *new_word;

	len = strlen(word);
	new_word = (char *)malloc(len+1);
	if(new_word==NULL)
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
	memcpy(new_word,&word[start],end-start+1);/*copy string*/
	new_word[end-start+1] = '\0';

	return new_word;
}

/*Receives a pointer to the whole command line string and checks commas errors. If no errors returns 0, ERROR otherwise*/
int check_commas (char *word, int line_num){
	int comma = ',';
	int i=0,j=0;	
	if(strchr(word,comma) == NULL){/*no commas*/
		return 0;
	}
	if(word[0]==comma){
		printf("Error, extraneous text before command in line number: %d\n", line_num);
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
					printf("Error, multiple consecutive commas in line number: %d\n", line_num);
					return ERROR;
				}
				j++;
			}
		}
		i++;	
	}
	return 0;
}

/*Function receives command line, it's length and line number and checks typo errors, if ok returns 0, error otherwise*/
int line_typo_errors_check(char* command, int line_num, int length){
	
	if(command[0] == ';')/*if this is comment line - ignore and go to next*/
		return ERROR;

	if(ispunct(command[0]) && command[0]!='.'){
		printf("Error, illegal punctuation mark at the beginning of command, in line number: %d\n", line_num);
		return ERROR;
	}

	if(ispunct(command[length-1])){/*if punctuation mark at the end of command*/
		if(!strchr(command, '.')){/*of not directive - error*/
			printf("Error, extraneous punctuation mark at the end of command, in line number: %d\n", line_num);
			return ERROR;
		}
		if(command[length-1]!='"'){/*not a " punctuation mark at the end of command*/
			printf("Error, extraneous punctuation mark at the end of command, in line number: %d\n", line_num);
			return ERROR;
		}
	}
	if((check_commas(command, line_num))==ERROR)/*check consecutive commas*/
		return ERROR;
	return 0;
}



/*Function receives a number and checks if it legal, if ok returns it's value translated to integer, error otherwise*/
int check_one_num(char *num){	
	char *ptr;
	long int value;
	char *number = (char *)malloc(strlen(num)+1);
	if(number == NULL)
		return ERROR;

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

/*Receives pointer to the command line flah i label inside line and line number and checks if numbers are legal for .data operands.
If no errors returns amount of numbers, ERROR otherwise*/
int check_nums(char *line, int isLabel, int line_num){
	char *word;	
	char *ptr, *p;
	int count = 0;
	long int num;
	char *separator = " \t\v\f\r";
	char *numbers = (char *)malloc(strlen(line)+1);
	if(numbers == NULL)
		return ERROR;

	strcpy(numbers,line);
	if(isLabel){/*if label in line -  take label and command pointers*/
		p = strtok(numbers, separator);
		p = strtok(NULL, separator);
	}
	else p = strtok(numbers, separator);

	p = strtok(numbers, ",");/*take pointer to the first number*/
	while(p !=NULL){
		
		word = remove_blanks(p);/*removes blank spaces from both sides of num*/
		if(word[0]=='+' || word[0]=='-'){
			word++;
			if(!isdigit(word[0])){
				printf("Error, operand is not an integer, in line number:%d\n", line_num);
				free(numbers);
				free(word);
				return ERROR;
			}
		}
		
		strtol(word, &ptr, 10);		
		if(*ptr!='\0'){
			if((num = strtol(word, &separator,10))!=0){/*error inside 'word' parameter*/
				if(num == 0){
					printf("Error, etraneous text after end of command, in line number:%d\n", line_num);
					free(word);
					free(numbers);
					return ERROR;
				}
				else {
					printf("%s\n", word);			
					printf("Error, operand is not an integer number, in line number:%d\n", line_num);
					free(word);
					free(numbers);
					return ERROR;
				}
			}
		}
		count++;
		p = strtok(NULL, ",");
	}

	if(!count){
		if(word==NULL){
			printf("Error, missing operand, in line number:%d\n", line_num);
			free(numbers);
			return ERROR;
		}
	}
	free(numbers);	
	free(word);
	return count;
}

/*Function receives command line and flag if there is label. Checks if string operand is legal and returns 1, ERROR otherwise*/
int check_string_islegal(char* line, int isLabel){
	char *separator = " \t\v\f\r";
	char *ptr, *new_string;
	char *string = (char *)malloc(strlen(line)+1);
	if(string == NULL)
		return ERROR;

	strcpy(string, line);

	if(isLabel){/*if label in line -  take label and command pointers*/
		ptr = strtok(string, separator);
		ptr = strtok(NULL, separator);
	}
	else ptr = strtok(string, separator);

	ptr = strtok(NULL, separator);/*take pointer to the string*/
	new_string = remove_blanks(ptr); /*remove blanks*/
	free(string);
	if(new_string[0] == '"' && new_string[strlen(new_string)-1] == '"'){/*if string has "" by sides*/
		free(new_string);
		return 1;
	}
	free(new_string);
	return ERROR;

}




