#include "main.h" /*main libraries*/
#include "parse_file.h" /*functions*/
#include "cmd_check.h" /*check functions*/
#include "label_lists.h" /*lists of labels and structs*/
#include "helper_func.h"


/*Function receives file name and linked lists for saving data, reads file line by line and makes first error checks, if checks are ok passes line to function for further treatment*/
void read_cmd_line(char *sourceFileName, labels **head_lbl, labels **tail_lbl, directiveLine **head_drctv, directiveLine **tail_drctv, cmdLine **head_cmd, cmdLine **tail_cmd, externs **head_extern, externs **tail_extern){
	FILE *sfp;
	int line_num = 0;/*line number*/	
	char *command;

	if((sfp = fopen(sourceFileName, "r")) == NULL){/*cannot open source file, exit*/
		printf("Cannot open %s\n", sourceFileName);
		return;
	}
	
	command = (char*)malloc(sizeof(char)*LINESIZE+1);/*allocate memory for a line*/
	if(command==NULL)
		return;

	FOREVER {
		if(fgets(command, LINESIZE, sfp)==NULL)/*reads a line of LINESIZE length, checks if empty*/
			break;	
		
		line_num++;/*count line*/
		
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
		
		if((check_typo_errors(command, line_num))==ERROR)/*check typo errors*/
			continue;

		check_cmd_line(command, line_num, &(*head_lbl), &(*tail_lbl), &(*head_drctv), &(*tail_drctv), &(*head_cmd), &(*tail_cmd),
		&(*head_extern), &(*tail_extern));/*pass all parameters to next step checks*/
	}/*end of forever*/


	free(command);
	fclose(sfp);
}


/*Function receives line, it's number and linked lists for saving data, checks errors and saves legal line's data inside linked lists*/
void check_cmd_line(char *command, int line_num, labels **head_lbl, labels **tail_lbl, directiveLine **head_drctv, directiveLine **tail_drctv, cmdLine **head_cmd, cmdLine **tail_cmd, externs **head_extern, externs **tail_extern){
	char *firstWord, *secondWord, *thirdWord, *word, *label = NULL;/*pointers to separate words*/
	int arg_counter = 0;/*counter number of operands of the command*/
	char *source = NULL, *destination = NULL;/*operands of instruction*/
	int cmd_index, drctv_index;/*indexes of command names*/	
	int len; /*string length*/
	int isLabel = 0; /*label flag*/
	char *white_space = " \t\v\f\r\n";
	char *ptr, *command_copy;

	if((command_copy = remove_blanks(command))==NULL)/*copy of cmd line without whitespaces by sides*/
		return; /*if malloc of remove blanks fails*/

	if((firstWord = strtok(command, white_space))==NULL)/*take first word*/{
		free(command_copy);/*free memory allocated by remove_blanks func*/
		return;	
	}

	if((secondWord = strtok(NULL, white_space)) == NULL){/*take second word*/
		printf("Error, missing operands in line number: %d\n", line_num);
		free(command_copy);/*free memory allocated by remove_blanks func*/
		return;
	}

	thirdWord = strtok(NULL, white_space); /*take third word for check 3 words containing label positioning*/

	if(ispunct(firstWord[strlen(firstWord)-1])){
		if(firstWord[strlen(firstWord)-1] == ':'){/*if : at the end of word and it is label, check all parameters*/
			len = strlen(firstWord)-1;/*length of label name without colon*/
			label = strtok(firstWord, ":");/*take label name*/
			if(len!=strlen(label)){/*if length of first word is not the same as length of label name*/
				printf("Error, illegal colon inside label definition, in line number: %d\n", line_num);
				free(command_copy);
				return;
			}
			if((isLabel = check_label_islegal(label, line_num)) == ERROR){/*check if label name is legal, turn on label flag*/
				free(command_copy);
				return;
			}
			if(isLabel){/*if there is label in line, check if it legal*/
				if(check_repeated_labels(label, *head_lbl)==ERROR){
					printf("Error, repeated label definition, in line number: %d\n", line_num);
					free(command_copy);
					return;
				}
			}
		}
		else {
			printf("Error, illegal punctuation mark after first word of command, in line number: %d\n", line_num);
			free(command_copy);
			return;
		}
	}

	if(secondWord[0] == ','){/*if comma after first word*/
		printf("Error, illegal comma after first word of command, in line number: %d\n", line_num);
		free(command_copy);
		return;
	}

	if(strchr(secondWord, ':')){/*if colon in second word of command - error*/
		printf("Error, second word of command is not legal, in line number: %d\n", line_num);
		free(command_copy);
		return;
		}

	if(isLabel)/*choose next word to check*/
		word = secondWord;
	else word = firstWord;

	if(word[0] == '.'){/*if is directive*/
		if((drctv_index = check_directive_islegal(word, line_num))==ERROR){/*if directive is not legal go to next line*/
			free(command_copy);
			return;
		}

		switch(drctv_index){/*switch by directive index*/
			case 0:/*.data*/ 
				if((arg_counter = check_nums(command_copy, isLabel, line_num))==ERROR){/*check data parameters*/
					break;
				}

				if(isLabel) /*add label if exists*/{
					if((check_label_positioning(&(*head_lbl), &(*head_extern), label, LABEL, line_num))==ERROR)
						break;
					add_node_label(&(*head_lbl), &(*tail_lbl), label, line_num, LABEL);
				}
				add_data_arg(command_copy, isLabel, line_num, &(*head_drctv), &(*tail_drctv));
				break;	
			case 1:/*.string*/
				if(check_string_islegal(command_copy, isLabel)==ERROR){
					printf("Error, string for .string directive is not legal, in line: %d\n", line_num);
					break;
				}

				if(isLabel){
					if((check_label_positioning(&(*head_lbl), &(*head_extern), label, LABEL, line_num))==ERROR)
						break;
					add_node_label(&(*head_lbl), &(*tail_lbl), label, line_num, LABEL);
				}
				add_string_arg(command_copy, isLabel, line_num, &(*head_drctv), &(*tail_drctv));
				break;
			case 2: /*.struct*/
				if(!isLabel){
					printf("Error, struct definition without initialization, in line number: %d\n", line_num);
					break;
				}
				if((check_label_positioning(&(*head_lbl), &(*head_extern), label, STRUCT, line_num))==ERROR)
					break;
				if(check_operand_struct(command_copy, line_num, isLabel)==ERROR)/*check struct operands*/
					break;
				add_node_label(&(*head_lbl), &(*tail_lbl), label, line_num, STRUCT);
				add_struct_arg(command_copy, isLabel, line_num, &(*head_drctv), &(*tail_drctv));
				break;
			case 3: /*.entry*/
				if(isLabel){/*if label before .entry word*/
					printf("Warning, label before position directive will be ignored, in line number: %d\n", line_num);
					secondWord = thirdWord;
					if(secondWord == NULL){
						printf("Error, missing label name for label positioning, in line number: %d\n", line_num);
						break;
					}
				}
				
				if(check_label_islegal(secondWord, line_num)==ERROR){
					break;
				}

				if((check_label_positioning(&(*head_lbl), &(*head_extern), secondWord, ENTRY, line_num))==ERROR)
					break;
				add_node_label(&(*head_lbl), &(*tail_lbl), secondWord, line_num, ENTRY);
				break;
			case 4: /*.extern*/
				if(isLabel){/*if label before .extern word*/
					printf("Warning, label before position directive will be ignored, in line number: %d\n", line_num);
					secondWord = thirdWord;
					if(secondWord == NULL){
						printf("Error, missing label name for label positioning, in line number: %d\n", line_num);
						break;
					}					
				}
				
				if(check_label_islegal(secondWord, line_num)==ERROR)
					break;
				
				if((check_label_positioning(&(*head_lbl), &(*head_extern), secondWord, EXTERN, line_num))==ERROR)
					break;
				add_node_extern(&(*head_extern), &(*tail_extern), secondWord);
				break;
			break;		
			}/*end of switch*/			
		}/*end of if*/

		else {/*if not directive check if instruction*/
			if(isLabel)/*if label check if second word is legal command*/
				cmd_index = check_cmd_name(secondWord);
			else cmd_index = check_cmd_name(firstWord);/*else check first word*/

			if(cmd_index==ERROR){/*command isn't legal*/
				printf("Error, undefined command name in line number: %d\n", line_num);
				free(command_copy);
				return;
			}

			if((check_cmd_operands(command_copy, line_num, isLabel, cmd_index)) == ERROR){
				free(command_copy);
				return;
			}
			if(isLabel)
				add_node_label(&(*head_lbl), &(*tail_lbl), label, line_num, LABEL);

			if(isLabel){
				word = strtok(command_copy, white_space);
				word = strtok(NULL, white_space);
			}
			else 
				word = strtok(command_copy, white_space);

					
			if((ptr = strtok(NULL, ","))!=NULL){
					arg_counter++; /*argument counter*/
					source = remove_blanks(ptr);
					if(source == NULL)/*if malloc inside remove blanks fails*/
						return;
				if((ptr = strtok(NULL, white_space))!=NULL){
					arg_counter++; /*argument counter*/
					destination = remove_blanks(ptr);
					if(destination == NULL)/*if malloc inside remove blanks fails*/
						return;
				}
				else {
					destination = source;
					source = NULL;
				}
			}
			else/*if no operands for instruction*/
				destination = NULL;

			add_instruction_node(&(*head_cmd), &(*tail_cmd), source, destination, cmd_index, line_num, arg_counter, isLabel);

			if(arg_counter==1)/*free pointer returned from remove blanks*/
				free(destination);
			if(arg_counter==2){/*free pointers returned from remove blanks*/
				free(source);
				free(destination);
			}
		}
	free(command_copy);

}/*end of checkcmdline func*/





/*Function receives directive .data line, it's details, head and tail of linked list of directives and adds arguments of data directive to linked list*/
void add_data_arg(char* line, int isLabel, int line_num, directiveLine **head_drctv, directiveLine **tail_drctv){
	char *ptr;
	char *white_space = " \t\v\f\r\n";
	short int arg;
	char* number = (char*)malloc(strlen(line)+1);
	if(number==NULL)
		return;
	strcpy(number, line);

	if(isLabel){
		ptr = strtok(number, white_space);
		ptr = strtok(NULL, white_space);
	}
	else ptr = strtok(number, white_space);

	while((ptr = strtok(NULL, ","))!=NULL){
		arg = (short)atoi(ptr);
		if(arg>MAX_10_BITS_NUM){/*if 10 10 bits is not enough for signed number*/
			printf("Warning, parameter of .data exceeds boundary, some data may be lost, in line: %d\n", line_num);
			arg = MAX_10_BITS_NUM;
		}
		if(arg<MIN_10_BITS_NUM){
			printf("Warning, parameter of .data exceeds boundary, some data may be lost, in line: %d\n", line_num);
			arg = MIN_10_BITS_NUM;
		}
		add_directive_node(&(*head_drctv), &(*tail_drctv), line_num, isLabel, arg);/*adds directive arg into linked list*/	

	}

	free(number);	
}

/*Function receives directive .data line, it's details, head and tail of linked list of directives and adds arguments of data directive to linked list*/
void add_string_arg(char* line, int isLabel, int line_num, directiveLine **head_drctv, directiveLine **tail_drctv){
	int i;
	int inString = 0;
	char *lineCopy = (char*)malloc(strlen(line)+1);
	if(lineCopy==NULL)
		return;
	strcpy(lineCopy, line);
	
	for(i=0; i<strlen(line); i++){
		if(lineCopy[i]=='"'){
			if(inString){
				if(i==(strlen(lineCopy)-1))
					break;
				else
					add_directive_node(&(*head_drctv), &(*tail_drctv), line_num, isLabel, (short)lineCopy[i]);/*adds directive arg into linked list*/
			}
			else inString = 1;
		}
		else {
			if(inString)
				add_directive_node(&(*head_drctv), &(*tail_drctv), line_num, isLabel, (short)lineCopy[i]);/*adds directive arg into linked list*/	
		}	
	}
	add_directive_node(&(*head_drctv), &(*tail_drctv), line_num, isLabel, (short)0);/*adds directive arg with \0 char into linked list*/	
	free(lineCopy);
}


/*Function receives directive .struct line, it's details, head and tail of linked list of directives and adds arguments of struct directive to linked list*/
void add_struct_arg(char* line, int isLabel, int line_num, directiveLine **head_drctv, directiveLine **tail_drctv){
	int i;
	int in_string = 0;/*inside string flag*/
	char *white_space = " \t\v\f\r\n";
	char *ptr;
	short int arg;/*argument of struct*/
	char *line_copy = (char*)malloc(strlen(line)+1);/*allocate memory for copy of command line*/
	if(line_copy==NULL)
		return;
	
	strcpy(line_copy, line);

	ptr = strtok(line_copy, white_space);
	ptr = strtok(NULL, white_space);
	ptr = strtok(NULL, ",");/*take first argument of struct*/
	
	arg = (short)atoi(ptr);
	if(arg>MAX_10_BITS_NUM){/*if 10 bits is not enough for signed number*/
		printf("Warning, parameter of .struct exceeds boundary, some data may be lost, in line: %d\n", line_num);
		arg = MAX_10_BITS_NUM;
	}
	if(arg<MIN_10_BITS_NUM){/*if 10 bits is not enough for negative signed number*/
		printf("Warning, parameter of .struct exceeds boundary, some data may be lost, in line: %d\n", line_num);
		arg = MIN_10_BITS_NUM;
	}
	add_directive_node(&(*head_drctv), &(*tail_drctv), line_num, isLabel, arg);/*adds directive num arg into linked list*/	
	
	for(i=0; i<strlen(line); i++){/*iterate char by char*/
		if(line_copy[i]=='"'){
			if(in_string){
				if(line_copy[i+1]=='\n')/*if inside string and this is end if line*/
					break;	
			}
			else in_string = 1;/*if there is " char inside string*/
		}
		else {
			if(in_string){/*add char of string to linked list*/
				add_directive_node(&(*head_drctv), &(*tail_drctv), line_num, isLabel, (short)line_copy[i]);/*adds string arg into linked list*/
			}		
		}	
	}
	add_directive_node(&(*head_drctv), &(*tail_drctv), line_num, isLabel, (short)0);/*adds arg with \0 char into linked list*/	
	free(line_copy);
}

/*Function receives head and tail of linked list of copcode ommands, source and destination arguments and details about command line.
 * Creates and adds new node of opcode command and adds to linked list*/
void add_instruction_node(cmdLine **head, cmdLine **tail, char* source, char* destination, int cmd_index, int line_num, int args, int isLabel){
	cmdLine *new = (cmdLine*)malloc(sizeof(cmdLine));
	if(new==NULL)
		return;
	
	if(!args){/*if no arguments*/
		new->source=NULL;
		new->destination=NULL;
	}
	if(args==2){/*if two arguments*/
		new->source = (char*)malloc(strlen(source)+1);
		if(new->source==NULL)
			return;
		strcpy(new->source, source);
		new->destination = (char*)malloc(strlen(destination)+1);
		if(new->destination==NULL){
			free(new->source);
			return;
		}
		strcpy(new->destination, destination);
	}
	else if(args==1){/*if one argument*/
		new->destination = (char*)malloc(strlen(destination)+1);
		if(new->destination==NULL)
			return;
		strcpy(new->destination, destination);
		new->source=NULL;
	}

	new->args = args;
	new->line_num = line_num;
	new->cmd_index = cmd_index;
	new->is_label = isLabel;
	new->next = NULL;

	if(*head == NULL){/*if this is first node*/
		*head = new;
	}
	else if((*head)->next == NULL){/*if this is second node*/
		(*head)->next = new;
		*tail = (*head)->next;
	}
	else {
		(*tail)->next = new;
		*tail = new;
	}
}

/*Function receives head and tail of directives linked list, directive argument and it's details.
 * Creates and adds new node and adds into linked list of directives*/
void add_directive_node(directiveLine **head, directiveLine **tail, int line_num, int isLabel, int arg){
	directiveLine *new = malloc(sizeof(directiveLine));
	if(new==NULL)
		return;

	new->arg = arg;
	new->isLabel = isLabel;
	new->line_num = line_num;
	new->memory_count = 0;
	new->next = NULL;

	if(*head==NULL){/*if this is first node*/
		*head = new;
		return;
	}
	else if((*head)->next == NULL){/*if this is second node*/
		(*head)->next = new;
		*tail = (*head)->next;
	}
	else {
		(*tail)->next = new;
		*tail = new;
	}
}

/*Function receives head of linked list of instruction and line number of node need to be deleted, searches this node and frees a memory of node and it's members
 * Frees node and contains and deletes it from linked list*/
void delete_instruction_node(cmdLine **head, int line_num){
	cmdLine *ptr = *head;
	cmdLine *temp;

	/*search by line number what need to be deleted*/
	if((*head)->line_num  == line_num){/*if this is head*/
		temp = *head;
		*head = (*head)->next;
	}
	else {
		while(ptr->next != NULL){
			if((ptr->next)->line_num == line_num){
				temp = ptr->next;
				ptr->next = (ptr->next)->next;
				break;
			}
			else ptr = ptr->next;
		}
	}

	if(temp->args == 1)/*free contains if one arg*/
		free(temp->destination);
	if(temp->args == 2){/*free contains if two args*/
		free(temp->source);
		free(temp->destination);
	}
	free(temp);/*free node*/
}






