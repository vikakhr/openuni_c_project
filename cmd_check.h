

enum {LABEL_CMD, LABEL_DRCTV, CMD, DRCTV, LABEL_POSITION};



#define ERROR -1
#define LABELSIZE 30
#define CMDLENGTH 16 
#define REGLENGTH 8 



char* remove_blanks(char* word);
int checkCommas (char *word, int line_num);
int check_one_num(char *num);


int check_command_type(char *line, int line_num);
int check_label_islegal(char* label, int line_num);




