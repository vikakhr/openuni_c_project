#include "helper_func.h"
#include "main.h"
#include "parse_file.h"
#include "cmd_check.h"


/*Receives a pointer to the string and removes blankspaces before and after it. Returns a pointer to the new string without blankspaces by sides*/
char* remove_blanks(char* word){
	int i, start=0, end=0;
	size_t len = strlen(word);
	char *new_word;

	new_word = (char*)malloc(len+1);
	if(new_word==NULL)
		return NULL;

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
	new_word[end-start+1] = '\0';/*adds \0 to new string*/
	return new_word;
}

/*Function receives command line and line number, and checks commas errors. If no errors returns 0, ERROR otherwise*/
int check_commas (char *line, int line_num){
	int comma = ',';
	int i=0,j=0;	

	if(!strchr(line,comma))/*no commas*/
		return 0;

	if(line[0]==comma){/*comma at the start of command line*/
		printf("Error, extraneous text before command in line number: %d\n", line_num);
		return ERROR;
	}	
	while(line[i]!='\0'){/*iterate char by char*/
		if(line[i]==comma){/*if comma was found*/
			j=i+1;
			while(line[j]!='\0'){
				if(isalnum(line[j])){/*if some text after - ok*/
					i=j;
					break;
				}
				if(line[j]==comma){/*another comma was found  without text before*/
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

/*Function receives command line and line number. Checks typo errors, if ok returns 0, error otherwise*/
int check_typo_errors(char* command, int line_num){
	char *command_copy = remove_blanks(command);/*copy of command without blanks by sides*/
	int len = strlen(command_copy);

	if(command_copy[0] == ';'){/*if this is comment line - ignore and go to next*/
		free(command_copy);
		return ERROR;
	}
	if(ispunct(command_copy[0]) && command_copy[0]!='.'){
		printf("Error, illegal punctuation mark at the beginning of command, in line number: %d\n", line_num);
		free(command_copy);
		return ERROR;
	}

	if(ispunct(command_copy[len-1])){/*if punctuation mark at the end of command*/
		if(!strchr(command_copy, '.')){/*of not directive - error*/
			printf("Error, extraneous punctuation mark at the end of command, in line number: %d\n", line_num);
			free(command_copy);
			return ERROR;
		}
		if(command_copy[len-1]!='"'){/*not a " punctuation mark at the end of command*/
			printf("Error, extraneous punctuation mark at the end of command, in line number: %d\n", line_num);
			free(command_copy);
			return ERROR;
		}
	}
	free(command_copy);
	if((check_commas(command, line_num))==ERROR)/*check consecutive commas*/
		return ERROR;
	return 0;
}

/*Function receives a number and checks if it legal, if ok returns it's value translated to integer, int_max otherwise*/
int check_one_num(char *num){	
	char *ptr;
	long int value;

	char *number = (char *)malloc(strlen(num)+1);
	if(number == NULL)
		return INT_MAX;

	if(num[0]=='#')
		num++; /*pointer to number after #*/
	if(num[0]=='+' || num[0]=='-'){
		num++;
		if(!isdigit(num[0])){/*if contains non digit char*/
			free(number);
			return INT_MAX;
		}
	}

	strcpy(number,num);
	value = strtol(number, &ptr, 10);			
	if(*ptr!='\0' && *ptr!='\n'){/*if number is not legal*/
		free(number);
		return INT_MAX;
	}

	free(number);
	return (int)value;
}

/*Function receives command line, it's number, flag is label. Checks if numbers are legal for .data arguments.
If no errors returns amount of numbers, ERROR otherwise*/
int check_nums(char *line, int isLabel, int line_num){
	char *word; /*for saving separate words*/
	char *ptr, *p; /*pointers for strtok and strtol*/
	int count = 0;/*counter for arguments*/
	long int num; /*for strtol*/
	char *whitespace = " \t\v\f\r";

	char *numbers = (char *)malloc(strlen(line)+1);/*copy of line passed to function*/
	if(numbers == NULL)
		return ERROR;

	strcpy(numbers,line);

	if(isLabel){/*if label in line -  take label and command pointers*/
		p = strtok(numbers, whitespace);
		p = strtok(NULL, whitespace);
	}
	else p = strtok(numbers, whitespace);

	p = strtok(NULL, ",");/*take pointer to the first number*/

	while(p != NULL){
		word = remove_blanks(p);/*removes blank spaces from both sides of num*/
		if(word[0]=='+' || word[0]=='-'){
			if(strlen(word)==1){/*if + or - without value*/
				printf("Error, argument is not an integer, in line number:%d\n", line_num);
				free(numbers);
				free(word);
				return ERROR;
			}

			if(!isdigit(word[1])){
				printf("Error, argument is not an integer, in line number:%d\n", line_num);
				free(numbers);
				free(word);
				return ERROR;
			}
		}
		else if(!isdigit(word[0])){
			printf("Error, argument is not an integer, in line number:%d\n", line_num);
			free(numbers);
			free(word);
			return ERROR;
		}

		strtol(word, &ptr, 10);
		if(*ptr!='\0'){
			if((num = strtol(word, &whitespace,10))!=0){/*error inside 'word' parameter*/
				if(num == 0){
					printf("Error, extraneous text after end of command, in line number:%d\n", line_num);
					free(word);
					free(numbers);
					return ERROR;
				}
			}
			printf("Error, argument is not an integer, in line number:%d\n", line_num);
			free(word);
			free(numbers);
			return ERROR;
		}
		count++;/*if no errors count this argument*/
		free(word);/*free allocated memory by remove blanks*/
		p = strtok(NULL, ",");
	}

	if(!count){/*if instruction without arguments*/
		if(word==NULL){
			printf("Error, missing argument, in line number:%d\n", line_num);
			free(numbers);
			return ERROR;
		}
	}
	free(numbers);
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

	ptr = strtok(NULL, "\n");/*take pointer to the string*/
	new_string = remove_blanks(ptr); /*remove blanks*/
	free(string);
	if(new_string[0] == '"' && new_string[strlen(new_string)-1] == '"'){/*if string has "" by sides*/
		free(new_string);
		return 1;
	}
	free(new_string);
	return ERROR;

}




