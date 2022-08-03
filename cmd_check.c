#include "main.h"
#include "first_step.h"
#include "cmd_check.h"
#include "label_lists.h"
#include "helper_func.h"

/*struct array of opcodes*/
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


int check_directive_islegal(char *word, int line_num){
	int drctv_index;
	
	if((drctv_index = check_directive(word))==ERROR){/*check if first word is directive*/
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





/*Function checks if argument is an integer, if it is returns num of addressing type, otherwise returns error*/
int check_arg_number(char *num){
	if(num[0]!='#')
		return ERROR;
	return check_one_num(num);
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

	if((check_one_num(ptr))==ERROR){/*if struct field num is not integer*/
		printf("Error, first field of struct is not integer, in line number: %d\n", line_num);	
		free(line_copy);
		return ERROR;
	}

	if((ptr = strtok(NULL, separator))==NULL){/*if no second argument*/
		printf("Error, missing arguments for struct, in line number: %d\n", line_num);
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



/*Function receives command, line num, command and struct of instruction commands, and checks if arguments of given command are right*/
int check_cmd_args(char *command, int line_num, int isLabel, int cmd_index){
	char *cmd_copy, *arg, *source, *destination, *p;
	char *white_space = " \t\v\f\r\n";
	int arg_count = 0, isError = 0;
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

	if((p = strtok(NULL, ","))!=NULL){
		if(ispunct(p[0]) && p[0]!='#'){
			printf("Error, extraneous punctuation mark between command and first argument, in line number: %d\n", line_num);
			free(cmd_copy);
			return ERROR;
		}
		arg_count++;
		source = remove_blanks(p);
		if(check_operand_errors(source)==ERROR){
			printf("Error, source parameter is not legal, in line number: %d\n", line_num);
			free(source);
			free(cmd_copy);
			return ERROR;
		}

		if((p = strtok(NULL, ","))!=NULL){
			if((arg = strtok(NULL, ","))!=NULL){
				printf("Error, extraneous number of operands for instruction command, in line number: %d\n", line_num);
				isError = -1;
			}
			arg_count++;
			destination = remove_blanks(p);
			if(check_operand_errors(destination)==ERROR){
				printf("Error, destination parameter is not legal, in line number: %d\n", line_num);
				free(source);
				free(destination);
				free(cmd_copy);
				return ERROR;
			}
		}
		else
			destination = source;
	}
	
	

	if(arg_count < cmd[cmd_index].args && !isError){
		if(!strchr(command, ','))
			printf("Error, missing comma between operands, in line number: %d\n", line_num);
		else printf("Error, missing arguments for instruction command, in line number: %d\n", line_num);
		isError = -1;
	}
	if((arg_count > cmd[cmd_index].args) && !isError){
		printf("Error, extraneous number of arguments for instruction command, in line number: %d\n", line_num);
		isError = -1;
	}
	if(!isError){
		switch(cmd_index){/*switch by func index of struct*/
			case 0: if(check_arg_number(destination)!=ERROR){/*if destination is number - ERROR*/
					printf("Error, destination parameter is not legal, in line number: %d\n", line_num);
					isError = -1;
					break;
				}
				break;
			case 1:	break;
			case 2:	
			case 3:	
				
			case 4:
						
			case 5: if(check_arg_number(destination)!=ERROR){/*if destination is number - ERROR*/
					printf("Error, destination parameter is not legal, in line number: %d\n", line_num);
					isError = -1;
					break;
				}
				break;
			case 6:
				if(check_arg_register(source)!=ERROR){/*if source is register - ERROR*/
					printf("Error, source parameter is not legal, in line number: %d\n", line_num);
					isError = -1;
					break;
				}
				if(check_arg_number(source)!=ERROR){/*if source is number - ERROR*/
					printf("Error, source parameter is not legal, in line number: %d\n", line_num);
					isError = -1;
					break;
				}
				if(check_arg_number(destination)!=ERROR){/*if destination is number - ERROR*/
					printf("Error, destination parameter is not legal, in line number: %d\n", line_num);
					isError = -1;
					break;
				}
				break;
			case 7: 
			case 8:
			case 9:
			case 10:
			case 11: if(check_arg_number(destination)!=ERROR){/*if destination is number - ERROR*/
					printf("Error, destination parameter is not legal, in line number: %d\n", line_num);
					isError = -1;
					break;
					}
				break;
			case 12: break;
			case 13: if(check_arg_number(destination)!=ERROR){/*if destination is number - ERROR*/
					printf("Error, destination parameter is not legal, in line number: %d\n", line_num);
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


/*Receives pointers to the word and array of command names. Checks if command is legal. If legal returns it's index, ERROR otherwise*/
int check_cmd(char *word){
	int cmd_index;	
	for(cmd_index=0; cmd[cmd_index].name!=NULL;cmd_index++){
		if(!strcmp(word,cmd[cmd_index].name)){
			return cmd_index;
		}
	}
	return ERROR;	
}

/*Receives pointers to the word and array of command names. Checks if command is legal. If legal returns it's index, ERROR otherwise*/
int check_directive(char *word){
	char *DIRECTIVE[] = {".data",".string",".struct",".entry",".extern"};
	int i;	
	int size = sizeof(DIRECTIVE)/sizeof(DIRECTIVE)[0];/*number of words in array*/
	for(i=0; i<size; i++)
		if(!strcmp(word, DIRECTIVE[i]))
			return i;
	return ERROR;	
}

int check_operand_errors(char *operand){
	if(isdigit(operand[0]))
		return ERROR;

	if(strchr(operand, '.')){/*if struct checks field number*/
		if(operand[strlen(operand)-1] != '1' && operand[strlen(operand)-1] != '2'){
			return ERROR;
		}
	}

	return 1;
}



