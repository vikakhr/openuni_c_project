#include "main.h"
#include "main_data.h"
#include "first_step.h"
#include "cmd_check.h"

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

	if(strlen(label)==cmdNameLength){/*if length of label string is three check if is not opcode name*/
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

