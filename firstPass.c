#include "project.h"
#include "firstPass.h"


static int line = 1, label_flag = 0, NumOfLines;


/*This function will get a pointer to the main simbolTable and the simbolName,
it will create a new item in the list and assign the name as a simbolName*/
simbolPtr addSimbol(simbolPtr * simbolTable, char symbolName[])
{
	simbolPtr p1, p2;

	p1= (simbolPtr) malloc(sizeof(simbol));
	if (!p1)
	{ 
		printf("\n cannot build list \n");
		exit(0);
	}
	strcpy(p1->symbolName , symbolName);
	if(*simbolTable == NULL)
		*simbolTable = p1;
	else{
		p2 = *simbolTable;
		*simbolTable = p1;
		p1->next = p2;
	}
	return p1;
	   
}


/*This function will recieve the simbol linked list and a simbol name,
and return a pointer to the item that its' name is the simbolName*/
simbolPtr findSimbol(simbolPtr simbolTable, char simbolName[])
{
	simbolPtr p1;
	p1 = simbolTable;
	while(p1 != NULL){
		if(!strcmp(p1->symbolName, simbolName))
			return p1;
		p1 = p1->next;
	} 
	return NULL;   
}


/*update the simbols of data with the IC value*/
void updateDataSimbols(simbolPtr sPtr)
{	
	int value;
	while(sPtr)
	{
		if(sPtr->data == 1)
		{
			value = sPtr->value_decimal = sPtr->value_decimal + IC;
			sPtr->base_address = value - (value%16);
			sPtr->offset = (value%16); 
		}
		sPtr=sPtr->next;
	}
}


/*This function will free the memmory allocated for the list*/
void freeSimbolList(simbolPtr * simbolTable)
{
	simbolPtr p;

	while (*simbolTable)
	{
		p=*simbolTable;
		*simbolTable = (*simbolTable)->next;
		free(p);
	}
   
}


/*main function to handle first pass*/
simbolPtr firstPass(char * fileName)
{
	char new_line[MAX], temp_line[MAX];
	int rows;
	type lineType;
	simbolPtr simbolTable = NULL, sPtr;
	FILE *fp2 = fopen(fileName, "r");
	if(fp2 == NULL){
			printf("The program couldn't open the file: %s.\n",fileName);
			exit(1);
	}
	while (fgets(new_line,MAX,fp2) != NULL) 
	{
		int skip = 0;
		strcpy(temp_line, new_line);
		lineType = read_line(temp_line);
		if(comment_or_empty(new_line))
			skip = 1;
		if(line_error(new_line)){
			errorInFile = 1;
			skip = 1;
		}
		if(current_line.lable[0] != '\0')
		{
			label_flag = 1;
			current_line.lable[strlen(current_line.lable)-1] = '\0';
		}
		if(findSimbol(simbolTable, current_line.lable) != NULL){
			printf("in line %d the lable is already exist in the simbol table.\n",line);
			errorInFile = 1;
			skip = 1;
		}
		if(!skip && label_flag && lineType == data)
		{	
			sPtr = addSimbol(&simbolTable, current_line.lable);
			rows = data_memory(new_line);
			sPtr->value_decimal = DC;
			sPtr->data = 1; 
			sPtr->line = line; 
			skip = 1;
			DC = DC + rows;
		}
		if(!skip && !label_flag && lineType == data)
		{
			DC = DC +  data_memory(new_line);
			skip = 1;
		}
		if(!skip && label_flag && lineType == string)
		{
			sPtr = addSimbol(&simbolTable, current_line.lable);
			rows = string_memory(new_line);
			sPtr->value_decimal = DC;
			sPtr->data = 1; 
			sPtr->line = line; 
			skip = 1;
			DC = DC + rows;
		}
		if(!skip && !label_flag && lineType == string)
		{
			DC = DC + string_memory(new_line);
			skip = 1;
		}
		if(!skip && lineType == external)
		{
			if(findSimbol(simbolTable, current_line.words[0]) == NULL){
				sPtr = addSimbol(&simbolTable, current_line.words[0]);
				sPtr->base_address = 0;
				sPtr->offset = 0;
				sPtr->external = 1; 
				sPtr->line = line; 
			}
			else if (!findSimbol(simbolTable, current_line.words[0])->external)
			{
				printf("In line %d the lable %s is already definde in this file as not external.\n",line,current_line.words[0]);
				errorInFile = 1;
			}
			skip = 1;
		}
		if(!skip && lineType == entry)
		{
			skip = 1;
		}
		if(!skip && lineType == command)
		{
			rows = words_number();
			if(label_flag){
				sPtr = addSimbol(&simbolTable, current_line.lable);
				sPtr->value_decimal = IC;
				sPtr->base_address = IC - (IC%16);
				sPtr->offset = (IC%16);
				sPtr->code = 1;
				sPtr->line = line; 
			}
			IC += rows;
		}
		skip = 0;
		prepareNextLine(new_line);
	}
	updateDataSimbols(simbolTable);
	return simbolTable;
}

/*clear variables before continuing to next line*/
void prepareNextLine(char * new_line)
{
	label_flag = 0;
	new_line[0] = '\0';
	line++;
	current_line.words[0][0] = '\0';
	current_line.words[1][0] = '\0';
	current_line.words[2][0] = '\0';
	current_line.words[3][0] = '\0';
	current_line.comma[0] = '\0';
	current_line.lable[0] = '\0';
}

/*check that the line is within line limit length*/
int line_error(char *new_line)
{
	int i = 0,counter = 0;
	char ch;
	while ((ch = new_line[i]) != '\0')
	{
		if(!isspace(ch))
		counter++;
		i++;
	}
	if(counter>LINE_LIMIT){
		printf("In line %d there are more than %d characters.\n",line,LINE_LIMIT);
		return 1;
	}
	return 0;
}


/* checks if the new_line is all blanks */
int all_blanks (char *new_line)
{
	int i = 0;
	char c;
	while ((c = new_line[i++]) != '\0'){
		if(!isspace(c))
			return 0;
	}
	return 1;
}


/* checks if the line is a comment or composed of blanks only*/
int comment_or_empty (char *new_line)
{
	if ((new_line[0] == ';') || (all_blanks(new_line)))
		return 1;
	return 0;
}


/*this function will update current_line and return the line type*/
type read_line(char * new_line)
{
	int i = 0;
	type returnVal = command;
	char delim[] = " ";
	char *ptr = strtok(new_line,delim);
	while(ptr != NULL)
	{
		if(i==0 && label_errors(ptr,1,0)) 
			strcpy(current_line.lable, ptr);
		else if(i==0 && !strcmp(ptr, ".data"))
			returnVal = data;
		else if (i==0 && !strcmp(ptr, ".string"))
			returnVal = string;
		else if (i==0 && !strcmp(ptr, ".extern"))
			returnVal = external;
		else if (i==0 && !strcmp(ptr, ".entry"))
			returnVal = entry;
		else if (i==2 && current_line.comma[0] == '\0') 
			strcpy(current_line.comma, ptr);
		else if (current_line.words[3][0] == '\0')
			strcpy(current_line.words[i++], ptr);
		ptr = strtok(NULL,delim);
	}
	return returnVal;
}


/*this function returns the numbers of words (lines) for each command. or -1 if the command isn't define*/
int words_number()
{
	int i, arguments = -1;
	method caseType1=0, caseType2=0;
	NumOfLines = 1;
	i = check_command(current_line.words[0]);
	if(i != -1)
		arguments = instruction_list[i].arguments;
	/*The first word isn't a command*/
	else
	{	/*check if the second word is a command*/
		if(current_line.lable[0]=='\0' && (check_command(current_line.words[1]) != -1))
		{/*The second word was a command, print the errors in the first word which is 			the label*/
			label_errors(current_line.words[0], 1, 1);
			errorInFile = 1;
			return -1;
		}
		else
		{
			printf("In line %d the command is not defined correctly.\n",line);
			errorInFile = 1;
			return -1;
		}
	}
	if(arguments > numberOfArguments()){
		printf("In line %d not enough arguments were defined for the command.\n",line);
		errorInFile = 1;
		return -1;
	}
	if(arguments < numberOfArguments()){
		printf("In line %d too many arguments were defined for the command.\n",line);
		errorInFile = 1;
		return -1;
	}
	if(arguments == 2){
		if(current_line.comma[0] == '\0'){
			printf("In line %d missing a comma between the operands.\n",line);
			errorInFile = 1;
			return -1;
		}
		else{
			NumOfLines = 2;
			caseType1 = case_type(current_line.words[1]);
			caseType2 = case_type(current_line.words[2]);
		}
	}
	if(arguments == 1){
		NumOfLines = 2;
		caseType1 = case_type(current_line.words[1]);
	}
	check_case_types(caseType1, caseType2, i);
	return NumOfLines;
}


/*return the number of operands in the line*/
int numberOfArguments()
{
	int counter = 0;
	if(current_line.words[1][0] != '\0' && current_line.words[1][0] != '\n')
		counter ++;
	if(current_line.words[2][0] != '\0' && current_line.words[2][0] != '\n')
		counter ++;
	if(current_line.words[3][0] != '\0' && current_line.words[3][0] != '\n')
		counter ++;
	return counter;
}


/*return the case type of the operand*/
method case_type(char *word)
{
	int i;
	char temp_lable[LABEL_LIMIT] = "", temp_reg[LABEL_LIMIT] = "";
	if(word[0] == '#'){
		if(word[1] != '-' && word[1] != '+' && !isdigit(word[1])){
			printf("In line %d the number arrgument is not defined correctly.\n",line);
			errorInFile = 1;
			return -1;
		}
		i = 2;
		while(word[i] != '\0'){
			if(!isdigit(word[i])){
				printf("In line %d the number arrgument is not defined correctly.\n",line);
				errorInFile = 1;
				return -1;
			}
			i++;
		}
		NumOfLines = NumOfLines + 1;
		return immediate;
	}
	if(check_register(word) != -1){
		return registerDirect;
	}
	if(check_third(word, temp_lable, temp_reg)){
		NumOfLines = NumOfLines + 2;
		return third_method;
	}
	if(label_errors(word, 0, 0)){
		NumOfLines = NumOfLines + 2;
		return direct;
	}
	return 0;
}


/*checks that the operands are right for the command*/
void check_case_types(method caseType1, method caseType2, int i)
{
	char charValue;
	if(caseType1)
	{
		charValue = caseType1+'0';
		if(strchr(instruction_list[i].source, charValue) == NULL)
		{
			printf("In line %d the operand %s is from an incorrect method for this command.\n",line, current_line.words[1]);
				errorInFile = 1;
		}
	}
	if(caseType2)
	{
		charValue = caseType2+'0';
		if(strchr(instruction_list[i].dest, charValue) == NULL)
		{
			printf("In line %d the operand %s is from an incorrect method for this command.\n",line, current_line.words[2]);
				errorInFile = 1;
		}
	}
}


/*this function will return the number of register or -1 if it's not a register*/
int check_register(char * word)
{
	int regNum;	
	char temp_num[3];
	if(word[0] != 'r')
		return -1;
	if(strlen(word) == 3){
		if(isdigit(word[1]) && isdigit(word[2])){
			temp_num[0] = word[1];
			temp_num[1] = word[2];
			temp_num[2] = '\0';
		}
	}
	else if(strlen(word) == 2){
		if(isdigit(word[1])){
			temp_num[0] = word[1];
			temp_num[1] = '\0';
		}
	}
	else
		return -1;
	regNum = atoi(temp_num);
	if(regNum >= 0 && regNum <= 15)
		return regNum;
	return -1; 	
}


/*returns the index of the command in the array or -1 if not found*/
int check_command(char * word)
{
	int i;
	for(i=0; i<INTRUCTIONS_NUMBER; i++)
		if(((strcmp(word,instruction_list[i].name))==0))
			return i;
	return -1;
}


/*checks if the method is index*/
int check_third(char * word, char * temp_lable, char * temp_reg)
{
	int i=0; 
	while(word[i] != '\0'){
		if(word[i] != '['){
			strncat(temp_lable, word+i, 1);
			i++;	
		}
		else{
			i++;
			while(word[i] != ']'){
				strncat(temp_reg, word+i, 1);
				i++;
			}
			break;
		}
	}
	if(label_errors(temp_lable, 0, 0) && (check_register(temp_reg) != -1))
		return 1;
	return 0;
}


/* handles labels' errors when defined at the beginning of a line*/
int label_errors(char *word, int needs_colon, int print)
{
	int i, returnVal = 1;
	char temp[100], message[200];
	strcpy(temp,word);
	if (isalpha(word[0])==0)
	{
		strcpy(message, "the label is not defined correctly. The label begins with a non-alphabetic character"); 
		returnVal = 0;   
	}
	if (needs_colon && word[strlen(word)-1]!=':')
	{
		strcpy(message, "the label is not defined correctly. Missing colon (:) at the end of the definition"); 
		returnVal = 0; 
	}
	if(needs_colon)
		remove_colon(temp);
	for(i=0;i<strlen(temp);i++)
	{
		if (isalnum(temp[i])==0)
		{
			strcpy(message, "the label is not defined correctly.There is a non-alphanumeric character"); 
			returnVal = 0; 
		}
	}
	if (strlen(temp)> LABEL_LIMIT)
	{
		strcpy(message, "the label is not defined correctly. The label has more than 31 characters"); 
		returnVal = 0; 
	}
	if(check_register(word) != -1)
	{
		strcpy(message, "the label is incorrect, it is a register"); 
		returnVal = 0; 
	}
	if(print)
		printf("in line %d %s.\n",line,message);
	return returnVal;
}


/* removes colon (:) at the end of the word passed as argument*/
void remove_colon(char *word)
{
    word[strlen(word)-1]='\0';
}


/* advances the data counter according to the string */
int string_memory(char *new_line)
{    
	int i = 0, counter = 0, started = 0;
	char c;
	while ((c = new_line[i++])!='\0'){
		if(c == '"'){
			if(started)
				started = 2;
			else
				started = 1;
		}
		if(started == 1)
			counter++;
	}
	if(started != 2)
	{
		errorInFile = 1;
		printf("In line %d the string isn't correct.\n",line);
		return 0;
	}
	return counter; 
}


/* advances the data counter according to the array*/
int data_memory(char *new_line)
{
	int i = 0;
	int counter = 0;
	char c;
	if(!check_data(new_line))
		return 0;
	while ((c = new_line[i++]) != '\n')
	{
		if (c==',')
			counter++;
	}
	counter++;
	return counter;
}


int check_data(char * new_line)
{
	char delim[] = " ";
	int needComma = 0, needNumber = 0, started = 0;
	char *ptr = strtok(new_line,delim);
	while(ptr != NULL && strcmp(ptr, "\n"))
	{	
		if(started)
		{
			if(needComma)
			{
				if(strcmp(ptr, ","))
				{
					printf("In line %d missing a comma between numbers.\n",line);
					errorInFile = 1;
					return 0;
				}
				needComma = 0;
				needNumber = 1;
			}
			else if(needNumber)
			{
				if(!isNumber(ptr))
				{
					printf("In line %d the number %s is not define correctly\n",line,ptr);
					errorInFile = 1;
					return 0;
				}
				needComma = 1;
				needNumber = 0;
			}
		}
		if(!strcmp(ptr, ".data") && !started)
		{
			started = 1;
			needNumber = 1;
		}	
		ptr = strtok(NULL,delim);
	}
	if(needNumber)
	{
		printf("In line %d there's a comma at the end after the numbers\n",line);
		errorInFile = 1;
		return 0;
	}
	return 1;
}


/*checks if a string is a valid number*/
int isNumber(char s[])
{
	int i;
	for (i = 0; s[i]!= '\0'; i++)
	{
		if (!isdigit(s[i]) && (i == 0 && s[i] != '-' && s[i] != '+' ))
		      return 0;
	}
	return 1;
}

