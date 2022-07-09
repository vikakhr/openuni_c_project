#include "first_step.h"
#include "cmd_check.h"
#include "main_data.h"

/*Receives a pointer to the string and removes blankspaces before and after it. Returns a pointer to the new string without blankspaces by sides*/
char* remove_blanks(char* word){
	int i, start=0, end=0;
	int len;
	char *p = (char *)malloc(len+1);
	if(p==NULL)
		exit(0);
	
	len = strlen(word);
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
int checkCommas (char *word, int line_num){
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

/*Function receives a number and checks if it legal*/
int check_one_num(char *num){				
	char *ptr;
	char *number = (char *)malloc(strlen(num)+1);
	if(number == NULL)
		return ERROR;
	num++; /*pointer to number after #*/
	if(num[0]=='+' || num[0]=='-'){
		num++;
		if(!isdigit(num[0]))
			return ERROR;
	}

	
	strcpy(number,num);
	
	
	strtol(number, &ptr, 10);			

	if(*ptr!='\0' && *ptr!='\n')	
		return ERROR;
			
	free(number);
	return 0;
}

