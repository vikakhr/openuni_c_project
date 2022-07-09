#include "main.h"
#include "main_data.h"
#include "first_step.h"
#include "cmd_check.h"




int check_label_islegal(char* label, char* afterLabel, int line_num){
	int i, regNameLength = 2, cmdNameLength = 3;

	if(strlen(label)>LABELSIZE){/*too long name for label*/
		printf("Label name is too long, in line number: %d\n", line_num);
		return 0;
	}	
	if(!isalpha(label[0])){/*if first char is not a character*/
		printf("Label name is not legal, in line number: %d\n", line_num);
		return 0;
	}

	if(strlen(label)==regNameLength){/*if length of label string is two check if is not register name*/
		for(i=0; i<REGLENGTH; i++)
			if(!strcmp(label, REGISTER[i])){
				printf("Error, label name has the same name as register in line number: %d\n", line_num);
				return 0;
			}
	}

	if(strlen(label)==cmdNameLength){/*if length of label string is two check if is not opcode name*/
		for(i=0; i<CMDLENGTH; i++)
			if(!strcmp(label, OPCODE[i])){
				printf("Error, label name has the same name as opcode in line number: %d\n", line_num);
				return 0;
			} 
	}

	if(!strcmp(afterLabel,".entry"))/*if label has .entry after, ignore*/
		return 0;
	if(!strcmp(afterLabel,".extern"))/*if label has .extern after, ignore*/
		return 0;
	
	
	printf("Labelname is ok\n");	
	return 1;
}

/*Function receives name of label and check of there are no same labels in the linked list. Returns 0 of name already exist, 1 otherwise*/
int check_repeated_labels(char* name, node_label* head){
	node_label* ptr = head;

	while(ptr!=NULL){
		if(!strcmp(ptr->name, name)){/*list of labels already has this label*/

			return 0;
		}
		ptr = ptr->next;
	}
	return 1;
	
}


/*Receives pointers to the word and array of command names. Checks if command is legal. If legal returns it's index, ERROR otherwise*/
int check_cmd(char *word, struct CmdNames *cmd){
	int cmd_index;	
	for(cmd_index=0; cmd[cmd_index].name!=NULL;cmd_index++){
		if(strcmp(word,cmd[cmd_index].name)==0){
			return cmd_index;
		}
	}
	return ERROR;	
}

/*Receives pointers to the word and array of command names. Checks if command is legal. If legal returns it's index, ERROR otherwise*/
int check_directive(char *word, struct DrctvNames *drctv){
	int drctv_index;	
	for(drctv_index=0; drctv[drctv_index].name!=NULL;drctv_index++){
		if(strcmp(word,drctv[drctv_index].name)==0){
			return drctv_index;
		}
	}
	return ERROR;	
}

/*Function receives command, line num, command type and struct of opcodes, and checks if arguments of given command are right*/
int check_cmd_args(char *command, int line_num, int type, struct CmdNames *cmd){
	char *label, *opcode, *arg, *source, *destination;
	char *white_space = " \t\v\f\r\n";
	int arg_count = 0, cmd_index;
	
	if(type == LABEL_CMD){
		label = strtok(command, white_space);
		opcode = strtok(NULL, white_space);
		label = strtok(label, ":");
	}
	else 
		opcode = strtok(command, white_space);
	
	cmd_index = check_cmd(opcode, cmd); 

	while((arg = strtok(NULL, ","))!=NULL){
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

	}
	else {
		switch(cmd_index){/*switch by func index of struct*/
			case 0: 
			case 1:		
			case 2:
			case 3:	
				if(((*(cmd[cmd_index].func))(source, destination))==ERROR)
							return ERROR;
			case 4:
						
			case 5: 
				if(((*(cmd[cmd_index].func))(destination))==ERROR)
							return ERROR;
						
			case 6:
				if(((*(cmd[cmd_index].func))(source, destination))==ERROR)
							return ERROR;		
			case 7: 
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
				if(((*(cmd[cmd_index].func))(source, destination))==ERROR)
							return ERROR;

			case 14: 
			case 15:

			break;
		}/*end of switch*/
	}/*end of else*/

	/*add label to linked list of labels*/
	/*if all is ok go to funct add node to linked list of opcodes*/
	return ERROR;
}

/*Function receives command, line num, command type and struct of directives, and checks if arguments of given directive are right*/
int check_drctv_args(char *command, int line_num, int type, struct DrctvNames *drctvs){
	char *label, *drctv, *arg;
	char *white_space = " \t\v\f\r\n";
	int arg_count = 0, drctv_index;
	
	if(type == LABEL_DRCTV){
		label = strtok(command, white_space);
		drctv = strtok(NULL, white_space);
		label = strtok(label, ":");
	}
	else 
		drctv = strtok(command, white_space);
	drctv_index = check_directive(drctv, drctvs); 
	

	while((arg = strtok(NULL, white_space))!=NULL){
		
			
				
	}


	return ERROR;
}



/*Function checks if argument is an integer, if it is returns num of addressing type, otherwise returns -1*/
int check_arg_number(char *num){
	if(num[0]!='#')
		return ERROR;
	else 
	return check_one_num(num);
}

/*Function checks if argument is a label, if it is returns num of addressing type, otherwise returns -1*/
int check_arg_label(char *word){
	/*if ok return 1*/

	
	return ERROR;
}




/*Function checks if argument is an integer, if it is returns num of addressing type, otherwise returns -1*/
int check_arg_struct(char *word){
	/*if ok return 2*/

	
	return ERROR;
}

/*Function checks if argument is register, if it is returns num of addressing type, otherwise returns -1*/
int check_arg_register(char *word, char *REGISTER[]){
	/*if ok return 3*/
	int i;
	for(i=0; i<CMDLENGTH; i++){
		if(!strcmp(word, REGISTER[i]))
			return i;
	}

	return ERROR;
}

