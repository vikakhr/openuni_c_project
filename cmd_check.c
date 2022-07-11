#include "main.h"
#include "first_step.h"
#include "cmd_check.h"
#include "label_lists.h"

/*Receives command line and line num, checks if opcode or directive is legal and returns which command type line is*/
int check_command_type(char *line, int line_num){
	char *firstWord, *secondWord, *word, *label;
	char *white_space = " \t\v\f\r\n";
	int cmd_type, drctv_type, cmd_index, drctv_index;
	int isLabel = 0;

	firstWord = strtok(line, white_space);/*take first word*/	
		
	if((secondWord = strtok(NULL, white_space)) == NULL){/*take second word*/
		printf("Error, missing arguments in line number: %d\n", line_num);
		return -1;
	}

	if(firstWord[strlen(firstWord)-1] == ':'){/*if : at the end of word and it is label, check all parameters*/
		label = strtok(firstWord, ":");/*take label name*/
			
		if((isLabel = check_label_islegal(label, line_num)) == 0)/*check if label name is legal*/
			return ERROR;

		/*if(!check_repeated_labels(label, head)){/*label name is already exists, error, CHECK THIS AT SECOND ITERATION
			printf("Error, repeated label name: %s\n in line number: %d\n", label, line_num);
			return -1;
		}*/
	}			
	if(isLabel)/*choose next word to check*/
		word = secondWord;
	word = firstWord;
	printf("First: %s\t Second: %s\t Word: %s\n", firstWord, secondWord, word);		
	if(word[0] == '.'){/*if is directive*/
		printf("Directive check?\n");
		if((drctv_index = check_directive(word))==ERROR){/*check if first word is directive*/
			printf("Here\n");
			if(word[strlen(firstWord)-1] == ','){/*Illegal comma after command name check*/
				printf("Illegal comma in line number: %d\n", line_num);
				return ERROR;	
			}
			else {
				printf("Undefined command name in line number: %d\n", line_num);
				return ERROR;
			}
		}
		if(drctv_index== 3 || drctv_index== 4){/*if this is label position directive?????*/	
			if(!isLabel){
				printf("This is label position directive, check if ok and put it into list\n");
				printf("First:%s\t Second:%s\t Word: %s\t\n", firstWord, secondWord, word);
				if((word = strtok(NULL, white_space))!=NULL){
					printf("Error, extraneous text after label position directive, in line number: %d\n", line_num);
					return ERROR;
				}
				if(check_label_islegal(secondWord, line_num)==ERROR){
					printf("Error, illegal label name for label position directive, in line number: %d\n", line_num);
					return ERROR;
				}
				else 
				return LABEL_POSITION;
				}
			else {/*if there is label before position directive, ignore*/
				printf("Warning, a label before position directive will be ignored, in line %d", line_num);
				isLabel = 0;	
			}
		}
		
		else if(isLabel)
			return LABEL_DRCTV;
		return DRCTV;						
		
	}
	else {
		if(isLabel)/*if label check if second word is legal command*/
			cmd_index = check_cmd(secondWord, cmd);
		else cmd_index = check_cmd(firstWord, cmd);/*else check first word*/

		if(cmd_index==ERROR){/*command isn't legal*/
			printf("Undefined command name in line number: %d\n", line_num);
			return ERROR;
		}
	}
	
	if(isLabel)
		return LABEL_CMD;
	else return CMD;
	
}


int check_label_islegal(char* label, int line_num){
	int i, regNameLength = 2, cmdNameLength = 3;

	if(strlen(label)>LABELSIZE){/*too long name for label*/
		printf("Label name is too long, in line number: %d\n", line_num);
		return 0;
	}	
	if(!isalpha(label[0])){/*if first char is not a character*/
		printf("%s\n", label);
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

	if(strlen(label)==cmdNameLength){/*if length of label string is three check if is not opcode name*/
		for(i=0; i<CMDLENGTH; i++)
			if(!strcmp(label, OPCODE[i])){
				printf("Error, label name has the same name as opcode in line number: %d\n", line_num);
				return 0;
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

/*Function checks if argument can be a label, if it is returns num of addressing type, otherwise returns -1*/
int check_arg_label(char *word){
	int i;
	/*if ok return 1*/
	if(strlen(word)>LABELSIZE)/*too long name for label*/
		return ERROR;
		
	if(!isalpha(word[0]))/*if first char is not a character*/
		return ERROR;
	
	if(strlen(word)==3){/*if length of label string is three check if is not opcode name*/
		for(i=0; i<CMDLENGTH; i++)
			if(!strcmp(word, OPCODE[i]))
				return ERROR;
	}
	return 1;
}




/*Function checks if argument can be a struct, if it is returns num of addressing type, otherwise returns -1*/
int check_arg_struct(char *word){
	char *white_space = " \t\v\f\r\n";
	char *p;
	char *strct_arg = (char *)malloc(strlen(word)+1);
	if(strct_arg == NULL)
		return ERROR;
	strcpy(strct_arg, word);
	
	if(strchr(strct_arg, '.')==NULL){
		printf("No . in the comand - not struct\n");
		free(strct_arg);
		return ERROR;
	}

	p = strtok(strct_arg, ".");/*take pointer to struct name*/

	if(check_arg_label(p) == ERROR)/*if struct name is not legal*/
		return ERROR;

	if((p = strtok(NULL, white_space))==NULL){/*if no struct field num*/
		free(strct_arg);
		return ERROR;
	}
	if(check_one_num(p)==ERROR){/*if struct field num is not integer*/		
		free(strct_arg);
		return ERROR;
	}
	free(strct_arg);
	return 2;
}

/*Function checks if argument is register, if it is returns num of addressing type, otherwise returns -1*/
int check_arg_register(char *word){
	int i;
	for(i=0; i<CMDLENGTH; i++){
		if(!strcmp(word, REGISTER[i]))
			return 3;
	}

	return ERROR;
}

void check_label_position(char *command){
	char* position, *label;
	char *white_space = " \t\v\f\r\n";
	
	position = strtok(command, white_space);
	label = strtok(NULL, white_space);

	/*put inside linked list*/


}

