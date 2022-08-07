#include "main.h"
#include "parse_file.h"
#include "cmd_check.h"
#include "label_lists.h"
#include "helper_func.h"

/*struct array of opcodes, used in this file only*/
static CmdNames cmd[] ={
	{"mov", 2},
	{"cmp", 2},
	{"add", 2},
	{"sub", 2},
	{"not", 1},
	{"clr", 1},
	{"lea", 2},
	{"inc", 1},
	{"dec", 1},
	{"jmp", 1},
	{"bne", 1},
	{"get", 1},
	{"prn", 1},
	{"jsr", 1},
	{"rts", 0},
	{"hlt", 0},
	{"not_valid",-1}
};

/*Function receives word and line num, checks if word is directive and if no errors.
 * If no errors found returns array index of directive, error otherwise*/
int check_directive_islegal(char *word, int line_num){
	int drctv_index;
	
	if((drctv_index = check_directive_name(word))==ERROR){/*check if first word is directive*/
			if(word[strlen(word)-1] == ','){/*Illegal comma after command name check*/
				printf("Error, illegal comma in line number: %d\n", line_num);
				return ERROR;	
			}
			else {
				printf("Error, undefined directive name in line number: %d\n", line_num);
				return ERROR;
			}
	}
	return drctv_index;
}


/*Function receives label name and line number where label defined. Checks if label name is legal, if is ok returns 1, error otherwise*/
int check_label_islegal(char* label, int line_num){
	int i, regNameLength = 2, cmdNameLength = 3;

	if(strlen(label)>LABELSIZE){/*too long name for label*/
		printf("Error, label name is too long, in line number: %d\n", line_num);
		return ERROR;
	}	

	if(!isalpha(label[0])){/*if first char is not a character*/
		printf("Error, label name is not legal, in line number: %d\n", line_num);
		return ERROR;
	}

	for(i=0; i<strlen(label); i++){/*if punctuation mark inside label name*/
		if(ispunct(label[i])){
			printf("Error, label name is not legal, in line number: %d\n", line_num);
			return ERROR;
		}
	}

	if(strlen(label)==regNameLength){/*if length of label string is two check if is not register name*/
		if(check_arg_register(label)!=ERROR){
				printf("Error, label name has the same name as register, in line number: %d\n", line_num);
				return ERROR;
		}
	}

	if(strlen(label)==cmdNameLength){/*if length of label string is three check if is not opcode name*/
		for(i=0; cmd[i].name!=NULL; i++){
			if(!strcmp(label, cmd[i].name)){
				printf("Error, label name has the same name as opcode, in line number: %d\n", line_num);
				return ERROR;
			} 
		}
	}
	return 1;
}

/*Function receives argiment of opcode. Checks if addressing method is immediate and argument is an integer,
 * if it is returns num of addressing type, otherwise returns max integer value*/
int check_arg_number(char *num){
	char *number;
	int value;
	if(num[0]!='#')
		return INT_MAX;
	number = (char*)malloc(strlen(num)+1);
	if(number==NULL)
		return INT_MAX;
	strcpy(number, num);
	if((value = check_one_num(number))==INT_MAX){
		free(number);
		return INT_MAX;
	}
	free(number);
	return value;
}

/*Function checks if argument is register, if it is returns index of register, otherwise returns error*/
int check_arg_register(char *word){
	char *REGISTER[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
	int size = sizeof(REGISTER)/sizeof(REGISTER[0]);
	int i;
	for(i=0; i<size; i++){
		if(!strcmp(word, REGISTER[i])){
			return i;}
	}
	return ERROR;
}

/*Function checks if argument can be a struct, if it is returns num of addressing type, otherwise returns error*/
int check_struct_arg(char *line, int line_num, int isLabel){
	char *separator = " \t\v\f\r";
	char *ptr;
	char *line_copy = (char*)malloc(strlen(line)+1);
	if(line_copy==NULL)
		return ERROR;

	strcpy(line_copy, line);
	if(isLabel){
		ptr = strtok(line_copy, separator);/*take label*/
		ptr = strtok(NULL, separator);/*take directive name*/
	}
	else 	ptr = strtok(line_copy, separator);

	if((ptr = strtok(NULL, ",")) == NULL){/*if no first argument*/
		printf("Error, missing arguments for struct, in line number: %d\n", line_num);
		free(line_copy);
		return ERROR;
	}

	if((check_one_num(ptr))==INT_MAX){/*if struct field num is not integer*/
		printf("Error, first field of struct is not an integer, in line number: %d\n", line_num);
		free(line_copy);
		return ERROR;
	}

	if((ptr = strtok(NULL, separator))==NULL){/*if no second argument*/
		printf("Error, missing arguments for struct directive, in line number: %d\n", line_num);
		free(line_copy);
		return ERROR;
	}

	if(ptr[0] == '"' && ptr[strlen(ptr)-1] == '"'){/*if string has "" by sides*/
		free(line_copy);
		return 1;
	}
	else {
		printf("Error, string parameter is not legal, in line number: %d\n", line_num);
 		free(line_copy);
		return ERROR;
	}	
}

/*Function receives command, line num, line number and command index. Checks if arguments of given command are right*/
int check_cmd_args(char *command, int line_num, int isLabel, int cmd_index){
	char *cmd_copy, *arg, *source, *destination, *p;
	char *white_space = " \t\v\f\r\n";
	int arg_count = 0, isError = 0;/*count of arguments, flag is error*/
	cmd_copy = (char*)malloc(strlen(command)+1);
	if(cmd_copy==NULL)
		return ERROR;

	strcpy(cmd_copy, command);

	if(isLabel){
		strtok(cmd_copy, white_space);
		strtok(NULL, white_space);
	}
	else 
		strtok(cmd_copy, white_space);

	if((p = strtok(NULL, ","))!=NULL){/*take first argument*/
		if(ispunct(p[0]) && p[0]!='#'){/*if punctuation mark before first argument*/
			printf("Error, extraneous punctuation mark between command and first argument, in line number: %d\n", line_num);
			free(cmd_copy);
			return ERROR;
		}
		arg_count++;
		if((source = remove_blanks(p))==NULL){/*remove blanks, check if malloc inside function not fails*/
			free(cmd_copy);
			return ERROR;
		}

		if(check_arg_errors(source)==ERROR){/*errors check of source*/
			printf("Error, first argument is not legal, in line number: %d\n", line_num);
			free(source);
			free(cmd_copy);
			return ERROR;
		}

		if((p = strtok(NULL, ","))!=NULL){
			if((arg = strtok(NULL, ","))!=NULL){/*if there is third argument in line with opcode*/
				printf("Error, extraneous number of arguments for instruction command, in line number: %d\n", line_num);
				isError = -1;
			}
			arg_count++;
			if((destination = remove_blanks(p))==NULL){/*remove blanks, check if malloc inside function not fails*/
				free(cmd_copy);
				return ERROR;
			}

			if(check_arg_errors(destination)==ERROR){/*errors check of destination*/
				printf("Error, second argument is not legal, in line number: %d\n", line_num);
				free(source);
				free(destination);
				free(cmd_copy);
				return ERROR;
			}
		}
		else
			destination = source; /*if just one argument in line - it is destination*/
	}
	if(arg_count < cmd[cmd_index].args && !isError){/*if num of arguments is less than need for opcode*/
		if(!strchr(command, ','))
			printf("Error, missing comma between arguments, in line number: %d\n", line_num);
		else printf("Error, missing arguments for instruction command, in line number: %d\n", line_num);
		isError = -1;
	}
	if((arg_count > cmd[cmd_index].args) && !isError){/*if more arguments than need for opcode*/
		printf("Error, extraneous number of arguments for instruction command, in line number: %d\n", line_num);
		isError = -1;
	}
	if(!isError){
		/*Check if addressing type of arguments is legal for specific opcode by opcode index*/
		switch(cmd_index){/*switch by command index of struct*/
			case 0: if(check_arg_number(destination)!=INT_MAX){/*if destination is number not legal*/
					printf("Error, destination argument is not legal, in line number: %d\n", line_num);
					isError = -1;
					break;
				}
				break;
			case 1:	break;
			case 2:	
			case 3:	
				
			case 4:
						
			case 5: if(check_arg_number(destination)!=INT_MAX){/*if destination is number - not legal*/
					printf("Error, destination argument is not legal, in line number: %d\n", line_num);
					isError = -1;
					break;
				}
				break;
			case 6:
				if(check_arg_register(source)!=ERROR){/*if source is register - not legal*/
					printf("Error, source argument is not legal, in line number: %d\n", line_num);
					isError = -1;
					break;
				}
				if(check_arg_number(source)!=INT_MAX){/*if source is number - not legal*/
					printf("Error, source argument is not legal, in line number: %d\n", line_num);
					isError = -1;
					break;
				}
				if(check_arg_number(destination)!=INT_MAX){/*if destination is number - not legal*/
					printf("Error, destination argument is not legal, in line number: %d\n", line_num);
					isError = -1;
					break;
				}
				break;
			case 7: 
			case 8:
			case 9:
			case 10:
			case 11: if(check_arg_number(destination)!=INT_MAX){/*if destination is number - not legal*/
					printf("Error, destination argument is not legal, in line number: %d\n", line_num);
					isError = -1;
					break;
					}
				break;
			case 12: break;
			case 13: if(check_arg_number(destination)!=INT_MAX){/*if destination is number - not legal*/
					printf("Error, destination argument is not legal, in line number: %d\n", line_num);
					isError = -1;
					break;
				}	
				break;
			case 14: 
			case 15:

			break;
		}/*end of switch*/
	}/*end of if*/

	free(cmd_copy);
	if(arg_count==1)
		free(destination);
	if(arg_count==2){
		free(source);
		free(destination);
	}

	if(!isError)
		return 1;
	else return ERROR;
}


/*Receives word and checks if it is directive. If yes returns it's index, ERROR otherwise*/
int check_cmd_name(char *word){
	int cmd_index;	
	for(cmd_index=0; cmd[cmd_index].name!=NULL;cmd_index++){
		if(!strcmp(word,cmd[cmd_index].name)){
			return cmd_index;
		}
	}
	return ERROR;	
}

/*Receives word and checks if it is opcode. If yes returns it's index, ERROR otherwise*/
int check_directive_name(char *word){
	char *DIRECTIVE[] = {".data",".string",".struct",".entry",".extern"};
	int i;	
	int size = sizeof(DIRECTIVE)/sizeof(DIRECTIVE)[0];/*number of words in array*/
	for(i=0; i<size; i++)
		if(!strcmp(word, DIRECTIVE[i]))
			return i;
	return ERROR;	
}

/*Receives argument of opcode, checks errors. If argument is not legal return error, 1 is ok*/
int check_arg_errors(char *arg){
	if(isdigit(arg[0]))/*if number without # - not legal*/
		return ERROR;

	if(strchr(arg, '.')){/*if possibly struct checks it's field number*/
		if(arg[strlen(arg)-1] != '1' && arg[strlen(arg)-1] != '2'){
			return ERROR;
		}
	}
	if(strchr(arg, ' ') || strchr(arg, '\t'))/*if argument contains whitespaces*/
		return ERROR;

	return 1;
}



