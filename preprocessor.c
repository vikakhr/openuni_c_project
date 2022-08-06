/*This file contains preprocessor functions*/
#include "main.h" /*main libraries for output etc*/
#include "preprocessor.h" /*functions*/
#include "helper_func.h"/*for remove_blanks function*/
#include "cmd_check.h" /*for cmd name and directive name check functions*/

/*Function receives file name with extension and without it, creates .am file  * and writes source file after macro treatment.
 * Returns 1 if no errors, error otherwise*/
int preprocessor(char *file_name_extension, char *file_name){
	FILE *sfp, *dfp;/*sfp is .as source file, dfp is .am destination file*/
	node_macro *head = NULL, *tail = NULL;/*linked list nodes for macroes*/		
	char* macro = "macro";
	char* endmacro = "endmacro";
	char* command, *command_copy, *macro_name;
	int isInside = 0;/*flag if inside macro*/
	int isMacroInFile = 0;/*flag if macro exists in file*/

	char* destFileName = (char*)malloc(strlen(file_name)+4);/*allocates memory for new .am file*/
	if(destFileName==NULL)
		return ERROR;
	
	sprintf(destFileName,"%s.am", file_name);/*writes a full name of file with extension*/

	if((sfp = fopen(file_name_extension, "r")) == NULL){/*if cannot open source file - error*/
		printf("Cannot open %s\n", file_name_extension);
		free(destFileName);
		return ERROR;
	}

	if((dfp = fopen(destFileName, "w"))==NULL){/*if cannot read/create destination file - error*/
		printf("Cannot open %s\n", destFileName);
		free(destFileName);
		return ERROR;
	}

	command = (char*)malloc(sizeof(char)*LINESIZE+1);
	if(command==NULL)
		return ERROR;

	command_copy = (char *)malloc(sizeof(char)*LINESIZE+1);
	if(command_copy==NULL){
		free(command);/*free before exit*/
		return ERROR;
	}

	FOREVER {
		if(fgets(command, LINESIZE, sfp)==NULL)/*reads a line of LINESIZE length, checks if empty*/
			break;	

		strcpy(command_copy, command);/*makes a copy of original command*/

		if(isInside){/*if inside macro add this line to macro data*/
			if(is_one_word(command) && strstr(command, endmacro)){/*if endmacro found*/
				isInside = 0;
				continue;
			}
			put_macro_data(command, &head, &tail);/*add new data to this macro*/
			continue;
		}
		else if(strstr(command, macro)==NULL){/*line without "macro"*/
			if(is_one_word(command))/*if line contains one word*/
				if(isMacroInFile)/*if macro already exists in file*/
					if(write_macro_data(command, head, dfp))/*if this is macro name write and go to next line*/
						continue;
			fprintf(dfp, "%s", command_copy); /*write inside .am file*/
			continue;/*go to read next line*/
		}

		else{   				
			if(if_is_macro(command_copy)){/*if new macro was found*/
				isMacroInFile = 1;/*turn on macro exists in file flag*/
				isInside = 1; /*turn on inside macro flag*/
				if((macro_name = take_macro_name(command))==NULL)/*if malloc inside function failed*/
					break;
				add_node_macro(&head, &tail, macro_name);
				continue;
			}
			else {/*if macro name is illegal, ignore and write into file*/
				fprintf(dfp, "%s", command_copy); /*write inside .am file*/
				continue;
			}
		}

	}
	free(command);
	free(command_copy);

	fclose(sfp);
	fclose(dfp);

	free(destFileName);
	if(isMacroInFile){/*if linked list contains some nodes*/
		free_list(head);
	}
	return 1;
}

/*Function receives head, tail and text to put into new node, creates new node with text and adds this node at the end of list*/
void add_node_macro(node_macro** head, node_macro** tail, char* name){
	node_macro *new = (node_macro*)malloc(sizeof(node_macro));
	if(new==NULL)
		return;
	new->name = name;/*pointer received by remove blanks*/
	new->data = NULL;
	new->next = NULL;

	if(*head==NULL){/*if this is first node*/
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

/*Function receives line contains macro name, head and tail and adds a data to list*/
void put_macro_data(char* string, node_macro** head, node_macro** tail){
	node_macro* ptr;
	if(*tail==NULL)
		ptr = *head;
	else ptr = *tail;
	if(((ptr)->data) == NULL){
		(ptr)->data = (char*)malloc(strlen(string)+1);
		if(((ptr)->data)==NULL)
			return;
		strcpy((ptr)->data, string);
	}
	else {
		(ptr)->data = (char*)realloc((ptr)->data, strlen((ptr)->data)+1+strlen(string)+1);
		if(((ptr)->data)==NULL)
			return;
		strcat((ptr)->data, string);
	}
}

/*Function receives line, returns 1 if it contains one word, return 0 if it empty or contains more than one word*/
int is_one_word(char* string){
	char *p, *copy;	
	char *white_space = " \t\v\f\r\n";
	copy = (char*)malloc(strlen(string)+1);
	if(copy == NULL)
		exit(0);
	strcpy(copy, string);
	if((p=strtok(copy, white_space)) == NULL){/*empty*/
		free(copy);
		return 0;
	}
	if((p=strtok(NULL, white_space)) == NULL){/*one word line*/
		free(copy);		
		return 1;
	}
	free(copy);
	return 0;	
}

/*Function checks if this is macro command, returns 1 if yes, 0 otherwise*/
int if_is_macro(char* string){
	char *white_space = " \t\v\f\r";
	char* str = "macro";
	char *p, *copy;

	copy = remove_blanks(string);

	p = strtok(copy, white_space);
	if(strcmp(p, str)){/*if first word is not "macro"*/
		free(copy);
		return 0;
		
	}
	if((p = strtok(NULL, white_space))==NULL){/*no name of macro*/
		free(copy);
		return 0;
	}

	if((check_directive_name(p)!=ERROR) || (check_cmd_name(p)!=ERROR)){/*check if name of macro is not opcode or directive*/
		free(copy);
		return 0;
	}

	free(copy);/*free pointer allocated by remove blanks*/
	return 1;
}

/*Function checks if this is endmacro command, if yes returns 1, 0 otherwise*/
int if_is_endmacro(char* string){
	char *white_space = " \t\v\f\r";
	char *p;
	p = strtok(string, white_space);
	if(strcmp(p, "endmacro"))/*if not endmacro*/
		return 0;		
	return 1;
}

/*Function receives first line and returns name of macro*/
char* take_macro_name(char* string){
	char* p, *new_line, *macro_name;
	char *white_space = " \t\v\f\r";

	if((new_line = remove_blanks(string))==NULL)/*remove blanks by sides, check if malloc inside function not fails*/
		return NULL;

	macro_name = (char*)malloc(strlen(new_line)+1);
	if(macro_name == NULL)
		return NULL;

	p = strtok(new_line, white_space);
	p = strtok(NULL, white_space);
	strcpy(macro_name, p);
	free(new_line);
	return macro_name;
}

/*writes data into file*/
int write_macro_data(char* string, node_macro* head, FILE *dfp){
	node_macro* ptr = head;
	char *white_space = " \t\v\f\r";
	char *copy;
	copy = remove_blanks(string);

	strtok(copy, white_space);

	while(ptr!=NULL){
		if(!strcmp(ptr->name, copy)){/*search name of macro in the list*/
			fprintf(dfp, "%s", ptr->data);/*if found write data*/
			free(copy);
			return 1;
		}
		ptr = ptr->next;
	}
	free(copy);
	return 0;
}


/*Function receives a head of linked list and frees memory of each node*/
void free_list(node_macro* head){
	node_macro* ptr;

	while(head!=NULL){
		ptr = head;
		head = head->next;
		free(ptr->name);/*free memory of name*/
		free(ptr->data);/*free memory of data*/
		free(ptr);/*free memory of node*/
	}
}

