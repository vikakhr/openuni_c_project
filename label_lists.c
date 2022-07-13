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


/*ADD CHECK IF MALLOC FAILS??????????????*/
/*Function receives head, tail and text to put into new node, creates new node with text and adds this node at the end of list of labels*/
void add_node_label(labels** head, labels** tail, char* name, int line){
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




/*Function receives head, tail and text to put into new node, creates new node with text and adds this node at the end of list of structs*/
void add_node_struct(structs** head, structs** tail, char* name, int line){
	structs *new = malloc(sizeof(structs));
	new->struct_name = (char*)malloc(strlen(name)+1);
	new->line_number = line;
	new->next = NULL;
	strcpy(new->struct_name, name);
	printf("Inside addnode of struct\n");
	printf("Node is: %s %d\n", new->struct_name, new->line_number);
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

/*Function receives head, tail and text to put into new node, creates new node with text and adds this node at the end of list of structs*/
void add_node_entry_label(entryLabels** head, entryLabels** tail, char* name, int line){
	entryLabels *new = malloc(sizeof(entryLabels));
	new->label = (char*)malloc(strlen(name)+1);
	new->line_number = line;
	new->next = NULL;
	strcpy(new->label, name);
	printf("Inside addnode of entern label\n");
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

/*Function receives head, tail and text to put into new node, creates new node with text and adds this node at the end of list of structs*/
void add_node_extern_label(externLabels** head, externLabels** tail, char* name, int line){
	externLabels *new = malloc(sizeof(externLabels));
	new->label = (char*)malloc(strlen(name)+1);
	new->line_number = line;
	new->next = NULL;
	strcpy(new->label, name);
	printf("Inside addnode of extern label\n");
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


/*Function frees nodes and linked list*/
void print_struct_list(structs* head){
	structs* ptr;
	ptr = head;
	printf("Inside free struct trying to print\n");
	while(ptr!=NULL){
		printf("1. %s - %d\n", ptr->struct_name, ptr->line_number);
		 ptr = ptr->next;
		
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




/*Function frees nodes and linked list*/
void print_label_list(labels* head){
	labels* ptr;
	ptr = head;
	printf("Inside free label trying to print\n");
	while(ptr!=NULL){
		printf("1. %s - %d\n", ptr->label, ptr->line_number);
		 ptr = ptr->next;
		
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




/*Function prints linked list*/
void print_extern_labels(externLabels* head){
	externLabels* ptr;
	ptr = head;
	printf("Inside extern label trying to print\n");
	while(ptr!=NULL){
		printf("1. %s - %d\n", ptr->label, ptr->line_number);
		 ptr = ptr->next;
		
		/*free(ptr->name);free memory of name
		free(ptr);/*free memory ofnode*/
	}
}


/*Function prints linked list*/
void print_entry_labels(entryLabels* head){
	entryLabels* ptr;
	ptr = head;
	printf("Inside entry label trying to print\n");
	while(ptr!=NULL){
		printf("* %s - %d\n", ptr->label, ptr->line_number);
		 ptr = ptr->next;
	
		/*free(ptr->name);free memory of name
		free(ptr);/*free memory of node*/
	}
}






