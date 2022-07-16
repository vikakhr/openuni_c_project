

typedef struct CmdNames{/*linked list of opcodes*/
	char *name;
	int args;/*amount of arguments for command*/
	int (*func)();
}CmdNames; 

typedef struct DrctvNames{/*linked list of opcodes*/
	char *name;
	int num;
	int args;/*amount of arguments for command*/
	void (*func)();
}DrctvNames; 


struct node_cmd{/*linked list to save binary representation??????*/
	int cmd_index;	
	int args;
	char* source;
	char* destination;
	int line_num;
	struct node_cmd* next;
}node_cmd; 


struct node_directive{/*linked list to save binary representation??????*/
	char* drctv_line;
	int isLabel;
	int directive_type; 
	int line_num;
	struct node_directive* next;
}node_directive; 





typedef struct node_directive directiveLine;/*lines with directive*/
typedef struct node_cmd instructionLine;/*lines with instructions*/


void check_cmd_line(char *sourceFileName);
int line_typo_errors_check(char* command, int line_num);
line_instruction_check(char* command, int line_num, int isLabel, int cmd_index);




int check_cmd (char *word, struct CmdNames *cmd);
int check_directive(char *word);
int check_cmd_args(char *word, int line_num, int isLabel, int cmd_index, struct CmdNames *cmd);
int check_drctv_args(char *word, int line_num, int type, struct DrctvNames *drctv);


int check_first_group(char *source, char *dest, int line_num);
int check_second_group(char *dest);
int check_cmp(char *source, char *dest);
int check_lea(char *source, char *dest);
int check_prn(char *dest);

void print_instruction_list(instructionLine* head);

void add_instruction_node(instructionLine** head, instructionLine** tail, char* source, char* destination, int cmd_index, int line_num, int args);
void add_directive_node(directiveLine **head, directiveLine **tail, int line_num, char *line, int isLabel, int directive_type);

static char *OPCODE[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};

static char *REGISTER[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"}; 

static char *DIRECTIVE[] = {".data",".string",".struct",".entry",".extern"};


/*struct array of commands*/
static CmdNames cmd[] ={
	{"mov", 2, check_first_group},
	{"cmp", 2, check_cmp}, 
	{"add", 2, check_first_group},
	{"sub", 2, check_first_group},
	{"not", 1, check_second_group},
	{"clr", 1, check_second_group},
	{"lea", 2, check_lea},
	{"inc", 1, check_second_group},
	{"dec", 1, check_second_group},
	{"jmp", 1, check_second_group},
	{"bne", 1, check_second_group},
	{"get", 1, check_second_group},
	{"prn", 1, check_prn},
	{"jsr", 1, check_second_group},
	{"rts", 0, 0},
	{"hlt", 0, 0},
	{"not_valid",16, 0}
};






