

typedef struct CmdNames{/*linked list of opcodes*/
	char *name;
	int num;
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
int check_directive(char *word, struct DrctvNames *drctv);
int check_cmd_args(char *word, int line_num, int type, struct CmdNames *cmd);
int check_drctv_args(char *word, int line_num, int type, struct DrctvNames *drctv);


int check_first_group(char *source, char *dest);
int check_second_group(char *dest);
int check_cmp(char *source, char *dest);
int check_lea(char *source, char *dest);
int check_prn(char *dest);

void check_data_param();
void check_string();
void check_struct_param();
void check_entry();
void check_extern();


/*struct array of commands*/
static CmdNames cmd[] ={
	{"mov",0, 2, check_first_group},
	{"cmp",1, 2, check_cmp}, 
	{"add",2, 2, check_first_group},
	{"sub",3, 2, check_first_group},
	{"not",4, 1, check_second_group},
	{"clr",5, 1, check_second_group},
	{"lea",6, 2, check_lea},
	{"inc",7, 1, check_second_group},
	{"dec",8, 1, check_second_group},
	{"jmp",9, 1, check_second_group},
	{"bne",10, 1, check_second_group},
	{"get",11, 1, check_second_group},
	{"prn",12, 1, check_prn},
	{"jsr",13, 1, check_second_group},
	{"rts",14, 0, 0},
	{"hlt",15, 0, 0},
	{"not_valid",16, 0}
};

/*struct array of directives*/
static DrctvNames drctv[]={
	{".data",0, 40, check_data_param},
	{".string",1, 2, check_string}, 
	{".struct",2, 2, check_struct_param},
	{".entry",3, 1, check_entry},
	{".extern",4, 1, check_extern},
	{"not_valid",5, 0}
};






