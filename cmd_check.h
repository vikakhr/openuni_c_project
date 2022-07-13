
/*label+opcode, label+directive, opcode, directive, (.extern/.entry)+label*/
enum {LABEL_CMD, LABEL_DRCTV, CMD, DRCTV, LABEL_POSITION}; 





char* remove_blanks(char* word);
int checkCommas (char *word, int line_num);
int check_one_num(char *num);
int check_nums(char *nums, int isLabel);

int check_directive_islegal(char *word, int line_num);
int check_label_islegal(char* label, int line_num);
int check_string_islegal(char* line, int isLabel);
int check_arg_struct(char line, int line_num);


