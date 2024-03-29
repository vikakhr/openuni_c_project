/*Victoria Podolinskiy 333877256*/
/*This assembler program takes instructions and translates them into base 32 code. Although memory capacity is 256,
 * program will not check this. In addition, there is no clear information if we can use struct operand if it extern,
 * so in this program if struct is not defined inside file as .struct, program will print error if it used as operand for instruction*/

/*parse_file.c includes first step functions to read, make first checks and save linked lists of commands and directives*/
/*cmd_check.c includes fucntions that check names of commands and operands*/
/*label_lists.c includes functions to handle label lists*/
/*translator.c includes functions to translate, add memory count and output into files*/

#include "main.h"
#include "preprocessor.h"
#include "translator.h"
#include "parse_file.h"
#include "label_lists.h"


/*Function receives file names as arguments and opens them one by one, returns 1 if no errors, 0 otherwise*/
int main(int argc, char *argv[]){
	FILE *ifp;
	char *file_name_extension, *file_name;/*file name with extension and without it*/
	size_t len;/*name length*/

	if(argc == 1)/*if no files entered*/
		return 0;

	file_name = (char*)malloc(PATH_MAX+1);/*allocates memory*/
	if(file_name==NULL)
		return 0;

	file_name_extension = (char*)malloc(PATH_MAX+4);/*allocates memory for file name with extension*/
	if(file_name_extension==NULL){
		free(file_name)	;
		return 0;
	}
	while(--argc>0){/*read file one by one*/
		labels *head_lbl = NULL,  *tail_lbl = NULL; /* of labels list*/
		externs *head_extern = NULL, *tail_extern = NULL; /*of extern labels list*/
		directiveLine *head_drctv = NULL, *tail_drctv = NULL; /*of directives list*/
		cmdLine *head_cmd = NULL, *tail_cmd = NULL; /*of instructions list*/
		codeWords *head_code = NULL, *tail_code = NULL; /*of machine code list*/

		len = strlen(*++argv);/*length of name of file*/

		memcpy(file_name, *argv, len+1);/*copies name*/
		sprintf(file_name_extension,"%s.as", *argv);/*full name with extension*/

		if((ifp = fopen(file_name_extension, "r")) == NULL){/*cannot open file, go to the next one*/
			printf("Can't open %s\n", *argv);
		}
		else {
			fclose(ifp);

			if(preprocessor(file_name_extension, file_name)==ERROR)/*preprocessor function, if error - go to next file*/
				continue;

			sprintf(file_name_extension,"%s.am", *argv);/*name of file with extension after preprocessor*/

			/*first step*/
			read_cmd_line(file_name_extension, &head_lbl, &tail_lbl, &head_drctv, &tail_drctv, &head_cmd, &tail_cmd, &head_extern, &tail_extern); 

			/*second step*/
			check_label_defined(&head_lbl, &head_extern, &head_cmd);

			/*third step*/
			translate_lines(file_name, &head_code, &tail_code, &head_cmd, &tail_cmd, &head_drctv, &head_lbl);

			/*last step - free all lists*/
			free_labels_list(head_lbl);
			free_directive_list(head_drctv);
			free_cmd_list(head_cmd);
			free_ext_list(head_extern);
			free_code_list(head_code);
		}
	}
	free(file_name);
	free(file_name_extension);
	return 1;
}

/*Function receives head of directives linked list and frees all nodes*/
void free_directive_list(directiveLine *head_drctv){
	directiveLine *ptr;
	while(head_drctv!=NULL){
		ptr = head_drctv;
		head_drctv = head_drctv->next;
		free(ptr);
	}
}

/*Function receives head of commands linked list and frees all nodes*/
void free_cmd_list(cmdLine *head_cmd){
	cmdLine *ptr;
	while(head_cmd!=NULL){
		ptr = head_cmd;
		head_cmd = head_cmd->next;
		free(ptr->source);
		free(ptr->destination);
		free(ptr);
	}
}

/*Function receives head linked list of extern labels and frees all nodes*/
void free_ext_list(externs *head_extern){
	externs *ptr;
	while(head_extern!=NULL){
		ptr = head_extern;
		head_extern = head_extern->next;
		free(ptr->ext_label);
		free(ptr);
	}
}

/*Function receives head of code presentaion linked list and frees all nodes*/
void free_code_list(codeWords *head_code){
	codeWords *ptr;
	while(head_code!=NULL){
		ptr = head_code;
		head_code = head_code->next;
		free(ptr->literal);
		free(ptr);
	}
}

/*Function receives head of linked list of labels and frees all nodes*/
void free_labels_list(labels *head_lbl){
	labels *ptr;
	while(head_lbl!=NULL){
		ptr = head_lbl;
		head_lbl = head_lbl->next;
		free(ptr->label);
		free(ptr);
	}
}
