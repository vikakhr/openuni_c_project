

typedef struct CmdNames{/*linked list of opcodes*/
	char *name;
	int args;/*amount of arguments for command*/
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






int check_cmd (char *word, struct CmdNames *cmd);
int check_directive(char *word);
int check_cmd_args(char *word, int line_num, int isLabel, int cmd_index, struct CmdNames *cmd);
int check_drctv_args(char *word, int line_num, int type, struct DrctvNames *drctv);



void print_instruction_list(instructionLine* head);

void add_instruction_node(instructionLine** head, instructionLine** tail, char* source, char* destination, int cmd_index, int line_num, int args);
void add_directive_node(directiveLine **head, directiveLine **tail, int line_num, char *line, int isLabel, int directive_type);

static char *OPCODE[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};

static char *REGISTER[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"}; 

static char *DIRECTIVE[] = {".data",".string",".struct",".entry",".extern"};


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






