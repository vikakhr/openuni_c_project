#include "main.h" /*main libraries*/
#include "main_data.h" /*data*/
#include "first_step.h" /*functions*/
#include "cmd_check.h" /*check functions*/




void check_cmd_line(char *sourceFileName){
	FILE *sfp;
	int line_num = 0;
	int cmd_type;
	char *command, *commandCopy;


	
	if((sfp = fopen(sourceFileName, "r")) == NULL){/*cannot open source file, exit*/
		printf("Cannot open %s\n", sourceFileName);
		return;
	}
	
	FOREVER {
		command = (char*)malloc(sizeof(char)*LINESIZE);
		if(command==NULL)
			return;

		commandCopy = (char *)malloc(sizeof(char)*LINESIZE);
		if(commandCopy==NULL)
			return;
	
		if(fgets(command, LINESIZE, sfp)==NULL)/*reads a line of LINESIZE length, checks if empty*/
			break;	
		
		line_num++;/*count line*/
		printf("Command is: %s\n\n", command);
		if(command[strlen(command)-1]!='\n'){/*if given length is not enough for line*/
			printf("Error, too long command line, in line number: %d\n", line_num);
			FOREVER {/*ignores the too long line*/
				if(fgets(command, LINESIZE, sfp)==NULL)/*reads a line of LINESIZE length, checks if empty*/
					break;
				if(command[strlen(command)-1]=='\n')/*if given length is enough for line*/
					break;			
			}/*end of nested forever*/
			continue;
		}

		command = remove_blanks(command);/*remove whitespaces by sides of line*/

		if(strlen(command)==1 && command[0]==' ')/*check if line is all whitespaces*/
			continue;

		if(command[0] == ';')/*if this is comment line - ignore and go to next*/
			continue;

		if(ispunct(command[strlen(command)-1])){/*if punctuation mark at the end of command*/ 
			printf("Extraneous punctuation mark at the end of line, in line number: %d\n", line_num);
			continue;
		}

		if((checkCommas(command, line_num))==ERROR)/*check consecutive commas*/
			continue;
		
		strcpy(commandCopy, command);/*makes a copy of original command*/
	
		if((cmd_type = check_command_type(commandCopy, line_num))==-1)/*check command type*/
			continue;

		else if(cmd_type==LABEL_CMD || cmd_type==CMD){ /*check parameters*/
			if(check_cmd_args(command, line_num, cmd_type, cmd)==ERROR)
				continue;	
		}
		else if(check_drctv_args(command, line_num, cmd_type, drctv)==ERROR)
				continue;	
	

	}/*end of forever*/	
		

fclose(sfp);
	
}/*end of checkcmdline func*/










int check_first_group(char *source, char *dest){
	if(!check_one_num(source))/*if source is number - OK*/
		return 1;
/*go and check addressing type, check if is right*/
	if(!check_one_num(dest))/*if destination is number - ERROR*/
		return ERROR;
	return ERROR;

}

int check_second_group(char *dest){
	if(!check_one_num(dest))/*if destination is number - error*/
		return ERROR;
	return ERROR;
}



int check_cmp(char *source, char *dest){
	if(!check_one_num(source))/*if source is number - OK*/
		return 1;
	if(!check_one_num(dest))/*if destination is number - OK*/
		return 1;
	return ERROR;
}

int check_lea(char *source, char *dest){
	if(!check_one_num(source))/*if source is number - error*/
		return ERROR;
	if(!check_one_num(dest))/*if destination is number - error*/
		return ERROR;
	return ERROR;
}

int check_prn(char *dest){
	if(!check_one_num(dest))/*if destination is number - OK*/
		return 1;
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




void check_data_param(){}
void check_string(){}
void check_struct_param(){}
void check_entry(){}
void check_extern(){}


