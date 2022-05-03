
#define ARRAY_SIZE 40
#define MAX_DATA 4
#define EMPTY "" 
#define ZERO 0

extern int IC, DC;

/*function for simbol list*/
simbolPtr findSimbol(simbolPtr simbolTable, char simbolName[]);
void freeSimbolList(simbolPtr * simbolTable);
void printSimbolTable(simbolPtr h);

/*function declaration*/
void string_to_hex(char *word);
void codification_to_hex(char *number,char *word,int num,int flag,int absolute,int data_flag);
void one_complement(char binary[]);
void two_complement(char binary[]);
void data_codification(char *new_line);
void string_codification(char *new_line);
void first_word(char *word);
void second_word();
void register_codification(int register1, char array_register1[5]);
void immediate_word(char * immediate);
void direct_word(char * direct,simbolPtr ptr);
void index_word(char *index,simbolPtr ptr);
void entry_symbol(char *symbol,simbolPtr);


/*function from firstPass used in this file*/
void prepareNextLine(char * new_line);
int comment_or_empty (char *new_line);
type read_line(char * new_line);
int numberOfArguments();
method case_type(char *word);
int check_register(char * word);
int check_command(char * word);
int check_third(char * word, char * temp_lable, char * temp_reg);

