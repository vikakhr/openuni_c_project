#include "main.h" /*main libraries*/
#include "cmd_check.h" /*check functions*/
#include "second_step.h" /*functions*/
#include "label_lists.h"







/*Function frees nodes and linked list*/
void free_label_list(labels* head){
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



/*Function checks if string il legal and returns it's length, 0 otherwise
int check_string_length(char* string){
	string = remove_blanks(string);
	if(string[0] == '"' && string[strlen(string)-1] == '"')
		return strlen(string);
	else return 0; 

}





}*/

