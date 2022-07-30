

typedef struct CmdNames{/*linked list of opcodes*/
	char *name;
	int args;/*amount of arguments for command*/
}CmdNames; 




void read_cmd_line(char *sourceFileName, labels **head_lbl, labels **tail_lbl, directiveLine **head_drctv, directiveLine **tail_drctv, cmdLine **head_cmd, cmdLine **tail_cmd, externs **head_extern, externs **tail_extern);
void check_cmd_line(char *command, int line_num, labels **head_lbl, labels **tail_lbl, directiveLine **head_drctv, directiveLine **tail_drctv, cmdLine **head_cmd, cmdLine **tail_cmd, externs **head_extern, externs **tail_extern);


int line_typo_errors_check(char* command, int line_num, int length);
void add_data_arg(char* line, int isLabel, int line_num, directiveLine **head, directiveLine **tail);
void add_string_arg(char* line, int isLabel, int line_num, directiveLine **head_drctv, directiveLine **tail_drctv);
void add_struct_arg(char* line, int isLabel, int line_num, directiveLine **head_drctv, directiveLine **tail_drctv);



int check_cmd (char *word);
int check_directive(char *word);
int check_cmd_args(char *word, int line_num, int isLabel, int cmd_index);


void delete_instruction_node(cmdLine **head, int line_num);
void print_instruction_list(cmdLine* head);


void add_instruction_node(cmdLine** head, cmdLine** tail, char* source, char* destination, int cmd_index, int line_num, int args, int isLabel);
void add_directive_node(directiveLine **head, directiveLine **tail, int line_num,  int isLabel, int directive_type);




/*struct array of commands*/
static CmdNames cmd[] ={
	{"mov", 2},
	{"cmp", 2}, 
	{"add", 2},
	{"sub", 2},
	{"not", 1},
	{"clr", 1},
	{"lea", 2},
	{"inc", 1},
	{"dec", 1},
	{"jmp", 1},
	{"bne", 1},
	{"get", 1},
	{"prn", 1},
	{"jsr", 1},
	{"rts", 0},
	{"hlt", 0},
	{"not_valid",-1}
};






