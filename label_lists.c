#include "main.h"
#include "label_lists.h"





/*Function receives name of label and check of there are no same labels in the linked list. Returns 0 of name already exist, 1 otherwise*/
int check_repeated_labels(char* name, labels* head){
	labels* ptr = head;
	
	while(ptr!=NULL){
		if(!strcmp(ptr->label, name)){/*list of labels already has this label name*/
		return 0;
		}
		ptr = ptr->next;
	}
	return 1;
	
}

/*Function receives head and tail of linked list of labels, entry/extern label and it's details and checks if this label is already defined as extern or entry.
Returns error if label has been defined second time with another positioning, 1 if no error found*/
int check_label_positioning(labels** head, externs** ext_head, char* label, int label_type, int line_num){
	labels *ptr = *head;
	externs *ptr_ext = *ext_head;
	printf("Inside check label positioning , LABEL TYPE: %d\n", label_type);
	while(ptr_ext!=NULL){
		if(!(strcmp(label, ptr_ext->ext_label))){
			if(label_type == ENTRY || label_type == LABEL || label_type == STRUCT){
				printf("Error, conflicting positioning type for label defined multiple times, in line number: %d\n", line_num);
		 		return ERROR;
			}
			else if(label_type == EXTERN){
				printf("Warning, repeated label positioning definition - will be ignored, in line number: %d\n", line_num);
				return ERROR;
			}
		}
		ptr_ext = ptr_ext->next;
	}

	while(ptr!=NULL){
		if(!(strcmp(label, ptr->label))){
			if(label_type == EXTERN){
				printf("Error, conflicting positioning type for label defined multiple times, in line number: %d\n", line_num);
		 		return ERROR;
			}
			else if(label_type == ENTRY && ptr->label_type == ENTRY){
				printf("Warning, repeated label positioning definition - will be ignored, in line number: %d\n", line_num);
				return ERROR;
			}		
		}
		ptr = ptr->next;
	}
	return 1;
}



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
	new->memory_count = 0;
	new->next = NULL;
	strcpy(new->label, name);
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

/*Function receives head, tail and text to put into new node, creates new node with text and adds this node at the end of list of labels*/
void add_node_extern(externs** head, externs** tail, char* name){
	externs *ptr = *head;
	externs *new = malloc(sizeof(externs));
	if(new==NULL)
		return;

	new->ext_label = (char*)malloc(strlen(name)+1);
	if(new->ext_label==NULL)
		return;

	new->next = NULL;
	strcpy(new->ext_label, name);

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
void print_extlabel_list(externs* head){
	externs* ptr = head;
	int i=1;
	printf("Inside print extern label:\n");
	while(ptr!=NULL){
		printf("%d - %s d\n", i, ptr->ext_label);
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

/*Function frees nodes and linked list*/
void print_label_list(labels* head){
	labels* ptr;
	ptr = head;
	int i=1;
	printf("Inside print label:\n");
	while(ptr!=NULL){
		printf("%d - %s - %d,line: %d memory_num: %d\n", i, ptr->label, ptr->label_type, ptr->line_number,ptr->memory_count);
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





/*Function receives head of instruction lines, labels and extern labels. Checks if label in argument of instruction is defined in label tables, if not prints error
message and deletes error line from linked list of instructions*/
void check_label_defined(labels** head_label, externs **head_ext, cmdLine **head_cmd){
	labels *ptr_label = *head_label;
	externs *ptr_ext = *head_ext;
	cmdLine *temp;
	cmdLine *ptr_cmd = *head_cmd;
	char *strct_name;

	if(ptr_cmd->source!=NULL){
		if(strchr(ptr_cmd->source, '.')){/*if struct*/
			strct_name = strtok(ptr_cmd->source, ".");
			while(ptr_cmd!=NULL){
				if(!(strcmp(ptr_label->label, strct_name)))/*if struct was found*/					
					break;
				ptr_label = ptr_label->next;
			}
			temp = ptr_cmd;
			ptr_cmd = ptr_cmd->next;
			delete_instruction_node(head_cmd,temp->line_num);/*deletes and frees memory of node that contains error*/
		}
		else if((check_arg_register(ptr_cmd->source))==ERROR && (check_arg_number(ptr_cmd->source))==ERROR){/*if source not number or register*/
			while(ptr_cmd!=NULL){
				if(ptr_cmd->source!=NULL){
					while(ptr_label!=NULL){/*check if source is label*/
						if(!(strcmp(ptr_label->label, ptr_cmd->source)))/*if label was found*/					
							break;
						ptr_label = ptr_label->next;
					}
					while(ptr_ext!=NULL){/*check if source is extern label*/
						if(!(strcmp(ptr_ext->ext_label, ptr_cmd->source)))/*if extern label*/					
							break;
						ptr_ext = ptr_ext->next;
					}
					printf("Error, label name of source parameter is not defined, int line_number: %d\n", ptr_cmd->line_num);
					temp = ptr_cmd;
					ptr_cmd = ptr_cmd->next;
					delete_instruction_node(head_cmd,temp->line_num);/*deletes and frees memory of node that contains error*/
				}
			ptr_cmd = ptr_cmd->next;
			}
		}
	}
	if(ptr_cmd->destination!=NULL){
		ptr_label = *head_label;
		ptr_cmd = *head_cmd;
		ptr_ext = *head_ext;
		if(strchr(ptr_cmd->destination, '.')){/*if struct*/
			strct_name = strtok(ptr_cmd->destination, ".");
			while(ptr_cmd!=NULL){
				if(!(strcmp(ptr_label->label, strct_name)))/*if struct was found*/					
					return;
				ptr_label = ptr_label->next;
			}
			temp = ptr_cmd;
			ptr_cmd = ptr_cmd->next;
			delete_instruction_node(head_cmd,temp->line_num);/*deletes and frees memory of node that contains error*/
		}
		else if((check_arg_register(ptr_cmd->destination))==ERROR && (check_arg_number(ptr_cmd->destination))==ERROR){/*if destination not number or register*/
			printf("Destination is not register nor number: %s\n", ptr_cmd->destination);
			while(ptr_cmd!=NULL){
				if(ptr_cmd->destination!=NULL){
					while(ptr_label!=NULL){/*check if destination is label*/
						if(!(strcmp(ptr_label->label, ptr_cmd->destination))){/*if label was found*/
							return;
						}
						ptr_label = ptr_label->next;
					}
					while(ptr_ext!=NULL){/*check if destination is extern label*/
						if(!(strcmp(ptr_ext->ext_label, ptr_cmd->destination)))/*if extern label*/					
							return;
						ptr_ext = ptr_ext->next;
					}
					ptr_cmd->destination = remove_blanks(ptr_cmd->destination);
					printf("Error, label name of destination parameter is not defined, int line_number: %d\n", ptr_cmd->line_num);
					temp = ptr_cmd;
					ptr_cmd = ptr_cmd->next;
					delete_instruction_node(head_cmd,temp->line_num);/*deletes and frees memory of node that contains error*/
				}
			ptr_cmd = ptr_cmd->next;
			}
		}
	}

}





