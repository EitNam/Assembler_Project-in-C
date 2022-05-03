
#define MAX_LINE_LENGTH 82
#define MAX_MACRO_ROWS 6
#define MACRO_NAME_LENGTH 30


/*Creating new structure for macro linked list*/
typedef struct macro * macroPtr;
typedef struct macro{
	char key[MACRO_NAME_LENGTH];
	char macroLines[MAX_MACRO_ROWS*MAX_LINE_LENGTH];
	macroPtr next;
}macro;

typedef enum { macroStart, macroEnd, regularLine, fileEnd } state; 


/*function for macro list that are used in this file
addMacro: creating a new item in the list with the macro name.
addMacroLine: adding the line to the relevant item in the list.
findMacro: returning a pointer to an item that has macroName as a key.
freeList: free the macro list.*/
void addMacro(macroPtr * macroTable, char macroName[]);
void addMacroLine(macroPtr relevantMacro, char macroLine[]);
macroPtr findMacro(macroPtr macroTable, char macroName[]);
void freelist(macroPtr * macroTable);
