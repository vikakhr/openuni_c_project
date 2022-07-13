#include "main.h" /*main libraries*/
#include "cmd_check.h" /*check functions*/
#include "second_step.h" /*functions*/
#include "label_lists.h"

/*Function receives command line and it's details, creates and adds new node to linked list of opcode commands*/
void add_opcode_node(struct opcodeLine **head, struct opcodeLine **tail, char *line, char *label, int opcode_index){
	opcodeLine *new = malloc(sizeof(opcodeLine));
	if(new==NULL)
		return;
	
	strcpy(new->label, label);
	strcpy(new->cmd_line, line);
	new->opcode_index = opcode_index;
	
	if(*head==NULL){/*if this is first node*/
		*head = new;
		*tail = new;
		printf("Head is: %s\n", (*head)->cmd_line);		
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
void add_directive_node(struct node_directive **head, struct node_directive **tail, char *line, char *label, int directive_type){
	node_directive *new = malloc(sizeof(node_directive));
	if(new==NULL)
		return;
	
	strcpy(new->label, label);
	strcpy(new->drctv_line, line);
	new->directive_type = directive_type;
	
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
