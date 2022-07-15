#include "main.h"
#include "first_step.h"
#include "cmd_check.h"
#include "label_lists.h"


int check_directive_islegal(char *word, int line_num){
	int drctv_index;
	
	if((drctv_index = check_directive(word))==ERROR){/*check if first word is directive*/
			if(word[strlen(word)-1] == ','){/*Illegal comma after command name check*/
				printf("Illegal comma in line number: %d\n", line_num);
				return ERROR;	
			}
			else {
				printf("Undefined directive name in line number: %d\n", line_num);
				return ERROR;
			}
	}
	return drctv_index;
}



int check_label_islegal(char* label, int line_num){
	int i, regNameLength = 2, cmdNameLength = 3;

	if(strlen(label)>LABELSIZE){/*too long name for label*/
		printf("Label name is too long, in line number: %d\n", line_num);
		return ERROR;
	}	
	if(!isalpha(label[0])){/*if first char is not a character*/
		printf("%s\n", label);
		printf("Label name is not legal, in line number: %d\n", line_num);
		return ERROR;
	}

	if(strlen(label)==regNameLength){/*if length of label string is two check if is not register name*/
		for(i=0; i<REGLENGTH; i++)
			if(!strcmp(label, REGISTER[i])){
				printf("Error, label name has the same name as register in line number: %d\n", line_num);
				return ERROR;
			}
	}

	if(strlen(label)==cmdNameLength){/*if length of label string is three check if is not opcode name*/
		for(i=0; i<CMDLENGTH; i++)
			if(!strcmp(label, OPCODE[i])){
				printf("Error, label name has the same name as opcode in line number: %d\n", line_num);
				return ERROR;
			} 
	}
	return 1;
}





/*Function checks if argument is an integer, if it is returns num of addressing type, otherwise returns -1*/
int check_arg_number(char *num){
	if(num[0]!='#')
		return ERROR;
	else 
	return check_one_num(num);
}




/*Function checks if argument is register, if it is returns num of addressing type, otherwise returns -1*/
int check_arg_register(char *word){
	int i;
	for(i=0; i<REGLENGTH; i++){
		if(!strcmp(word, REGISTER[i]))
			return 3;
	}
	return ERROR;
}



/*Function receives command, line num, command type and struct of opcodes, and checks if arguments of given command are right*/
int check_cmd_args(char *command, int line_num, int type, struct CmdNames *cmd){
	printf("Inside check_cmd_args for: %s - %d\n", command, type);
	char *label, *opcode, *arg, *source, *destination;
	char *white_space = " \t\v\f\r\n";
	int arg_count = 0, cmd_index;
	
	if(type == LABEL_CMD){
		label = strtok(command, white_space);
		opcode = strtok(NULL, white_space);
	}
	else 
		opcode = strtok(command, white_space);
	
	cmd_index = check_cmd(opcode, cmd);/*index of opcode*/

	while((arg = strtok(NULL, ","))!=NULL){
		printf("Argument is: %s\n", arg);
		arg = remove_blanks(arg);
		if((++arg_count) == 1)
			source = arg;
		else if((arg_count) == 2)
			destination = arg;
		else {
			printf("Error, extraneous number of arguments for opcode, in line number: %d\n", line_num);
			return ERROR;
		}
	}
	if(arg_count < cmd[cmd_index].args){
		printf("Error, missing arguments for opcode, in line number: %d\n", line_num);
		return ERROR;
	}
	else if(arg_count > cmd[cmd_index].args){
		printf("Error, extraneous number of arguments for opcode, in line number: %d\n", line_num);
		return ERROR;
	}
	else {
		
	}/*end of else*/

	/*add label to linked list of labels*/
	/*if all is ok go to funct add node to linked list of opcodes*/
	return ERROR;
}




/*Function receives source and destination adressing type and checks if they are legal for first group of opcodes*/
int check_first_group(char *source, char *dest, int line_num){
	if(check_one_num(dest)!=ERROR)/*if destination is number - ERROR*/
		return ERROR;

	if(check_one_num(source)!=ERROR){/*if source is number - OK*/
		printf("Source is a number - OK\n");
	}
	else if(check_arg_register(source)!=ERROR)/*if source is register - OK*/
		return 3;
	else if(check_arg_struct(source, line_num)!=ERROR)/*if source is struct - OK*/
		return 2;
 
	if(check_arg_register(dest)!=ERROR)/*if destination is register - OK*/
		return 3;
	


	return ERROR;
}

/*Function receives destination adressing type and checks if it's legal for second group of opcodes*/
int check_second_group(char *dest){
	if(check_one_num(dest)!=ERROR)/*if destination is number - ERROR*/
		return ERROR;
	
	if(check_arg_register(dest)!=ERROR)/*if destination is register - OK*/
		return 3;

	
	return ERROR;
}


/*Function receives source and destination adressing type and checks if they are legal for cmp opcode*/
int check_cmp(char *source, char *dest){
	if(check_one_num(source)!=ERROR)/*if source is number - OK*/
		return 1;
	if(check_one_num(dest)!=ERROR)/*if destination is number - OK*/
		return 1;
	return ERROR;
}

/*Function receives source and destination adressing type and checks if they are legal for lea opcode*/
int check_lea(char *source, char *dest){
	if(check_one_num(source)!=ERROR)/*if source is number - ERROR*/
		return ERROR;
	if(check_one_num(dest)!=ERROR)/*if destination is number - ERROR*/
		return ERROR;
	if(check_arg_register(source)!=ERROR)/*if source is register - ERROR*/
		return ERROR;	
	if(check_arg_register(dest)!=ERROR)/*if destination is register - OK*/
		return 3;
	return ERROR;
}

/*Function receives destination adressing type and checks if it's legal for prn opcode*/
int check_prn(char *dest){
	if(!check_one_num(dest))/*if destination is number - OK*/
		return 1;
	return ERROR;
}

/*Receives pointers to the word and array of command names. Checks if command is legal. If legal returns it's index, ERROR otherwise*/
int check_cmd(char *word, struct CmdNames *cmd){
	int cmd_index;	
	for(cmd_index=0; cmd[cmd_index].name!=NULL;cmd_index++){
		if(strcmp(word,cmd[cmd_index].name)==0){
			printf("Command index is: %d\n", cmd_index);
			return cmd_index;
		}
	}
	return ERROR;	
}

/*Receives pointers to the word and array of command names. Checks if command is legal. If legal returns it's index, ERROR otherwise*/
int check_directive(char *word){
	int i;	
	for(i=0; i<DRCTVNUM; i++)
		if(!strcmp(word, DIRECTIVE[i]))
			return i;
	return ERROR;	
}





