#include "project.h"
#include "macro.h"


/*This function will get a pointer to the main macroTable and the macroName,
it will create a new itm in the list and assign the name as a key*/
void addMacro(macroPtr * macroTable, char macroName[])
{
	macroPtr p1, p2;

	p1= (macroPtr) malloc(sizeof(macro));
	if (!p1)
	{ 
		printf("\n cannot build list \n");
		exit(0);
	}
	strcpy(p1->key , macroName);
	if(*macroTable == NULL)
		*macroTable = p1;
	else{
		p2 = *macroTable;
		*macroTable = p1;
		p1->next = p2;
	}
	   
}

/*This function will get a pointer to the correct item in the list, and
add a line to its' macroLines*/
void addMacroLine(macroPtr relevantMacro, char macroLine[])
{
	strcat(relevantMacro->macroLines, macroLine);
}

/*This function will recieve the macro linked list and a macro name,
and return a pointer to the item that its' key is the macroName*/
macroPtr findMacro(macroPtr macroTable, char macroName[])
{
	macroPtr p1;
	p1 = macroTable;
	while(p1 != NULL){
		if(!strcmp(p1->key, macroName))
			return p1;
		p1 = p1->next;
	} 
	return NULL;   
}

/*This function will free the memmory allocated for the list*/
void freelist(macroPtr * macroTable)
{
  macroPtr p;

   while (*macroTable)
   {
     p=*macroTable;
     *macroTable = (*macroTable)->next;
     free(p);
   }
   
}


/*function decleration*/
/*The main function that will get the original file and create the extended one*/
void readMacro(FILE *originalFile, char * fileName);
/*This function will get the file and read a line from it. it returns the state of this
line to the function 'readMacro'*/
state readLine(FILE *f, char * line, char* firstWord);

void readMacro(FILE *originalFile, char * fileName){
	char firstWord[MAX_LINE_LENGTH], line[MAX_LINE_LENGTH];
	macroPtr macroTable = NULL, mPointer;
	int inMacro = 0;
	state lineState;
	FILE *extendedlFile = fopen(strcat(fileName, "Extended.txt") , "w");
	while((lineState = readLine(originalFile, line, firstWord)) != fileEnd){
		switch (lineState){
			case fileEnd:{
				break;
			}
			case macroStart:{
				inMacro = 1;
				addMacro(&macroTable, line);
				mPointer = macroTable;
				break;
			}
			case macroEnd:{
				inMacro = 0;
				break;
			}
			case regularLine:{
				if(inMacro)
					addMacroLine(mPointer, line);
				else{
					if((mPointer = findMacro(macroTable, firstWord)) != NULL)
						fputs(mPointer->macroLines, extendedlFile);
					else
						fputs(line, extendedlFile);
				}
			}

		}
	}
	freelist(&macroTable);
	fclose(originalFile);
	fclose(extendedlFile);
}

state readLine(FILE *f, char * line, char * firstWord){
	/* variables explenation:
	c: to get char by char from the file
	i: an index to insert chars to "line"
	token: to split the line to saperate words
	*/
	int c, i = 0, notWhite = 0;
	char* token;
	char lineForToken[MAX_LINE_LENGTH];
	while ((c = fgetc(f)) != '\n'){
		if(c == EOF)
			return fileEnd;
		if(c != '\t' && c != ' ' && c != ','){
			line[i++] = c;
			notWhite = 1;
		}
		else if(c == ' ' && notWhite){
			line[i++] = c;
			notWhite = 0;
		}
		else if(c == ','){
			line[i++] = ' ';
			line[i++] = ',';
			line[i++] = ' ';
			notWhite = 0;
		}
	}
	line[i++] = ' ';
	line[i++] = '\n';
	line[i] = '\0';
	strcpy(lineForToken, line);
	token = strtok(lineForToken, " ");
	if(!strcmp(token, "macro")){
		token = strtok(NULL, " ");
		strcpy(line, token);
		return macroStart;
	}
	if(!strcmp(token, "endm"))
		return macroEnd;
	strcpy(firstWord, token);
	return regularLine;
}


