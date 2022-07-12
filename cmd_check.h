
/*label+opcode, label+directive, opcode, directive, (.extern/.entry)+label*/
enum {LABEL_CMD, LABEL_DRCTV, CMD, DRCTV, LABEL_POSITION}; 

enum {DATA, STRING, STRUCT, ENTRY, EXTERN};


#define ERROR -1
#define LABELSIZE 30 /*max length of label name*/
#define CMDLENGTH 16 /*number of opcodes*/
#define REGLENGTH 8 /*number of registers*/
#define DRCTVNUM 5 /*number of directives*/


char* remove_blanks(char* word);
int checkCommas (char *word, int line_num);
int check_one_num(char *num);
int check_nums(char *nums, int isLabel);

int check_directive_islegal(char *word, int line_num);
int check_label_islegal(char* label, int line_num);
int check_string_islegal(char* line, int isLabel);
int check_arg_struct(char line, int line_num);


