#include "main.h" /*main libraries*/
#include "first_step.h" /*functions*/
#include "cmd_check.h" /*check functions*/
#include "label_lists.h" /*lists of labels and structs*/




void check_cmd_line(char *sourceFileName){
	FILE *sfp;
	int line_num = 0;
	int cmd_type;
	char *command, *commandCopy, *commandFinal;
	char *firstWord, *secondWord, *word, *label;
	int isLabel = 0; /*label flag*/
	char *white_space = " \t\v\f\r\n";
	int cmd_index, drctv_index;
	int isError = 0;	

	labels *head_lbl = NULL,  *tail_lbl = NULL; /*list of labels*/
	entryLabels *head_en_lbl = NULL,  *tail_en_lbl = NULL; /*list of internal labels*/
	externLabels *head_ex_lbl = NULL,  *tail_ex_lbl = NULL; /*list of external labels*/
	structs *head_struct = NULL,  *tail_struct = NULL; /*list of structs*/


	if((sfp = fopen(sourceFileName, "r")) == NULL){/*cannot open source file, exit*/
		printf("Cannot open %s\n", sourceFileName);
		return;
	}
	
	FOREVER {
		command = (char*)malloc(sizeof(char)*LINESIZE+1);/*to check typo errors*/
		if(command==NULL)
			return;

		commandCopy = (char *)malloc(sizeof(char)*LINESIZE+1);/*to check all parameters are legal*/
		if(commandCopy==NULL)
			return;
	
		commandFinal = (char *)malloc(sizeof(char)*LINESIZE+1);/*to save all arguments of legal line*/
		if(commandFinal==NULL)
			return;

		if(fgets(command, LINESIZE, sfp)==NULL)/*reads a line of LINESIZE length, checks if empty*/
			break;	
		
		line_num++;/*count line*/
		printf("Command is: %s\n", command);
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

		if(ispunct(command[strlen(command)-1]) && (command[strlen(command)-1]!='"')){/*not a "" punctuation mark at the end of command*/ 
			printf("Extraneous punctuation mark at the end of line, in line number: %d\n", line_num);
			continue;
		}

		if((checkCommas(command, line_num))==ERROR)/*check consecutive commas*/
			continue;
		
		strcpy(commandCopy, command);/*makes a copy of original command*/
		strcpy(commandFinal, command);/*makes a copy of original command*/
	

		firstWord = strtok(command, white_space);/*take first word*/	
		
		if((secondWord = strtok(NULL, white_space)) == NULL){/*take second word*/
			printf("Error, missing arguments in line number: %d\n", line_num);
			continue;
		}

		if(firstWord[strlen(firstWord)-1] == ':'){/*if : at the end of word and it is label, check all parameters*/
		label = strtok(firstWord, ":");/*take label name*/
			
		if((isLabel = check_label_islegal(label, line_num)) == ERROR)/*check if label name is legal, turn on label flag*/
			continue;
		}			
		if(isLabel)/*choose next word to check*/
			word = secondWord;
		else word = firstWord;

		if(word[0] == '.'){/*if is directive*/
			if((drctv_index = check_directive_islegal(word, line_num))==ERROR)/*if directive is not legal go to next line*/
				continue;
			
			switch(drctv_index){/*switch by func index of struct*/
			case 0: 
				if(check_nums(commandCopy, isLabel)==ERROR){/*check data parameters*/
					isError = 1;
					break;
				}
				printf("Data directive is ok \n");

				if(isLabel) /*add label if exists*/
					add_node_label(&head_lbl, &tail_lbl, firstWord, line_num);	
				break;	
			case 1:	
				if(check_string_islegal(commandCopy, isLabel)==ERROR){
					printf("String for .string directive is not legal, in line: %d\n", line_num);
					isError = 1;
					break;
				}
				printf("String directive is ok \n");
				if(isLabel) 
					add_node_label(&head_lbl, &tail_lbl, firstWord, line_num);
				break;
			case 2: if(!isLabel){
					printf("Error, struct definition without initialization, in line num: %d\n", line_num);
					isError = 1;
					break;
				}
				add_node_struct(&head_struct, &tail_struct, label, line_num);
				printf("This is struct directive, added to list \n");
				break;
			case 3: 
				if(isLabel)
					printf("Warning, label before position directive will be ignored, in line %d\n", line_num);
					
				printf("Check label name");		
				add_node_entry_label(&head_en_lbl, &tail_en_lbl, label, line_num);	
				break;
			case 4: 
				if(isLabel)
					printf("Warning, label before position directive will be ignored, in line %d\n", line_num);
					
				printf("Check label name");
				if(check_label_islegal(label, line_num)==ERROR){
					isError = 1;
					break;
				}
				add_node_extern_label(&head_ex_lbl, &tail_ex_lbl, label, line_num);	
				break;
			break;		
			}/*end of switch*/

		}

		else {
			if(isError)			
				continue;
			if(isLabel)/*if label check if second word is legal command*/
				cmd_index = check_cmd(secondWord, cmd);
			else cmd_index = check_cmd(firstWord, cmd);/*else check first word*/

			if(cmd_index==ERROR){/*command isn't legal*/
				printf("Undefined command name in line number: %d\n", line_num);
				continue;
			}
			if(isLabel){
				add_node_label(&head_lbl, &tail_lbl, firstWord, line_num);	
			}
		}
		if(isLabel){
			print_label_list(head_lbl); /*print list?????*/
		}

	}/*end of forever*/	
	

	print_struct_list(head_struct);
free(command);
free(commandCopy);
free(commandFinal);
fclose(sfp);
	
}/*end of checkcmdline func*/




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




/*Function receives source and destination adressing type and checks if they are legal for first group of opcodes*/
int check_first_group(char *source, char *dest){
	if(check_one_num(dest)!=ERROR)/*if destination is number - ERROR*/
		return ERROR;

	if(check_one_num(source)!=ERROR){/*if source is number - OK*/
		printf("Source is a number - OK\n");
	}
	else if(check_arg_register(source)!=ERROR)/*if source is register - OK*/
		return 3;
	else if(check_arg_struct(source)!=ERROR)/*if source is struct - OK*/
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





