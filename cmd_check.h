




char* remove_blanks(char* word);
int check_commas (char *word, int line_num);
int check_one_num(char *num);
int check_nums(char *nums, int isLabel);

int check_directive_islegal(char *word, int line_num);
int check_label_islegal(char* label, int line_num);
int check_string_islegal(char* line, int isLabel);
int check_struct_arg(char *line, int line_num, int isLabel);

int check_arg_number(char *num);
int check_arg_register(char *word);
