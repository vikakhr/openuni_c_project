#include "main.h"
#include "translator.h"
#include "label_lists.h"
#include "cmd_check.h"

void translate_lines(char *file_name, codeWords **head_code, codeWords **tail_code, cmdLine **head_cmd, cmdLine **tail_cmd, directiveLine **head_drctv, labels **head_lbl){
	int memory_count = STARTMEMORY;
	int cmd_code_count, drctv_code_count;/*number of words of each type of command*/
	ext *head_extern = NULL, *tail_extern;
	char *base_32 = (char*)malloc(3);/*string for base 32 representation*/
	if(base_32==NULL)
		return;

	memory_count = first_cmd_translation(&(*head_cmd),  &(*head_lbl), &(*head_code), &(*tail_code), &head_extern, &tail_extern, memory_count);

	cmd_code_count = memory_count - STARTMEMORY;
	memory_count = add_drctv_memory_count(&(*head_drctv), &(*head_lbl), memory_count);
	drctv_code_count = memory_count - cmd_code_count - STARTMEMORY;

	add_address_of_labels(&(*head_code),  &(*head_lbl));

	translate_and_output(file_name, cmd_code_count, drctv_code_count, &(*head_code), &(*head_drctv), base_32);
	output_entry_labels(file_name, &(*head_lbl), base_32);
	output_and_free_ext_labels(file_name, head_extern, base_32);
	free(base_32);
} 

/*Function receives linked list of command lines, labels, decimal machine code and memory counter,
 * makes first translations of command to machine code and adds into linked list, counting memory. Returns memory counter one after last*/
int first_cmd_translation(cmdLine **head_cmd, labels **head_lbl, codeWords **head_code, codeWords **tail_code, ext **head_ext, ext **tail_ext, int memory_count){
	cmdLine *ptr_cmd = *head_cmd;/*instructions list*/
	short int code;
	int num_s, num_d;

	while(ptr_cmd!=NULL){
		code = 0;
		code = code|(ptr_cmd->cmd_index<<6);/*opcode  places 6-9*/
		if(ptr_cmd->is_label){
			add_label_memory_num(&(*head_lbl), memory_count, ptr_cmd->line_num);
		}
		if(!ptr_cmd->args){
			add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);/*if no operands add with cmd index*/
			memory_count++;
			ptr_cmd = ptr_cmd->next;
			continue;
		}
		if(ptr_cmd->args==1){/*if one operand*/
			num_d = check_addressing_type(ptr_cmd->destination, &(*head_lbl));
			if(num_d == OPERAND_EXTERN)/*here extern is the same addressing type - label*/
				code = code|(OPERAND_LABEL<<2);
			else code = code|(num_d<<2); /*addressing type of destination, places 2-3*/
			add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);/*if no arguments add with cmd index*/
			memory_count++;
			/*after adding first "word" translate and add destination*/
			memory_count = translate_one_operand(ptr_cmd->destination, num_d, memory_count, ptr_cmd->line_num, &(*head_code), &(*tail_code), &(*head_ext), &(*tail_ext));
			ptr_cmd = ptr_cmd->next;
			continue;
		}

		if(ptr_cmd->args==2){/*if two operands*/
			num_s = check_addressing_type(ptr_cmd->source, &(*head_lbl));
			if(num_s == OPERAND_EXTERN)/*here extern is the same type - label*/
				code = code|(OPERAND_LABEL<<4);
			else code = code|(num_s<<4);/*addressing type of source, places 4-5*/

			num_d = check_addressing_type(ptr_cmd->destination, &(*head_lbl));
			if(num_d == OPERAND_EXTERN)/*here extern is the same type - label*/
				code = code|(OPERAND_LABEL<<2);
			else code = code|(num_d<<2);/*addressing type of destination, places 2-3*/
			add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);
			memory_count++;
			/*after adding first "word" translate and add source and destination*/
			memory_count = translate_two_operands(ptr_cmd->source, ptr_cmd->destination, num_s, num_d, memory_count, ptr_cmd->line_num, &(*head_code), &(*tail_code), &(*head_ext), &(*tail_ext));
			ptr_cmd = ptr_cmd->next;
			continue;
		}
	}
	return memory_count;
}		

/*Function receives argument of command, head of label list and returns addressing type of argument*/
int check_addressing_type(char *word, labels** head_lbl){
	int type, num;
	labels *ptr_lbl = *head_lbl;/*label lists*/

	if(strchr(word, '.'))/*if struct*/
		return OPERAND_STRUCT;

	if((type = check_operand_number(word))!=INT_MAX)/*if number*/
		return OPERAND_NUMBER;

	if((num = check_operand_register(word))!=ERROR)/*if register*/
		return OPERAND_REGISTER;

	while(ptr_lbl!=NULL){
		if(!strcmp(ptr_lbl->label, word))/*if this is label defined in this file*/
			return OPERAND_LABEL;
		ptr_lbl = ptr_lbl->next;
	}/*end of while*/

	return OPERAND_EXTERN;
}

/*Function receives destination, it's type, details of line, head and tail of labels. Translates destination to binary machine code. Returns memory counter one after last*/
int translate_one_operand(char *destination, int destination_type, int memory_count, int line_number, codeWords **head_code, codeWords **tail_code, ext **head_ext, ext **tail_ext){
	int num;
	short int unknown = -1; /*if binary representation is unknown on this step*/
	short int code = 0; /*binary representation*/

	switch(destination_type){
		case OPERAND_NUMBER:
			num = atoi(++destination);/*translate number after #*/
			code = (short)num<<2;
			add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);
			memory_count++;
			break;
		case OPERAND_LABEL:
			add_node_code(&(*head_code), &(*tail_code), memory_count, unknown, destination);
			memory_count++;
			break;
		case OPERAND_STRUCT:
			if(destination[strlen(destination)-1]=='1')/*num of struct field*/
				code = 1<<2;
			else code = 2<<2;

			add_node_code(&(*head_code), &(*tail_code), memory_count, unknown, destination);/*memory count is unknown*/
			memory_count++;
			add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);/*add it's number <<2*/
			memory_count++;
			break;
		case OPERAND_REGISTER:
			num = check_operand_register(destination);/*take index of register*/
			code = code|(num<<2);
			add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);
			memory_count++;
			break;
				
		case OPERAND_EXTERN:
			add_node_code(&(*head_code), &(*tail_code), memory_count, 1, NULL);/*add extern type*/
			add_memory_extern_arg(&(*head_ext), &(*tail_ext), destination, memory_count);
			memory_count++;
			break;

		break;
	}
	return memory_count;


}

/*Function receives source and destination, their types, details of line, head and tail of labels. Translates source and destination to binary machine code. Returns memory counter one after last*/
int translate_two_operands(char *source, char *destination, int source_type, int destination_type, int memory_count, int line_number, codeWords **head_code, codeWords **tail_code, ext **head_ext, ext **tail_ext){
	int num_d, num;
	int code = 0;
	int unknown = -1;

	switch(source_type){
			case OPERAND_NUMBER:
				num = atoi(++source);
				code = (short)num<<2;
				add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);
				memory_count++;
				break;
			case OPERAND_LABEL:
				add_node_code(&(*head_code), &(*tail_code), memory_count, unknown, source);
				memory_count++;
				break;
			case OPERAND_STRUCT:
				if(source[strlen(source)-1]=='1')/*num of struct field*/
						code = 1<<2;
				else code = 2<<2;

				add_node_code(&(*head_code), &(*tail_code), memory_count, unknown, source);/*memory count is unknown*/
				memory_count++;
				add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);/*add it's number <<2*/
				memory_count++;
				break;
			case OPERAND_REGISTER:
				num = check_operand_register(source);/*take index of register*/
				code = code|(num<<6);

				if(destination_type==OPERAND_REGISTER){/*if source and destination are registers this is the same word*/
						num_d = check_operand_register(destination);/*take index of register*/
						code = code|num_d<<2;
						add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);
						memory_count++;
						return memory_count;
				}

				add_node_code(&(*head_code), &(*tail_code), memory_count, code, NULL);
				memory_count++;
				break;

			case OPERAND_EXTERN:
				add_node_code(&(*head_code), &(*tail_code), memory_count, 1, NULL);/*add extern type*/
				add_memory_extern_arg(&(*head_ext), &(*tail_ext), source, memory_count);
				memory_count++;
				break;

			break;
		}
	memory_count = translate_one_operand(destination, destination_type, memory_count, line_number, &(*head_code), &(*tail_code), &(*head_ext), &(*tail_ext));
	return memory_count;

}

/*Function receives head and tail of binary representation linked list, memory counter, code and argument name if
 * code is still unknown. Creates new node and adds into linked list*/
void add_node_code(codeWords **head, codeWords **tail, int memory_count, int code, char *literal){
	char *ptr;
	codeWords *new = malloc(sizeof(codeWords));
	if(new==NULL)
		return;

	if(literal == NULL)
		new->literal = literal;
	else {
		new->literal = (char *)malloc(strlen(literal)+1);
		if(new->literal == NULL)
			return;
		if(strchr(literal, '.')){/*if this is struct*/
			ptr = strtok(literal, ".");
			strcpy(new->literal, ptr);
		}
		else strcpy(new->literal, literal);
	}

	new->code = code;
	new->memory_count = memory_count;
	new->next = NULL;
	if(*head==NULL){/*if this is first node*/
		*head = new;
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

/*Function receives linked list of binary machine code of instructions and entry labels,
 * adds "word* with label memory count and ARE*/
void add_address_of_labels(codeWords **head_code, labels **head_lbl){
	codeWords *ptr_code = *head_code;
	labels *ptr_label;
	int unknown = -1;
	int ARE = 2;/*ARE of label*/
	int code;
	while(ptr_code!=NULL){
		code = 0;
		ptr_label = *head_lbl;
		if(ptr_code->code==unknown){
			while(ptr_label!=NULL){
				if(!strcmp(ptr_label->label, ptr_code->literal)){
						code = code | ARE;/*setting ARE of label*/
						code = code | (ptr_label->memory_count)<<2;/*adds address of label*/
					ptr_code->code = code;
					break;
				}
				ptr_label = ptr_label->next;
				}
			}
		ptr_code = ptr_code->next;
	}
}

/*Function receives heads of directives and labels linked list and last memory count, adds memory count of labels that
 have been defined in directive instruction*/
int add_drctv_memory_count(directiveLine **head_drctv, labels** head_label, int memory_count){
	directiveLine *ptr_drctv = *head_drctv;
	labels *ptr_label;
	int line_num = 0;
	while(ptr_drctv!=NULL){
		ptr_label = *head_label;
		ptr_drctv->memory_count = memory_count;
		if(ptr_drctv->isLabel && line_num!=ptr_drctv->line_num){
			line_num = ptr_drctv->line_num;
			while(ptr_label!=NULL){
				if(ptr_drctv->line_num == ptr_label->line_number){
					ptr_label->memory_count = memory_count;
				}
				ptr_label = ptr_label->next;
			}
		}
		memory_count++;
		ptr_drctv = ptr_drctv->next;
	}
		return memory_count;
}

/*Function receives head of label's list, line number and memory counter. Assigns memory counter to the label*/
void add_label_memory_num(labels** head_label, int memory_count, int line_number){
	labels *ptr = *head_label;

	while(ptr!=NULL){
		if((ptr->line_number) == line_number){
			ptr->memory_count = memory_count;
			break;
		}
		ptr = ptr->next;
	}
}

/*Function receives name of file, counters of commands, linked lists of commands and string for base 32 presentation, translates
 * all lines and writes into the .ob file*/
void translate_and_output(char *file_name, int cmd_code_count, int drctv_code_count, codeWords **head_code, directiveLine **head_drctv, char* base_32){
	FILE *dfp;
	codeWords *ptr_code = *head_code;
	directiveLine *ptr_drctv = *head_drctv;
	char* objFileName = (char*)malloc(strlen(file_name)+4);/*allocates memory for new .obj file*/
		if(objFileName==NULL)
			return;

	sprintf(objFileName,"%s.ob", file_name);/*writes a full name of file*/

	if((dfp = fopen(objFileName, "w"))==NULL){/*cannot read/create destination file, exit*/
			printf("Cannot open %s\n", objFileName);
			free(objFileName);
			return;
	}
	base_32 = translate_to_base32((short)cmd_code_count, base_32);/*translate and write num of cmd lines*/
	fprintf(dfp, "  %s ", base_32);
	base_32 = translate_to_base32(drctv_code_count, base_32); /*translate and write num of direction lines*/
	fprintf(dfp, "%s\n", base_32);

	while(ptr_code!=NULL){
		base_32 = translate_to_base32((short)ptr_code->memory_count, base_32);
		fprintf(dfp, "%s\t", base_32);
		base_32 = translate_to_base32(ptr_code->code, base_32);
		fprintf(dfp, "%s\n", base_32);
		ptr_code = ptr_code->next;
	}
	while(ptr_drctv!=NULL){
		base_32 = translate_to_base32((short)ptr_drctv->memory_count, base_32);
		fprintf(dfp, "%s\t", base_32);
		base_32 = translate_to_base32(ptr_drctv->arg, base_32);
		fprintf(dfp, "%s\n", base_32);
		ptr_drctv = ptr_drctv->next;
	}

	free(objFileName);
	fclose(dfp);
}

/*Function receives number and string, translates number to base 32, returns string with base 32 code presentation*/
char* translate_to_base32(short int num, char *base){
	char base_32[32] = {'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v'};
	int first, second;

	first = extract_bits(num, 5,6);
	second = extract_bits(num, 5,1);

	base[0] = (char)base_32[first];
	base[1] = (char)base_32[second];
	base[2] = '\0';
	return base;
}

/*Function receives number to extract bits, number of bits to be extracted and position from right, returns number with extracted bits*/
int extract_bits(short int num, int num_bits, int position){
	int num1, num2;
	num1 = (1<<num_bits)-1;
	num2 = num>>(position-1);
	return num1&num2;
}

/*Function receives name of file, linked lists of labels and string for base 32 presentation, creates .ent file
 * if at least one label exists. Writes into file.*/
void output_entry_labels(char *file_name, labels **head_lbl, char* base){
	FILE *dfp;	
	labels *ptr_lbl, *ptr2_lbl;
	char* entFileName;
	int is_first = 1;

	if(*head_lbl==NULL)/*if no labels - exit*/
		return;

	entFileName = (char*)malloc(strlen(file_name)+5);/*allocates memory for new .ent file*/
	if(entFileName==NULL)
		return;

	sprintf(entFileName,"%s.ent", file_name);/*writes a full name of file*/

	ptr_lbl = *head_lbl;
	while(ptr_lbl!=NULL){
		if(ptr_lbl->label_type == LABEL){
			ptr2_lbl = *head_lbl;
				while(ptr2_lbl!=NULL){
					if(ptr2_lbl->label_type == ENTRY && !strcmp(ptr_lbl->label, ptr2_lbl->label)){
						if(is_first){
							if((dfp = fopen(entFileName, "w"))==NULL){/*cannot read/create destination file, exit*/
									printf("Cannot create %s\n", entFileName);
									free(entFileName);
									return;
							}
							is_first = 0;
						}
						fprintf(dfp, "%s\t", ptr_lbl->label);
						base = translate_to_base32(ptr_lbl->memory_count, base);
						fprintf(dfp, "%s\n", base);
						break;
					}
				ptr2_lbl = ptr2_lbl->next;
				}
		}
		ptr_lbl= ptr_lbl->next;
	}
	free(entFileName);
	if(!is_first)/*if file has been created*/
		fclose(dfp);
}

/*Function receives head, tail of linked list of extern labels, name of label and memory count.
 *  Adds new node of extern label into linked list*/
void add_memory_extern_arg(ext** head, ext** tail, char* name, int memory_count){
	ext *new = (ext*)malloc(sizeof(ext));
	if(new==NULL)
		return;

	new->ext_label = (char*)malloc(strlen(name)+1);
	if(new->ext_label==NULL)
		return;

	new->memory_count = memory_count;
	new->next = NULL;
	strcpy(new->ext_label, name);

	if(*head == NULL){/*if this is first node*/
		*head = new;
		return;
	}

	else if((*head)->next == NULL){/*if this is second node*/
		(*head)->next = new;
		*tail = (*head)->next;
	}
	else {
		(*tail)->next = new;
		*tail = new;
	}
}

/*Function receives name of file, linked lists of extern labels and string for base 32 presentation, creates .ext file
 * if at least one extern label has been used as argument. Writes into file.*/
void output_and_free_ext_labels(char *file_name, ext *head_extern, char *base){
	FILE *dfp;
	ext *ptr_extern;
	char* extFileName;

	if(head_extern==NULL)/*if no extern labels - exit*/
		return;

	extFileName = (char*)malloc(strlen(file_name)+5);/*allocates memory for new .ext file*/
		if(extFileName==NULL)
			return;

	sprintf(extFileName,"%s.ext", file_name);/*writes a full name of file*/
	if((dfp = fopen(extFileName, "w"))==NULL){/*cannot read/create destination file, exit*/
		printf("Cannot open %s\n", extFileName);
		free(extFileName);
		return;
	}

	while(head_extern!=NULL){
		ptr_extern = head_extern;
		head_extern = head_extern->next;
		fprintf(dfp, "%s\t", ptr_extern->ext_label);
		base = translate_to_base32((short)ptr_extern->memory_count, base);
		fprintf(dfp, "%s\n", base);

		free(ptr_extern->ext_label);
		free(ptr_extern);
	}

	free(extFileName);
	fclose(dfp);
}

