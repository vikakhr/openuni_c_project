enum {OPERAND_NUMBER, OPERAND_LABEL, OPERAND_STRUCT, OPERAND_REGISTER, OPERAND_EXTERN};/*addressing number according to type of operand*/

#define STARTMEMORY 100/*starting counter for memory*/
#define MAXMEMORYSIZE 256

typedef struct ext{/*linked list of labels*/
	char* ext_label;
	int memory_count;/*is used in the translation function*/
	struct node_label* next;
}ext;
typedef struct ext ext_memory;/*linked list to save extern labels*/


void translate_lines(char *file_name, codeWords **head_code, codeWords **tail_code, cmdLine **head_cmd, cmdLine **tail_cmd, directiveLine **head_drctv, labels **head_lbl, externs **head_extern, externs **tail_extern);
int first_cmd_translation(cmdLine **cmd_head, labels** lbl_head, codeWords** code_head, codeWords **tail_code, externs **head_ext, externs **tail_ext, int memory_num);
int check_addressing_type(char *word, labels** lbl_head, int code);
int translate_one_operand(char *destination, int destination_type, int memory_count, int line_number, codeWords **head_code, codeWords **tail_code, externs **head_ext, externs **tail_ext);
int translate_two_operands(char *source, char *destination, int source_type, int destination_type, int memory_count, int line_number, codeWords **head_code, codeWords **tail_code, externs **head_ext, externs **tail_ext);



void add_node_code(codeWords **head_code, codeWords **tail_code, int memory_count, int code, char *literal);
void print_code_list(codeWords* head);/*DELETE AFTER*/
void add_address_of_labels(codeWords **head_code, labels **head_lbl);
int add_drctv_memory_count(directiveLine **head_drctv, int memory_count);
void add_label_memory_num(labels** head_label, int memory_count, int line_number);
char* tanslate_to_base32(short int num);
void translate_and_output(char *file_name, int cmd_code_count, int drctv_code_count, codeWords **head_code, directiveLine **head_drctv);
void output_entry_labels(char *file_name, labels **head_lbl);
void output_extern_labels(char *file_name, externs **head_extern);
void add_memory_extern_arg(externs** head, externs** tail, char* name, int memory_count);
