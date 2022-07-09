#include "main.h" /*main libraries*/
#include "main_data.h" /*data*/
#include "cmd_check.h" /*check functions*/
#include "first_step.h" /*functions*/





void check_cmd_line(char *sourceFileName){
	FILE *sfp;
	int line_num = 0;
	int i;
	int cmd_type;
	node_label *head = NULL, *tail = NULL;/*linked list nodes for lables*/	
	char *command, *commandCopy;
	char *label, *directive;
	char *white_space = " \t\v\f\r\n";
	char *source, *destination;

	
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
			break;
	
		
		if(fgets(command, LINESIZE, sfp)==NULL)/*reads a line of LINESIZE length, checks if empty*/
			break;	
		
		line_num++;/*count line*/

		if(commandCopy[strlen(commandCopy)-1]!='\n'){/*if given length is not enough for line*/
			printf("Error, too long command line, in line number: %d\n", line_num);

			FOREVER {/*ignores the too long line*/
				if(fgets(command, LINESIZE, sfp)==NULL)/*reads a line of LINESIZE length, checks if empty*/
					break;	
				
				if(command[strlen(command)-1]=='\n')/*if given length is enough for line*/
					break;			
			}/*end od nested forever*/
		}

		command = remove_blanks(command);/*remove whitespaces by sides of line*/

		if(strlen(command)==1 && command[0]==' ')/*check if line is all whitespaces*/
			continue;

		if(command[0] == ';')/*if this is comment line - ignore and go to next*/
			return -1;

		if(ispunct(command[strlen(command)-1])){/*if punctuation mark at the end of command*/ 
			printf("Extraneous punctuation mark at the end of line, in line number: %d\n", line_num);
			continue;
		}

		if(checkCommas(command, line_num)==ERROR)/*check consecutive commas*/
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




int check_command_type(char *line, int line_num){
	char *firstWord, *secondWord, *word, *label;
	char *white_space = " \t\v\f\r\n";
	int cmd_type, drctv_type, cmd_index, drctv_index;
	int isLabel = 0;

	firstWord = strtok(line, white_space);/*take first word*/	
		
	if(secondWord = strtok(NULL, white_space) == NULL){/*take second word*/
		printf("Error, missing arguments in line number: %d", line_num);
		return -1;
	}

	if(firstWord[strlen(firstWord)-1] == ':'){/*if : at the end of word and it is label, check all parameters*/
		label = strtok(firstWord, ":");/*take label name*/
			
		if((isLabel = check_label_islegal(label, secondWord, line_num)) == 0)/*check if label name is legal*/
			return -1;

		/*if(!check_repeated_labels(label, head)){/*label name is already exists, error, CHECK THIS AT SECOND ITERATION
			printf("Error, repeated label name: %s\n in line number: %d\n", label, line_num);
			return -1;
		}*/
	}			
	if(isLabel)/*choose next word to check*/
		word = secondWord;
	word = firstWord;
			
	if(word[0] == '.'){/*if is directive*/
		if((drctv_index = check_directive(word, drctv))==ERROR){/*check if first word is command*/
			if(word[strlen(firstWord)-1] == ','){/*Illegal comma after command name check*/
				printf("Illegal comma in line number: %d\n", line_num);
				return -1;	
			}
			else {
				printf("Undefined command name in line number: %d\n", line_num);
				return -1;
			}
		}
		else if(isLabel)
			return LABEL_DRCTV;
		return DRCTV;						
	}

	else if((cmd_index = check_cmd(word, cmd))==ERROR){/*check if first word is command*/
		if(word[strlen(firstWord)-1] == ','){/*Illegal comma after command name check*/
			printf("Illegal comma in line number: %d\n", line_num);
			return -1;	
		}
		else {
			printf("Undefined command name in line number: %d\n", line_num);
			return -1;
		}
	}
	else if(isLabel)
		return LABEL_CMD;
	return CMD;
	
}





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









