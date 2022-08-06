
typedef struct CmdNames{/*linked list of opcodes*/
	char *name;
	int args;/*amount of operands opcode receives*/
}CmdNames;



int check_directive_islegal(char *word, int line_num);
int check_label_islegal(char* label, int line_num);
int check_struct_arg(char *line, int line_num, int isLabel);
int check_arg_number(char *num);
int check_arg_register(char *word);
int check_arg_errors(char *operand);
int check_cmd_name(char *word);
int check_directive_name(char *word);
int check_cmd_args(char *word, int line_num, int isLabel, int cmd_index);
