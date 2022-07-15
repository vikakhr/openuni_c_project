#include "main.h"
#include "label_lists.h"



/*Function receives name of label and check of there are no same labels in the linked list. Returns 0 of name already exist, 1 otherwise*/
int check_repeated_labels(char* name, labels* head){
	labels* ptr = head;
	
	while(ptr!=NULL){
		if(!strcmp(ptr->label, name)){/*list of labels already has this label*/

			return 0;
		}
		ptr = ptr->next;
	}
	return 1;
	
}

/*Function receives head and tail of linked list of labels, entry/extern label and it's details and checks if this label is already defined as extern or entry.
Returns error if label has been defined second time with another positioning, 1 if no error found*/
int check_label_positioning(labels** head, labels** tail, char* label, int label_type, int line_num){
	labels *ptr = *head;
	int search_type;
	if(label_type == ENTRY)/*define which label type we are checking for errors*/
		search_type = EXTERN;
	else search_type = ENTRY;

	while(ptr!=NULL){
		if(!strcmp(label, ptr->label)){
			printf("%d - %d\t", ptr->label_type, search_type);
			if(ptr->label_type == search_type){
			printf("Error, conflicting positioning type for label defined multiple times, in line number: %d\n", line_num);
		 	return ERROR;
			}
			printf("Warning, repeated label positioning definition - will be ignored, in line number: %d\n", line_num);
			return ERROR;
		}
		ptr = ptr->next;
	}
	return 1;
}







/*DELETE MALLOC AND MAKE LABEL SIZE??????????????*/
/*Function receives head, tail and text to put into new node, creates new node with text and adds this node at the end of list of labels*/
void add_node_label(labels** head, labels** tail, char* name, int line, int label_type){
	labels *new = malloc(sizeof(labels));
	if(new==NULL)
		return;
	new->label = (char*)malloc(strlen(name)+1);
	if(new->label==NULL)
		return;
	new->line_number = line;
	new->label_type = label_type;
	new->memory_number = 0;
	new->next = NULL;
	strcpy(new->label, name);
	printf("Inside add to label list: %s\n", new->label);
	if(*head==NULL){/*if this is first node*/
		*head = new;
		*tail = new;
		printf("Head is: %s\n", (*head)->label);		
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
void print_label_list(labels* head){
	labels* ptr;
	ptr = head;
	int i=1;
	printf("Inside print label:\n");
	while(ptr!=NULL){
		printf("%d - %s - %d\n", i, ptr->label, ptr->label_type);
		 ptr = ptr->next;
		i++;
		/*free(ptr->name);free memory of name
		free(ptr);/*free memory of node*/
	}


	/*while(head!=NULL){
		ptr = head;
		 head = head->next;
		printf("1. %s - %d\n", ptr->label, ptr->line_number);
		/*free(ptr->name);free memory of name
		free(ptr);/*free memory of node*/
	

}











