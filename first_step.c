#include "main.h" /*main libraries*/
#include "first_step.h" /*functions*/
#include "cmd_check.h" /*check functions*/
#include "label_lists.h" /*lists of labels and structs*/
#include "helper_func.h"


/*Function receives file name and linked lists for saving data, reads file line by line and makes first error checks, if checks are ok passes line to function for further treatment*/
void read_cmd_line(char *sourceFileName, labels **head_lbl, labels **tail_lbl, directiveLine **head_drctv, directiveLine **tail_drctv, cmdLine **head_cmd, cmdLine **tail_cmd, externs **head_extern, externs **tail_extern){
	FILE *sfp;
	int line_num = 0;/*line number*/	
	char *command, *command_copy = NULL;


	if((sfp = fopen(sourceFileName, "r")) == NULL){/*cannot open source file, exit*/
		printf("Cannot open %s\n", sourceFileName);
		return;
	}
	
	FOREVER {
		command = (char*)malloc(sizeof(char)*LINESIZE+1);/*allocate memory for a line*/
		if(command==NULL)
			return;

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

		command_copy = remove_blanks(command);/*remove whitespaces by sides of line*/
		
		if((line_typo_errors_check(command_copy, line_num, strlen(command_copy)))==ERROR)/*check typo errors*/
			continue;		

		check_cmd_line(command_copy, line_num, &(*head_lbl), &(*tail_lbl), &(*head_drctv), &(*tail_drctv), &(*head_cmd), &(*tail_cmd),
		&(*head_extern), &(*tail_extern));/*pass all parameters to next step checks*/
	}/*end of forever*/

	print_label_list(*head_lbl);
	print_extlabel_list(*head_extern);
	print_instruction_list(*head_cmd);
	free(command);
	if(!command_copy)/*if there is at least one line in file, free pointer receoved from remove blanks*/
		free(command_copy);
	fclose(sfp);
}


/*Function receives line, it's number and linked lists for saving data, checks errors and saves legal line's data inside linked lists*/
void check_cmd_line(char *command, int line_num, labels **head_lbl, labels **tail_lbl, directiveLine **head_drctv, directiveLine **tail_drctv, cmdLine **head_cmd, cmdLine **tail_cmd, externs **head_extern, externs **tail_extern){
	
	char *firstWord, *secondWord, *thirdWord, *word, *label = NULL;/*pointers to separate words*/
	int operand_counter = 0;/*counter number of operands of the command*/
	char *source = NULL, *destination = NULL;/*of instruction line*/
	int cmd_index, drctv_index;/*indexes of command names*/	
	int isLabel = 0; /*label flag*/
	char *white_space = " \t\v\f\r\n";
	char *ptr;

	char *commandCopy = (char*)malloc(sizeof(char)*LINESIZE+1);
	if(commandCopy==NULL)
		return;

	/*source = (char*)malloc(sizeof(char)*LABELSIZE+1);
	if(source==NULL)
			return;
	destination = (char*)malloc(sizeof(char)*LABELSIZE+1);
	if(destination==NULL)
		return;*/

	strcpy(commandCopy, command);/*makes a copy of command without whitespaces by sides*/


	if((firstWord = strtok(command, white_space))==NULL)/*take first word*/
		return;	

	if((secondWord = strtok(NULL, white_space)) == NULL){/*take second word*/
		printf("Error, missing arguments in line number: %d\n", line_num);
		return;
	}

	thirdWord = strtok(NULL, white_space); /*take third word for check 3 words containing label positioning*/

	if(ispunct(firstWord[strlen(firstWord)-1])){
		if(firstWord[strlen(firstWord)-1] == ':'){/*if : at the end of word and it is label, check all parameters*/
			label = strtok(firstWord, ":");/*take label name*/
			if((isLabel = check_label_islegal(label, line_num)) == ERROR)/*check if label name is legal, turn on label flag*/
				return;
		}
		else {
			printf("Error, illegal punctuation mark after first word of command, in line number: %d\n", line_num);
			return;
		}
	}
	if(secondWord[0] == ','){
		printf("Error, illegal comma after first word of command, in line number: %d\n", line_num);
		return;
	}

	if(secondWord[0] == ':'){
		printf("Error, illegal label definition, in line number: %d\n", line_num);
		return;
		}

	if(isLabel)/*choose next word to check*/
		word = secondWord;
	else word = firstWord;

	if(word[0] == '.'){/*if is directive*/
		if((drctv_index = check_directive_islegal(word, line_num))==ERROR)/*if directive is not legal go to next line*/
			return;


		switch(drctv_index){/*switch by directive index*/
			case 0:/*.data*/ 
				if((operand_counter = check_nums(commandCopy, isLabel, line_num))==ERROR){/*check data parameters*/
					break;
				}

				if(isLabel) /*add label if exists*/{
					if((check_label_positioning(&(*head_lbl), &(*head_extern), label, LABEL, line_num))==ERROR)
						break;
					add_node_label(&(*head_lbl), &(*tail_lbl), label, line_num, LABEL);
				}
				add_data_arg(commandCopy, isLabel, line_num, &(*head_drctv), &(*tail_drctv));	
				break;	
			case 1:/*.string*/
				if(check_string_islegal(commandCopy, isLabel)==ERROR){
					printf("Error, string for .string directive is not legal, in line: %d\n", line_num);
					break;
				}

				if(isLabel){
					if((check_label_positioning(&(*head_lbl), &(*head_extern), label, LABEL, line_num))==ERROR)
						break;
					add_node_label(&(*head_lbl), &(*tail_lbl), label, line_num, LABEL);
				}
				add_string_arg(commandCopy, isLabel, line_num, &(*head_drctv), &(*tail_drctv));
				break;
			case 2: /*.struct*/
				if(!isLabel){
					printf("Error, struct definition without initialization, in line number: %d\n", line_num);
					break;
				}
				if((check_label_positioning(&(*head_lbl), &(*head_extern), label, STRUCT, line_num))==ERROR)
					break;
				check_struct_arg(commandCopy, line_num, isLabel);
				add_node_label(&(*head_lbl), &(*tail_lbl), label, line_num, STRUCT);
				add_struct_arg(commandCopy, isLabel, line_num, &(*head_drctv), &(*tail_drctv));
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
				cmd_index = check_cmd(secondWord);
			else cmd_index = check_cmd(firstWord);/*else check first word*/

			if(cmd_index==ERROR){/*command isn't legal*/
				printf("Error, undefined command name in line number: %d\n", line_num);
				return;
			}

			if((check_cmd_args(commandCopy, line_num, isLabel, cmd_index)) == ERROR)
				return;

			if(isLabel)
				add_node_label(&(*head_lbl), &(*tail_lbl), label, line_num, LABEL);

			if(isLabel){
				word = strtok(commandCopy, white_space);
				word = strtok(NULL, white_space);
			}
			else 
				word = strtok(commandCopy, white_space);

					
			if((ptr = strtok(NULL, ","))!=NULL){
					operand_counter++; /*operand counter*/
					/*strcpy(source, ptr);*/
					source = remove_blanks(ptr);
				if((ptr = strtok(NULL, white_space))!=NULL){
					operand_counter++; /*argument counter*/
					/*strcpy(destination, ptr);*/
					destination = remove_blanks(ptr);
				}
				else {
					destination = source;
					source = NULL;
				}
			}
			else/*if no operands for instruction*/
				destination = NULL;

			add_instruction_node(&(*head_cmd), &(*tail_cmd), source, destination, cmd_index, line_num, operand_counter, isLabel);

			if(operand_counter==1)/*free pointer returned from remove blanks*/
				free(destination);
			if(operand_counter==2){/*free pointers returned from remove blanks*/
				free(source);
				free(destination);
			}
		}
	free(commandCopy);

}/*end of checkcmdline func*/





/*Function receives directive .data line, it's details, head and tail of linked list of directives and adds operands of data directive to linked list*/
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
		add_directive_node(&(*head_drctv), &(*tail_drctv), line_num, isLabel, arg);/*adds directive arg into linked list*/	

	}

	free(number);	
}

/*Function receives directive .data line, it's details, head and tail of linked list of directives and adds operands of data directive to linked list*/
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
				if(lineCopy[i+1]=='\n')
					break;	
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



void add_struct_arg(char* line, int isLabel, int line_num, directiveLine **head_drctv, directiveLine **tail_drctv){
	int i;
	int inString = 0;
	char *white_space = " \t\v\f\r\n";
	char *ptr;
	short int arg;
	char *lineCopy = (char*)malloc(strlen(line)+1);
	if(lineCopy==NULL)
		return;
	strcpy(lineCopy, line);
	
	if(isLabel){
		ptr = strtok(lineCopy, white_space);
		ptr = strtok(NULL, white_space);
	}
	else ptr = strtok(lineCopy, white_space);
	
	ptr = strtok(NULL, ",");
	arg = (short)atoi(ptr);

	add_directive_node(&(*head_drctv), &(*tail_drctv), line_num, isLabel, arg);/*adds directive num arg into linked list*/	
	
	for(i=0; i<strlen(line); i++){
		if(lineCopy[i]=='"'){
			if(inString){
				if(lineCopy[i+1]=='\n')
					break;	
			}
			else inString = 1;
		}
		else {
			if(inString){
				add_directive_node(&(*head_drctv), &(*tail_drctv), line_num, isLabel, (short)lineCopy[i]);/*adds string arg into linked list*/	
			}		
		}	
	}
	add_directive_node(&(*head_drctv), &(*tail_drctv), line_num, isLabel, (short)0);/*adds arg with \0 char into linked list*/	
	free(lineCopy);
}

void add_instruction_node(cmdLine **head, cmdLine **tail, char* source, char* destination, int cmd_index, int line_num, int args, int isLabel){
	cmdLine *new = (cmdLine*)malloc(sizeof(cmdLine));
	if(new==NULL)
		return;
	
	if(!args){
		new->source=NULL;
		new->destination=NULL;
	}
	if(args==2){
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
	else if(args==1){
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
	else if(*tail == NULL){/*if this is second node*/
		(*head)->next = new;
		*tail = new;
	}
	else {
		(*tail)->next = new;
		*tail = new;
	}
}







/*Function receives directive line and it's details, creates and adds new node to linked list of opcode commands*/
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
		*tail = new;
		return;
	}
	else if(*tail == NULL){/*if this is second node*/
		(*head)->next = new;
		*tail = new;
	}
	else {
		(*tail)->next = new;
		*tail = new;
	}
}



/*Function frees nodes and linked list*/
void print_instruction_list(cmdLine* head){
	cmdLine* ptr;
	int i=1;
	ptr = head;

	while(ptr!=NULL){
		printf("%d: cmd_index: %d, source: %s, destination: %s, line_num: %d, num_args:%d\n", i, ptr->cmd_index, ptr->source, ptr->destination, ptr->line_num, ptr->args);
		 ptr = ptr->next;
		i++;
	}
}

/*Function frees nodes and linked list*/
void print_directive_list(directiveLine* head){
	directiveLine* ptr;
	int i=1;
	ptr = head;
	printf("Inside print directive node:\n");
	while(ptr!=NULL){
		printf("%d:  arg: %d, line_num: %d memory_num: %d\n", i, ptr->arg, ptr->line_num, ptr->memory_count);
		 ptr = ptr->next;
		i++;
	}
}







/*Function receives head of linkes list of instruction and line number of node need to be deleted, search this node and frees a memory of node and it's members*/
void delete_instruction_node(cmdLine **head, int line_num){
	cmdLine *ptr = *head;
	cmdLine *temp;

	if((*head)->line_num  == line_num){
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






