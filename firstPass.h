

#define LINE_LIMIT  90
#define DIRECTIVE_WORDS_NUMBER 4
#define INTRUCTIONS_NUMBER 16


extern int errorInFile, IC, DC;

/*function for simbol list that are used in this file
addSimbol: creating a new item in the list with the simbol name.
updateSimbol: adding the values to the relevant item in the list.
findSimbol: returning a pointer to an item that has name as a simbolName.
freeList: free the simbol list.*/
simbolPtr addSimbol(simbolPtr * simbolTable, char symbolName[]);
simbolPtr findSimbol(simbolPtr simbolTable, char simbolName[]);
void updateDataSimbols(simbolPtr sPtr);
void freeSimbolList(simbolPtr * simbolTable);
void printSimbolTable(simbolPtr h);

/*function declaration*/
simbolPtr firstPass(char *fp2);
void prepareNextLine(char * new_line);
int line_error(char *new_line);
int all_blanks (char *new_line);
int comment_or_empty (char *new_line);
type read_line(char * new_line);
int words_number();
int numberOfArguments();
method case_type(char *word);
void check_case_types(method caseType1, method caseType2, int i);
int check_register(char * word);
int check_command(char * word);
int check_third(char * word, char * temp_lable, char * temp_reg);
int label_errors(char *word, int needs_colon, int print);
void remove_colon(char *word);
int string_memory(char *new_line);
int data_memory(char *new_line);
int check_data(char * new_line);
int isNumber(char s[]);




