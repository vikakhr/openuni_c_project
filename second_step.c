#include "main.h" /*main libraries*/
#include "cmd_check.h" /*check functions*/
#include "second_step.h" /*functions*/
#include "label_lists.h"
#include "first_step.h"



void add_instruction_node(cmdLine **head, cmdLine **tail, char* source, char* destination, int cmd_index, int line_num, int args, int isLabel){
	cmdLine *new = malloc(sizeof(cmdLine));

	if(new==NULL)
		return;
	if(*head==NULL)
		printf("%d %d\n", *head, *tail);
	printf("Inside add instr node: %s\n", source);
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
	new->is_label = isLabel;

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
void add_directive_node(directiveLine **head, directiveLine **tail, int line_num, int isLabel, int arg){
	directiveLine *new = malloc(sizeof(directiveLine));
	if(new==NULL)
		return;
	
	new->arg = arg;
	new->isLabel = isLabel;
	new->line_num = line_num;
	new->memory_count = 0;

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
	ptr = head;
	int i=1;
	printf("Inside print instruction node:\n");
	while(ptr!=NULL){

		printf("%d: cmd_index: %d, source: %s, destination: %s, line_num: %d, num_args:%d\n", i, ptr->cmd_index, ptr->source, ptr->destination, ptr->line_num, ptr->args);
		
		 ptr = ptr->next;
		i++;
	}
}

/*Function frees nodes and linked list*/
void print_directive_list(directiveLine* head){
	directiveLine* ptr;
	ptr = head;
	int i=1;
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





