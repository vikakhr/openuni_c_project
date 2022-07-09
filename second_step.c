#include "main.h" /*main libraries*/
#include "main_data.h" /*linked lists to check or save data*/
#include "cmd_check.h" /*check functions*/
#include "second_step.h" /*functions*/





/*Function receives head, tail and text to put into new node, creates new node with text and adds this node at the end of list*/
void add_node(node_label** head, node_label** tail, char* name, int line){
	node_label *new = malloc(sizeof(node_label));
	new->name = (char*)malloc(strlen(name)+1);
	new->line_number = line;
	new->next = NULL;
	strcpy(new->name, name);
	printf("Inside addnode\n");
	printf("Node is: %s %d\n", new->name, new->line_number);
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



/*Function counts command's memory number*/
int count_memory_num(char* line, int num){
	char *white_space = " \t\v\f\r\n";
	char *p;
	if(p = strtok(line, white_space)!=NULL)
		num++;
	while(p = strtok(NULL, white_space)!=NULL)
		num++;
	return num;

}



/*Function frees nodes and linked list*/
void free_label_list(node_label* head){
	node_label* ptr;

	while(head!=NULL){
		ptr = head;
		head = head->next;
		printf("1. %s - %d\n", ptr->name, ptr->line_number);
		free(ptr->name);/*free memory of name*/
		free(ptr);/*free memory of node*/
	}
}



/*Function checks if string il legal and returns it's length, 0 otherwise*/
int check_string_length(char* string){
	string = remove_blanks(string);
	if(string[0] == '"' && string[strlen(string)-1] == '"')
		return strlen(string);
	else return 0; 

}






/*void add_error_line(node_error* head, node_error* tail, int line_num){
	node_error* ptr;
	
Function to add line-number of error into linked list, no need to arrange




}*/

