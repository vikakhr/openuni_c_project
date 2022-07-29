

typedef struct node_m{
	char* name;
	char* data;
	struct node_m* next;
}node_m;

typedef struct node_m node_macro;


int preprocessor(char *file_name_extension, char *file_name);
void add_node_macro(node_macro** head, node_macro** tail, char* data);
void put_macro_data(char* string, node_macro** head, node_macro** tail);
int is_one_word(char* string);
int if_is_macro(char* string);
int if_is_macro(char* string);
char* take_macro_name(char* string);
int write_macro_data(char* string, node_macro* head, FILE *dfp);
void free_list(node_macro* head);


