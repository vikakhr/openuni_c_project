

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

void check_cmd_line(char *sourceFileName);

int check_cmd (char *word, struct CmdNames *cmd);
int check_directive(char *word);
int check_cmd_args(char *word, int line_num, int type, struct CmdNames *cmd);
int check_drctv_args(char *word, int line_num, int type, struct DrctvNames *drctv);


int check_first_group(char *source, char *dest);
int check_second_group(char *dest);
int check_cmp(char *source, char *dest);
int check_lea(char *source, char *dest);
int check_prn(char *dest);



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






