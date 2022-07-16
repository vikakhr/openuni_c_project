#include "main.h" /*main libraries*/
#include "cmd_check.h" /*check functions*/
#include "second_step.h" /*functions*/
#include "label_lists.h"
#include "first_step.h"



void add_instruction_node(instructionLine **head, instructionLine **tail, char* source, char* destination, int cmd_index, int line_num, int args){
	instructionLine *new = malloc(sizeof(instructionLine));
	if(new==NULL)
		return;
	if(*head==NULL)
		printf("%d %d\n", *head, *tail);

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
		if(new->destination==NULL)
			return;
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
	printf("Inside add instruction node: %d %s %s %d %d %d\n", cmd_index, new->source, new->destination, new->line_num, new->cmd_index, new->args);
	if(*head!=NULL)
		printf("%d %d\n", *head, *tail);
	if(*head == NULL){/*if this is first node*/
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



/*Function frees nodes and linked list*/
void print_instruction_list(instructionLine* head){
	instructionLine* ptr;
	ptr = head;
	int i=1;
	printf("Inside print instruction node:\n");
	while(ptr!=NULL){
		printf("Before i++, %d\n", ptr);
		printf("%d: cmd_index: %d, source: %s, destination: %s, line_num: %d, num_args:%d\n", i, ptr->cmd_index, ptr->source, ptr->destination, ptr->line_num, ptr->args);
		
		 ptr = ptr->next;
		i++;
		printf("After i++\n");
	}

	printf("After\n");
	

}
