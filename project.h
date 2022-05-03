#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define LABEL_LIMIT 31
#define MAX 400
#define FILE_NAME 50

typedef enum { immediate = 1, direct, third_method, registerDirect } method;
typedef enum { data, string, external, entry, command } type;

struct line
    {
        char lable[MAX];
        char words[4][MAX];
        char comma[MAX];
    };

struct instruction
    {
        char name[5];
        char funct[5];
        char opcode[5];
        int arguments;
	char source[10];
	char dest[10];
    };

/*Creating new structure for data simbol table linked list*/
typedef struct simbol * simbolPtr;
typedef struct simbol{
        char symbolName[LABEL_LIMIT];
        int value_decimal;
        int base_address;
        int offset;
        int entry;
        int external;
        int code;
        int data;
	int line;
	simbolPtr next;
}simbol;

extern struct line current_line;
extern struct instruction instruction_list[16];

