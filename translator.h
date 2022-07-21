enum {OPERAND_NUMBER, OPERAND_LABEL, OPERAND_STRUCT, OPERAND_REGISTER, OPERAND_EXTERN};/*addressing number according to type of operand*/

void translate_lines(cmdLine **head_cmd, cmdLine **tail_cmd, directiveLine **head_drctv, labels **head_lbl, externs *head_extern);
int first_cmd_translation(cmdLine **cmd_head, labels** lbl_head, codeWords** code_head, codeWords **tail_code, int memory_num);
int check_addressing_type(char *word, labels** lbl_head, int code);
void translate_two_operands(char *source, char *destination, int source_type, int destination_type, int memory_count);



void add_node_code(codeWords **head_code, codeWords **tail_code, int memory_count, int code);
void print_code_list(codeWords* head);/*DELETE AFTER*/
