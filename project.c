#include "project.h"
#include "mainProject.h"

void readMacro(FILE *originalFile, char * fileName);
simbolPtr firstPass (char * fileName);
void secondPass(char * fileName ,simbolPtr ptr);


int errorInFile = 0, IC = 100, DC = 0;

struct line current_line;
struct instruction instruction_list[16]
={  {"mov","0000","1",2,"1234","234"},
    {"cmp","0000","2",2,"1234","1234"},
    {"add","1010","4",2,"1234","234"},
    {"sub","1011","4",2,"1234","234"},
    {"lea","0000","16",2,"23","234"},
    {"clr","1010","32",1,"234",""},
    {"not","1011","32",1,"234",""},
    {"inc","1100","32",1,"234",""},
    {"dec","1101","32",1,"234",""},
    {"jmp","1010","512",1,"23",""},
    {"bne","1011","512",1,"23",""},
    {"jsr","1100","512",1,"23",""},
    {"red","0000","4096",1,"234",""},
    {"prn","0000","8192",1,"1234",""},
    {"rts","0000","16384",0,"",""},
    {"stop","0000","32768",0,"",""}  };


int main(int argc, char * argv[]){
	int i = 1;
	char fileName[FILE_NAME];
	FILE *originalFile;
	simbolPtr simbolTable;
	if(argc == 1){
		printf("The program didn't recieve a text file\n");
		exit(1);
	}
	while(i < argc){
		originalFile = fopen(argv[i], "r");
		strcpy(fileName, argv[i]);
		fileName[(strlen(fileName)-4)] = '\0';
		if(originalFile == NULL){
			printf("The program couldn't open the file: %s.\n",argv[i]);
			exit(1);
		}
		readMacro(originalFile, fileName);
		simbolTable = firstPass(fileName);
		if(!errorInFile)
		{	
			secondPass(fileName, simbolTable);
		}
		else
			printf("There was an error in the file ,program didn't create the files.\n");
		i++;
	}
	return 0;
}


