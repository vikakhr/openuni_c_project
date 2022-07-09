
#define ERROR -1
#define LABELSIZE 30
#define CMDLENGTH 16 
#define REGLENGTH 8 


char* remove_blanks(char* word);
int checkCommas (char *word, int line_num);
int check_one_num(char *num);

int check_cmd (char *word, struct CmdNames *cmd);
int check_directive(char *word, struct DrctvNames *drctv);
int check_label_islegal(char* label, char* afterLabel, int line_num);
int check_repeated_labels(char* name, node_label* head);
int check_cmd_args(char *word, int line_num, int type, struct CmdNames *cmd);
int check_drctv_args(char *word, int line_num, int type, struct DrctvNames *drctv);
