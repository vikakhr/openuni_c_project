
typedef struct CmdNames{/*linked list of opcodes*/
	char *name;
	int args;/*amount of operands opcode receives*/
}CmdNames;



int check_directive_islegal(char *word, int line_num);
int check_label_islegal(char* label, int line_num);
int check_operand_struct(char *line, int line_num, int isLabel);
int check_operand_number(char *num);
int check_operand_register(char *word);
int check_operand_errors(char *operand);
int check_cmd_name(char *word);
int check_directive_name(char *word);
int check_cmd_operands(char *word, int line_num, int isLabel, int cmd_index);
