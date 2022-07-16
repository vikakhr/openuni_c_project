#include "main.h" /*main libraries*/
#include "first_step.h" /*functions*/
#include "second_step.h"
#include "cmd_check.h" /*check functions*/
#include "label_lists.h" /*lists of labels and structs*/




void check_cmd_line(char *sourceFileName){
	FILE *sfp;
	int line_num = 0;
	int cmd_type;
	char *command, *commandCopy, *commandFinal;
	char *firstWord, *secondWord, *thirdWord, *word, *label;
	int args_counter = 0;
	char *source, *destination;
	int isLabel = 0; /*label flag*/
	char *white_space = " \t\v\f\r\n";
	int cmd_index, drctv_index;	
	int isError;
	

	labels *head_lbl = NULL,  *tail_lbl = NULL; /*list of labels*/
	
	directiveLine *head_drctv = NULL, *tail_drctv = NULL; /*head and tail of directives list*/
	instructionLine *head_instruction = NULL, *tail_instruction = NULL; /*head and tail of instructions list*/
	


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

		if((line_typo_errors_check(command, line_num))==ERROR)
			continue;		


		strcpy(commandCopy, command);/*makes a copy of original command*/
		strcpy(commandFinal, command);/*makes a copy of original command*/
	
		isLabel = 0;
		isError = 0;
		args_counter = 0;
		
		if((firstWord = strtok(command, white_space))==NULL)/*take first word*/
			continue;	
		
		if((secondWord = strtok(NULL, white_space)) == NULL){/*take second word*/
			printf("Error, missing arguments in line number: %d\n", line_num);
			continue;
		}

		thirdWord = strtok(NULL, white_space); /*take third word for check 3 words containing label positioning*/

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
			printf("Directive\n");
			switch(drctv_index){/*switch by func index of struct*/
			case 0:/*.data*/ 
				if(check_nums(commandCopy, isLabel)==ERROR){/*check data parameters*/
					isError = 1;
					break;
				}
				printf("Data directive is ok \n");
				if(isLabel) /*add label if exists*/{
					if((check_label_positioning(&head_lbl, &tail_lbl, secondWord, ENTRY, line_num))==ERROR)
						break;
					add_node_label(&head_lbl, &tail_lbl, firstWord, line_num, ENTRY);
				}		
				add_directive_node(&head_drctv, &tail_drctv, line_num, commandCopy, isLabel, drctv_index);/*adds directive to linked list*/	
				break;	
			case 1:/*.string*/
				if(check_string_islegal(commandCopy, isLabel)==ERROR){
					printf("String for .string directive is not legal, in line: %d\n", line_num);
					isError = 1;
					break;
				}
				printf("String directive is ok \n");
				if(isLabel){
					if((check_label_positioning(&head_lbl, &tail_lbl, secondWord, ENTRY, line_num))==ERROR)
						break;
					add_node_label(&head_lbl, &tail_lbl, firstWord, line_num, ENTRY);
				}
				add_directive_node(&head_drctv, &tail_drctv, line_num, commandCopy, isLabel, drctv_index);/*adds directive to linked list*/
				break;
			case 2: /*.struct*/
				if(!isLabel){
					printf("Error, struct definition without initialization, in line num: %d\n", line_num);
					isError = 1;
					break;
				}
				if((check_label_positioning(&head_lbl, &tail_lbl, secondWord, ENTRY, line_num))==ERROR)
					break;

				add_node_label(&head_lbl, &tail_lbl, label, line_num, STRUCT);
				add_directive_node(&head_drctv, &tail_drctv, line_num, commandCopy, isLabel, drctv_index);/*adds directive to linked list*/
				printf("This is struct directive, added to list \n");
				break;
			case 3: /*.entry*/
				if(isLabel){/*if label before .entry word*/
					printf("Warning, label before position directive will be ignored, in line %d\n", line_num);
					printf("%s %s %s\n", firstWord, secondWord, thirdWord);
					secondWord = thirdWord;
					if(secondWord == NULL){
						printf("Error, missing label name for label positioning, in line_num: %d\n", line_num);
						isError = 1;
						break;
					}
				}
				
				if(check_label_islegal(secondWord, line_num)==ERROR){
					isError = 1;
					break;
				}
				printf("This is entry \n");
				if((check_label_positioning(&head_lbl, &tail_lbl, secondWord, ENTRY, line_num))==ERROR)
					break;
				add_node_label(&head_lbl, &tail_lbl, secondWord, line_num, ENTRY);
				break;
			case 4: /*.extern*/
				if(isLabel){/*if label before .extern word*/
					printf("Warning, label before position directive will be ignored, in line %d\n", line_num);
					printf("%s %s %s\n", firstWord, secondWord,  thirdWord);
					secondWord = thirdWord;
					if(secondWord == NULL){
						printf("Error, missing label name for label positioning, in line_num: %d\n", line_num);
						isError = 1;
						break;
					}
					
				}
				
				if(check_label_islegal(secondWord, line_num)==ERROR){
					isError = 1;
					break;
				}
				printf("This is extern \n");
				if((check_label_positioning(&head_lbl, &tail_lbl, secondWord, EXTERN, line_num))==ERROR)
					break;
				add_node_label(&head_lbl, &tail_lbl, secondWord, line_num, EXTERN);	
				break;
			break;		
			}/*end of switch*/





			
		}

		else {/*if not directive check if instruction*/
			printf("Instruction\n");
			
			if(isLabel)/*if label check if second word is legal command*/
				cmd_index = check_cmd(secondWord, cmd);
			else cmd_index = check_cmd(firstWord, cmd);/*else check first word*/

			if(cmd_index==ERROR){/*command isn't legal*/
				printf("Undefined command name in line number: %d\n", line_num);
				continue;
			}
			
			if((check_cmd_args(commandCopy, line_num, isLabel, cmd_index, cmd)) == ERROR)
				continue;

				
			if(isLabel)
			add_node_label(&head_lbl, &tail_lbl, label, line_num, ENTRY);

			if(isLabel){
				word = strtok(commandCopy, white_space);
				word = strtok(NULL, white_space);
			}
			else 
				word = strtok(commandCopy, white_space);
			printf("Before strtok: %s\n", word);
					
			if((source = strtok(NULL, ","))!=NULL){
					args_counter++; /*argument counter*/
					source = remove_blanks(source);
				if((destination = strtok(NULL, white_space))!=NULL){
					args_counter++; /*argument counter*/
					destination = remove_blanks(destination);
				}
				else {
					destination = source;
					source = NULL;
				}
			}
			else/*if no arguments for instruction*/
				destination = NULL;
				

			add_instruction_node(&head_instruction, &tail_instruction, source, destination, cmd_index, line_num, cmd[cmd_index].args);

			


		}

	

	}/*end of forever*/	
	
print_label_list(head_lbl);
/**/print_instruction_list(head_instruction);/************************************/
	printf("Before free\n");
free(command);
free(commandCopy);
free(commandFinal);


/*here need to free all lists*/

fclose(sfp);
	
}/*end of checkcmdline func*/

int line_typo_errors_check(char* command, int line_num){
	

	if(strlen(command)==1 && command[0]==' ')/*check if line is all whitespaces*/
		return ERROR;

	if(command[0] == ';')/*if this is comment line - ignore and go to next*/
		return ERROR;

	if(ispunct(command[strlen(command)-1]) && (command[strlen(command)-1]!='"')){/*not a "" punctuation mark at the end of command*/ 
			printf("Extraneous punctuation mark at the end of line, in line number: %d\n", line_num);
		return ERROR;
	}

	if((checkCommas(command, line_num))==ERROR)/*check consecutive commas*/
		return ERROR;
	return 0;
}


