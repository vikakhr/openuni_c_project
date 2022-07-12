#include "main.h" /*main libraries*/
#include "cmd_check.h" /*check functions*/
#include "second_step.h" /*functions*/
#include "label_lists.h"


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






}*/

