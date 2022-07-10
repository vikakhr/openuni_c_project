

enum {LABEL_CMD, LABEL_DRCTV, CMD, DRCTV};



#define ERROR -1
#define LABELSIZE 30
#define CMDLENGTH 16 
#define REGLENGTH 8 



char* remove_blanks(char* word);
int checkCommas (char *word, int line_num);
int check_one_num(char *num);


int check_command_type(char *line, int line_num);
int check_label_islegal(char* label, char* afterLabel, int line_num);




