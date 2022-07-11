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




/*Function receives head, tail and text to put into new node, creates new node with text and adds this node at the end of list*/
void add_node(labels** head, labels** tail, char* name, int line){
	labels *new = malloc(sizeof(labels));
	new->label = (char*)malloc(strlen(name)+1);
	new->line_number = line;
	new->next = NULL;
	strcpy(new->label, name);
	printf("Inside addnode\n");
	printf("Node is: %s %d\n", new->label, new->line_number);
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

