#define LABELSIZE 30 /*max length of label name*/
#define MAX_10_BITS_NUM 511 /*max number for 10 bits signed binary representation*/
#define MIN_10_BITS_NUM -511 /*min number for 10 bits signed binary representation*/

void read_cmd_line(char *sourceFileName, labels **head_lbl, labels **tail_lbl, directiveLine **head_drctv, directiveLine **tail_drctv, cmdLine **head_cmd, cmdLine **tail_cmd, externs **head_extern, externs **tail_extern);
void check_cmd_line(char *command, int line_num, labels **head_lbl, labels **tail_lbl, directiveLine **head_drctv, directiveLine **tail_drctv, cmdLine **head_cmd, cmdLine **tail_cmd, externs **head_extern, externs **tail_extern);

void add_data_arg(char* line, int isLabel, int line_num, directiveLine **head, directiveLine **tail);
void add_string_arg(char* line, int isLabel, int line_num, directiveLine **head_drctv, directiveLine **tail_drctv);
void add_struct_arg(char* line, int isLabel, int line_num, directiveLine **head_drctv, directiveLine **tail_drctv);

void add_instruction_node(cmdLine** head, cmdLine** tail, char* source, char* destination, int cmd_index, int line_num, int args, int isLabel);
void add_directive_node(directiveLine **head, directiveLine **tail, int line_num,  int isLabel, int directive_type);
void delete_instruction_node(cmdLine **head, int line_num);












