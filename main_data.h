

/*Header contains all data of after preprocessor excecution*/
#define MEMORY_SIZE 256
#define MAX_ERROR_NAME 150






typedef struct binaryCode{/*linked list to save binary representation??????*/
	int line_number;
	int opcode;
	int source_addressing;
	int destination_addressing;
	int A_R_E;
}binaryCode; 

typedef struct binaryCode word[MEMORY_SIZE];/*array of structs - with 'words'*/

static char *OPCODE[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};


static char *REGISTER[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"}; 






