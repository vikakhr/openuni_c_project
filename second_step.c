#include "main.h" /*main libraries*/
#include "cmd_check.h" /*check functions*/
#include "second_step.h" /*functions*/
#include "label_lists.h"
#include "first_step.h"

/*Function receives command line and it's details, creates and adds new node to linked list of opcode commands
void add_instruction_code(opcodeLine **head, opcodeLine **tail, char *line, char *label, int opcode_index){
	opcodeLine *new;

	new = malloc(sizeof(opcodeLine));
	if(new==NULL)
		return;
	
	new->cmd_line = (char*)malloc(strlen(line)+1);
	if(new->cmd_line==NULL)
		return;
		
	new->label = (char*)malloc(strlen(label)+1);
	if(new->label==NULL)
		return;
	

	strcpy(new->label, label);
	strcpy(new->cmd_line, line);
	new->opcode_index = opcode_index;
	
	
}

*/

/*Function receives command line and it's details, creates and adds new node to linked list of opcode commands*/
void add_instruction_node(instructionLine **head, instructionLine **tail, int line_num, char *line, int isLabel, int instruction_index){
	instructionLine *new = malloc(sizeof(instructionLine));
	if(new==NULL)
		return;
	
	new->instruction_line = (char*)malloc(strlen(line)+1);
	if(new->instruction_line==NULL)
		return;
		
	printf("Inside add instruction node\n");
	strcpy(new->instruction_line, line);
	new->instruction_index = instruction_index;
	new->isLabel = isLabel;
	new->line_num = line_num;
	
	if(*head==NULL){/*if this is first node*/
		*head = new;
		*tail = new;
		printf("Head is: %s\n", (*head)->instruction_line);		
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


/*Function receives directive line and it's details, creates and adds new node to linked list of opcode commands*/
void add_directive_node(directiveLine **head, directiveLine **tail, int line_num, char *line, int isLabel, int directive_type){
	directiveLine *new = malloc(sizeof(directiveLine));
	if(new==NULL)
		return;
	
	new->drctv_line = (char*)malloc(strlen(line)+1);
	if(new->drctv_line==NULL)
		return;

	strcpy(new->drctv_line, line);
	new->directive_type = directive_type;
	new->isLabel = isLabel;
	new->line_num = line_num;
	
	if(*head==NULL){/*if this is first node*/
		*head = new;
		*tail = new;
		printf("Head is: %s\n", (*head)->drctv_line);		
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
