#include "main.h"
#include "translator.h"
#include "first_step.h"
#include "label_lists.h"
#include "cmd_check.h"

void translate_lines(codeWords **head_code, codeWords **tail_code, cmdLine **head_cmd, cmdLine **tail_cmd, directiveLine **head_drctv, labels **head_lbl, externs *head_extern){
	int memory_count = STARTMEMORY;
	int cmd_code_count, drctv_code_count;/*number of words of each type of command*/

	memory_count = first_cmd_translation(&(*head_cmd),  &(*head_lbl), &(*head_code), &(*tail_code), memory_count);
	/*Here free cmd lines list*/
	cmd_code_count = memory_count - (STARTMEMORY+1);
	memory_count = add_drctv_memory_count(&(*head_drctv), memory_count);
	drctv_code_count = memory_count - cmd_code_count - (STARTMEMORY+1);

	printf("Commands: %d, directives: %d\n", cmd_code_count, drctv_code_count);
	add_address_of_labels(&(*head_code),  &(*head_lbl));

	/*make obj ouput*/

	print_code_list(*head_code);
	print_directive_list(*head_drctv);
	print_label_list(*head_lbl);
} 


/*Function receives linked list of command lines, labels, decimal machine code and memory counter, makes first translations of instruction command to machine code and adds into linked list, counting memory*/
int first_cmd_translation(cmdLine **head_cmd, labels **head_lbl, codeWords **head_code, codeWords **tail_code, int memory_count){

	cmdLine *ptr_cmd = *head_cmd;/*instructions list*/
	short int code;
	int num_s, num_d;


	printf("Inside cmd translation\n");
	while(ptr_cmd!=NULL){
		code = 0;
		printf("Command to translate:  %d  %s  %s %d %d\n", ptr_cmd->cmd_index, ptr_cmd->source, ptr_cmd->destination, ptr_cmd->line_num, ptr_cmd->args);
		code = code|(ptr_cmd->cmd_index<<6);
		if(ptr_cmd->is_label){
			add_label_memory_num(&(*head_lbl), memory_count, ptr_cmd->line_num);
		}
		if(!ptr_cmd->args){
			add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);/*if no arguments add with cmd index*/
			memory_count++;
			ptr_cmd = ptr_cmd->next;
			continue;
		}
		if(ptr_cmd->args==1){
			num_d = check_addressing_type(ptr_cmd->destination, &(*head_lbl), code);
			code = code|(num_d<<2);
			add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);/*if no arguments add with cmd index*/
			memory_count++;
			memory_count = translate_one_operand(ptr_cmd->destination, num_d, memory_count, ptr_cmd->line_num, &(*head_code), &(*tail_code));
			ptr_cmd = ptr_cmd->next;
			continue;
		}

		if(ptr_cmd->args==2){
			num_s = check_addressing_type(ptr_cmd->source, &(*head_lbl), code);
			code = code|(num_s<<4);

			num_d = check_addressing_type(ptr_cmd->destination, &(*head_lbl), code);
			code = code|(num_d<<2);
			add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);
			memory_count++;

			memory_count = translate_two_operands(ptr_cmd->source, ptr_cmd->destination, num_s, num_d, memory_count, ptr_cmd->line_num, &(*head_code), &(*tail_code));
			ptr_cmd = ptr_cmd->next;
			continue;
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

int translate_one_operand(char *destination, int destination_type, int memory_count, int line_number, codeWords **head_code, codeWords **tail_code){
	int i, value;
	short int unknown = -1;
	short int code = 0;
	char *ptr;
	printf("Inside translate one operand, %s\n", destination);
	switch(destination_type){
		case OPERAND_NUMBER: 
			value = strtol(destination, &ptr, 10);
			code = (short)value<<2;
			add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);
			memory_count++;
			break;
		case OPERAND_LABEL:	
			printf("Is label\n");
			add_node_code(&(*head_code), &(*tail_code), memory_count, unknown, destination);
			memory_count++;
			break;
		case OPERAND_STRUCT:	
			add_node_code(&(*head_code), &(*tail_code), memory_count, unknown, destination);
			memory_count++;
			if(destination[strlen(destination)-1]=='1')
				code = 1<<2;
			else code = 2<<2;
			add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);/*add it's number <<2*/
			memory_count++;
			break;
		case OPERAND_REGISTER:
			for(i=0; i<REGLENGTH; i++){
				if(!strcmp(destination, REGISTER[i])){
					code = code|(i<<2);
					add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);
					memory_count++;
				}
			}	
			break;
				
		case OPERAND_EXTERN:
			add_node_code(&(*head_code), &(*tail_code), memory_count, 1, NULL);/*add extern type*/
			memory_count++;
			break;

		break;
	}
	return memory_count;


}

int translate_two_operands(char *source, char *destination, int source_type, int destination_type, int memory_count, int line_number, codeWords **head_code, codeWords **tail_code){
	int num_s, num_d;
	int code = 0;
	int i, value;
	char *ptr;
	int unknown = -1;

	printf("Inside translate two operands\n");
	if(source_type==OPERAND_REGISTER){
		for(i=0; i<REGLENGTH; i++){
			if(!strcmp(source, REGISTER[i]))
				num_s = i;
		}
		code = code|num_s<<6;
		if(destination_type==OPERAND_REGISTER){/*if source and destination are registers*/
			for(i=0; i<REGLENGTH; i++){
				if(!strcmp(destination, REGISTER[i]))
					num_d = i;
			}
			code = code|num_d<<2;	
			add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);
			return ++memory_count;
		}
	}

	switch(source_type){
			case OPERAND_NUMBER:
				value = strtol(destination, &ptr, 10);
				code = (short)value<<4;
				add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);
				memory_count++;
				break;
			case OPERAND_LABEL:
				add_node_code(&(*head_code), &(*tail_code), memory_count, unknown, source_type);
				memory_count++;
				break;
			case OPERAND_STRUCT:
				add_node_code(&(*head_code), &(*tail_code), memory_count, unknown, source_type);
				memory_count++;
				if(destination[strlen(destination)-1]=='1')
					code = 1<<4;
				else code = 2<<4;
				add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);/*add it's number <<2*/
				memory_count++;
				break;
			case OPERAND_REGISTER:
				for(i=0; i<REGLENGTH; i++){
					if(!strcmp(destination, REGISTER[i])){
						code = code|(i<<4);
						add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);
						memory_count++;
					}
				}
				break;

			case OPERAND_EXTERN:
				add_node_code(&(*head_code), &(*tail_code), memory_count, 1, NULL);/*add extern type*/
				memory_count++;
				break;

			break;
		}
	translate_one_operand(destination, destination_type, memory_count, line_number, &(*head_code), &(*tail_code));


	return memory_count;

}


/*Function receives head, tail and text to put into new node, creates new node with text and adds this node at the end of list of labels*/
void add_node_code(codeWords **head, codeWords **tail, int memory_count, int code, char *literal){
	codeWords *new = malloc(sizeof(codeWords));
	if(new==NULL)
		return;

	if(literal == NULL)
		new->literal = literal;
	else {
		new->literal = (char *)malloc(strlen(literal)+1);
		if(new->literal == NULL)
			return;
		strcpy(new->literal, literal);
	}

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

/*Function re*/
void add_address_of_labels(codeWords **head_code, labels **head_lbl){
	codeWords *ptr_code = *head_code;
	labels *ptr_label = *head_lbl;
	int unknown = -1, isFound = 0;;

	while(ptr_code!=NULL){
		if(isFound){
			ptr_label = *head_lbl;
			isFound = 0;
		}
		if(ptr_code->code==unknown){
			while(ptr_label!=NULL && !isFound){
				if(!strcmp(ptr_label->label, ptr_code->literal)){
					ptr_code->code = ptr_label->memory_count;
					isFound = 1;
				}
				ptr_label = ptr_label->next;
			}
		}
		ptr_code = ptr_code->next;
	}
}

int add_drctv_memory_count(directiveLine **head_drctv, int memory_count){
	directiveLine *ptr_drctv = *head_drctv;
	while(ptr_drctv!=NULL){
		ptr_drctv->memory_count = memory_count;
		memory_count++;
		ptr_drctv = ptr_drctv->next;
	}

		return memory_count;
}

