#include "main.h"
#include "preprocessor.h"
#include "translator.h"
#include "first_step.h"
#include "label_lists.h"
#include "cmd_check.h"




int main(int argc, char *argv[]){
	FILE *ifp;
	char *file_name, *copy_file_name;
	size_t len;
	labels *head_lbl = NULL,  *tail_lbl = NULL; /*list of labels*/
	externs *head_extern = NULL, *tail_extern = NULL; /*list of extern labels*/
	directiveLine *head_drctv = NULL, *tail_drctv = NULL; /*head and tail of directives list*/
	cmdLine *head_cmd = NULL, *tail_cmd = NULL; /*head and tail of instructions list*/
	codeWords *head_code = NULL, *tail_code = NULL; /*head and tail of machine code list*/

	if(argc == 1)
		return 1;
	while(--argc>0){
		len = strlen(*++argv);
		file_name = (char*)malloc(len+4);/*allocates memory for .as file*/
		if(file_name==NULL)
			return 1;

		copy_file_name = (char*)malloc(len+1);/*allocates memory*/
		if(copy_file_name==NULL)
			return 1;

		memcpy(copy_file_name, *argv, len);/*copies name*/

		sprintf(file_name,"%s.as", *argv);/*writes a full name of file*/

		if((ifp = fopen(file_name, "r")) == NULL){/*cannot open file, go to the next one*/
			free(file_name);
			free(copy_file_name);
			printf("Can't open %s\n", *argv);
		}
		else {
			preprocessor(file_name);/*preprocessor function*/
			sprintf(file_name,"%s.am", *argv);/*writes a full name of file*/

			read_cmd_line(file_name, &head_lbl, &tail_lbl, &head_drctv, &tail_drctv, &head_cmd, &tail_cmd, &head_extern, &tail_extern); /*check errors*/
			check_label_defined(&head_lbl, &head_extern, &head_cmd);
			printf("After check lbl defined\n");
			translate_lines(copy_file_name, &head_code, &tail_code, &head_cmd, &tail_cmd, &head_drctv, &head_lbl);			

			free_labels_list(&head_lbl, &tail_lbl);
			free_directive_list(&head_drctv, &tail_drctv);
			free_cmd_list(&head_cmd);
			free_ext_list(&head_extern, &tail_extern);
			free_code_list(&head_code, &tail_code);
			
			fclose(ifp);
		}
		
	}
	free(file_name);
	free(copy_file_name);
	return 0;
}


void free_directive_list(directiveLine **head_drctv, directiveLine **tail_drctv){
	directiveLine *ptr;
	while(*head_drctv!=NULL){
		ptr = *head_drctv;
		printf("%d\n", ptr->isLabel);
		*head_drctv = (*head_drctv)->next;
		free(ptr);
		if(ptr == NULL)
			printf("Freed\n");
	}
}

void free_cmd_list(cmdLine **head_cmd){
	cmdLine *ptr;
	while(*head_cmd!=NULL){
		ptr = *head_cmd;
		*head_cmd = (*head_cmd)->next;
		free(ptr->source);
		free(ptr->destination);
		free(ptr);
	}
}

void free_ext_list(externs **head_extern, externs **tail_extern){
	externs *ptr;
	while(*head_extern!=NULL){
		ptr = *head_extern;
		*head_extern = (*head_extern)->next;
		free(ptr->ext_label);
		free(ptr);
	}
}

void free_code_list(codeWords **head_code, codeWords **tail_code){
	codeWords *ptr;
	while(*head_code!=NULL){
		ptr = *head_code;
		*head_code = (*head_code)->next;
		free(ptr->literal);
		free(ptr);
	}
}

void free_labels_list(labels **head_lbl, labels **tail_lbl){
	labels *ptr;
	while(*head_lbl!=NULL){
		printf("Inside free:%s\n", (*head_lbl)->label);
		ptr = *head_lbl;
		*head_lbl = (*head_lbl)->next;
		free(ptr->label);
		free(ptr);
	}
}
