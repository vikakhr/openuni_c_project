#include "main.h"
#include "translator.h"
#include "label_lists.h"
#include "cmd_check.h"

void translate_lines(cmdLine **head_cmd, cmdLine **tail_cmd, directiveLine **head_drctv, labels **head_lbl, externs *head_extern){
	int memory_count = 100;
	int cmd_code_count, drctv_code_count;/*number of words of each type of command*/

	codeWords *head_code = NULL, *tail_code = NULL; /*head and tail of machine code list*/

	printf("Inside translate lines\n");
	memory_count = first_cmd_translation(&(*head_cmd),  &(*head_lbl), &head_code, &tail_code, memory_count);

	/*numerate directives*/
	/*second command translation*/
	/*make obj ouput*/



	print_code_list(&head_code);
} 


/*Function receives linked list of command lines, labels, decimal machine code and memory counter, makes first translations of instruction command to machine code and adds into linked list, counting memory*/
int first_cmd_translation(cmdLine **head_cmd, labels **head_lbl, codeWords **head_code, codeWords **tail_code, int memory_count){

	cmdLine *ptr_cmd = *head_cmd;/*instructions list*/
	codeWords *ptr_code = *head_code;
	short int code = 0;
	int num_s, num_d;
	short int unknown = -1;

	printf("Inside cmd translation\n");
	while(ptr_cmd!=NULL){
		code = code&&(ptr_cmd->cmd_index<<6);
		if(!ptr_cmd->args){
			add_node_code(&(*head_code), &(*tail_code), memory_count, code);/*if no arguments add with cmd index*/
			memory_count++;
			continue;
		}
		if(ptr_cmd->args==1){
			num_d = check_addressing_type(ptr_cmd->destination, &(*head_lbl), code);
			code = code&&(num_d<<2);
			/*put into linked list*/
			if(num_d==OPERAND_NUMBER || num_d==OPERAND_REGISTER){
				add_node_code(&(*head_code), &(*tail_code), memory_count, num_d);
				memory_count++;
				continue;
			}
			if(num_d == OPERAND_LABEL){/*if this is label' it's address still unknown*/
				add_node_code(&(*head_code), &(*tail_code), memory_count, unknown);
				memory_count++;
			}
			if(num_d == 1){/*if this is struct*/
				add_node_code(&(*head_code), &(*tail_code), memory_count, unknown);
				memory_count++;
				add_node_code(&(*head_code), &(*tail_code), memory_count, unknown);/*add it's number <<2*/
				memory_count++;
				
			}
			
			break;
		}
		if(ptr_cmd->args==2){
			num_s = check_addressing_type(ptr_cmd->source, &(*head_lbl), code);
			code = code&&(num_s<<4);

			num_d = check_addressing_type(ptr_cmd->destination, &(*head_lbl), code);
			code = code&&(num_d<<2);

			/*put into linked list*/
			/*add code line of source and destination*/
			/*translate_two_operands(char *source, char *destination, int source_type, int destination_type, int memory_count)*/
			memory_count++;
			break;
		}
	}
	return memory_count;
}		





int check_addressing_type(char *word, labels** head_lbl, int code){
	int type, num;
	labels *ptr_lbl = *head_lbl;/*label lists*/

	if((type = check_arg_number(word))!=ERROR)/*if number*/
			return OPERAND_NUMBER;

	else if((num = check_arg_register(word))!=ERROR)/*if register*/
			return	OPERAND_REGISTER;
	else{
		while(ptr_lbl!=NULL){
			if(!strcmp(ptr_lbl->label, word)){/*if this is label defined in this file*/		
				if(ptr_lbl->label_type==STRUCT)
					return OPERAND_STRUCT;
				else return OPERAND_LABEL;
			}
			ptr_lbl = ptr_lbl->next;
		}/*end od while*/
	}
	return OPERAND_EXTERN;/*if this is entry or extern label*/		
}

void translate_two_operands(char *source, char *destination, int source_type, int destination_type, int memory_count){
	int code = 0, num_s, num_d;
	if(source_type==OPERAND_REGISTER){
		num_s = check_arg_register(source);
		code = code&&num_s<<4;
		if(destination_type==OPERAND_REGISTER){/*if source and destination are registers*/
			num_d = check_arg_register(destination);
			code = code&&num_d<<2;	
			/*add code into linked list*/
			return;
		}
	}

	if(source_type==OPERAND_NUMBER){
		num_s = check_arg_number(source);
		/*add code into linked list*/
	}
	if(source_type==OPERAND_STRUCT){
		/*first add -1 into linked list, memorycount++*/
		/*second add number of struct, memorycount++*/
	}
	if(source_type==OPERAND_LABEL){
		/*add -1 into linked list*/

	}
	if(source_type==OPERAND_EXTERN){
		/*add 1 (extern) to linked list*/


	}
	if(destination_type==OPERAND_REGISTER){/*if source and destination are registers*/
		num_d = check_arg_register(destination);
		code = code&&num_d<<2;	
		/*add code into linked list*/
		return;
	}
	if(destination_type==OPERAND_NUMBER){
		num_s = check_arg_number(destination);
		/*add code into linked list*/
		return;
	}
	if(destination_type==OPERAND_STRUCT){
		/*first add -1 into linked list, memorycount++*/
		/*second add number of struct, memorycount++*/
		return;
	}
	if(destination_type==OPERAND_LABEL){
		/*add -1 into linked list*/
		return;
	}
	if(destination_type==OPERAND_EXTERN){
		/*add 1 (extern) to linked list*/
		return;

	}

}


/*Function receives head, tail and text to put into new node, creates new node with text and adds this node at the end of list of labels*/
void add_node_code(codeWords **head, codeWords **tail, int memory_count, int code){
	codeWords *new = malloc(sizeof(codeWords));
	if(new==NULL)
		return;
	
	new->code = code;
	new->memory_count = memory_count;
	new->next = NULL;

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

void print_code_list(codeWords* head){/*DELETE AFTER*/
	codeWords* ptr = head;

	
	printf("Inside print code list:\n");
	while(ptr!=NULL){
		printf("memory: %d, code: %d\n", ptr->memory_count, ptr->code);		
		ptr = ptr->next;
	}
	 
}


