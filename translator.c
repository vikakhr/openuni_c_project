#include "main.h"
#include "translator.h"



/*Function receives linked list of command lines, labels, decimal machine code and memory counter, makes first translations of instruction command to machine code and adds into linked list, counting memory*/
int first_cmd_translation(cmdLine **cmd_head, labels** lbl_head, codeWords** code_head, int memory_num){
	char *lbl_ptr = *lbl_head;/*label lists*/
	char *cmd_ptr = *cmd_head;/*instructions list*/
	char *code_ptr = *code_head;
	short int code = 0;
	while(cmd_ptr!=NULL){
		code = code&&(cmd_ptr->cmd_index<<6);
		if(!cmd_ptr->args){
			memory_num++;
			/*put into linkes list*/
			break;
		}
		if(cmd_ptr->args==1){
			/*check destination type*/
			/*<<2 if num or register*/
			/*if label ?*/}
		if(cmd_ptr->args==2){
			/*check source type*/
			/*check destination type*/

		}




}



	return memory_num;
}



short int translate_cmd(int cmd_index){
	short int code = 0;
	



}
