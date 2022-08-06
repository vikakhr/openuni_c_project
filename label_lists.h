
enum {STRUCT, EXTERN, ENTRY, LABEL}; /*label's type number, data type includes .data and .string*/





int check_repeated_labels(char* name, labels* head);
void add_node_label(labels** head, labels** tail, char* name, int line, int label_type);
int check_label_positioning(labels** head, externs** ext_head, char* label, int label_type, int line_num);


void add_node_extern(externs** head, externs** tail, char* name);


void print_label_list(labels* head);
void print_extlabel_list(externs* head);


void check_label_defined(labels** head_label, externs **head_ext, cmdLine **head_cmd);
int check_operand_defined(labels** head_label, externs **head_ext, char* operand);
