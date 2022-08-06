#define LINESIZE 81

char* remove_blanks(char* word);
int check_commas (char *line, int line_num);
int check_typo_errors(char* command, int line_num);
int check_one_num(char *num);
int check_nums(char *nums, int isLabel, int line_num);
int check_string_islegal(char* line, int isLabel);

