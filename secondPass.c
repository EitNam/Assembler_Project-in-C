#include "project.h"
#include "secondPass.h"


static int line_counter = 100, data_counter, line = 1; /*data counter is a counter for the data_array*/

/*In the data_array are inserted the codes of the data type instructions (".data" and ".string"),
which must not be printed as soon as they are encountered but only at the end.*/
char ** data_array, array_to_store[ARRAY_SIZE]; 

FILE *objectFile, *entryFile, *externalFile;

/*main function to handle second pass*/
void secondPass(char * fileName ,simbolPtr ptr)
{
    char new_line[MAX], temp_line[MAX], tempName[FILE_NAME];
    int i;
    type lineType;
    method case_type1, case_type2;
    data_array = malloc(DC * sizeof(char *)); 
    for(i = 0; i < DC; i++)
       data_array[i] = malloc(ARRAY_SIZE * sizeof(char));
    FILE *fp2 = fopen(fileName, "r");
	if(fp2 == NULL){
		printf("The program couldn't open the file: %s.\n",fileName);
		exit(1);
	}
    fileName[(strlen(fileName)-12)] = '\0';
    strcpy(tempName, fileName);
    objectFile = fopen(strcat(tempName, ".ob") , "w");
    strcpy(tempName, fileName);
    entryFile = fopen(strcat(tempName, ".ent") , "w");
    strcpy(tempName, fileName);
    externalFile = fopen(strcat(tempName, ".ext") , "w");
    fprintf(objectFile ,"     %d     %d\n",(IC-100),DC);
    while (fgets(new_line,MAX,fp2) != NULL) 
    {
        strcpy(temp_line, new_line);
        lineType = read_line(temp_line);
        if(lineType == data)
        	data_codification(new_line);
        if(lineType == string)
        	string_codification(new_line);
        if(lineType == entry)
		entry_symbol(current_line.words[0],ptr);
        if (lineType == command)
        {
            if(!comment_or_empty(new_line))
            {
                first_word(current_line.words[0]);
                if (numberOfArguments()!=0)
                {
                    second_word();
                    case_type1 = case_type(current_line.words[1]);
                    case_type2 = case_type(current_line.words[2]);
                    if (case_type1 == immediate)
                    immediate_word(current_line.words[1]);
                    if (case_type1 == direct)
                    direct_word(current_line.words[1],ptr);
                    if (case_type1 == third_method)
                    index_word(current_line.words[1],ptr);
                    if (case_type2 == immediate)
                    immediate_word(current_line.words[2]);
                    if (case_type2 == direct)
                    direct_word(current_line.words[2],ptr);
                    if (case_type2 == third_method)
                    index_word(current_line.words[2],ptr);
                }
            }
        }
        prepareNextLine(new_line);
    }
    /*prints all the data_array at the end*/
    i = 0;
    while (i<DC)
    {
        if (!i)
		fprintf(objectFile,"%04d %s",line_counter,data_array[i]);
        else
        fprintf(objectFile,"\n%04d %s",line_counter,data_array[i]);
        i++;
        line_counter++;
    }
    fclose(objectFile);
    fclose(entryFile);
    fclose(externalFile);
    freeSimbolList(&ptr);
    printf("Files were created sucssesfully.\n");
}
/*this function converts each character in a string to its special hex equivalent represenetation.*/
void string_to_hex(char *word)
{
    int i;
    char ch,buffer[10],zero[2] = "0"; /*buffer stores the ascii value of the character as a string*/
    for (i = 1;i<(strlen(word)-1);i++)
    {
        ch = word[i];
        int ch_as_int = (int)ch;
        sprintf(buffer,"%d",ch_as_int);
        codification_to_hex(buffer,EMPTY,ZERO,0,1,1);
        strcpy(data_array[data_counter],array_to_store);
        data_counter++;
    }
    codification_to_hex(zero,EMPTY,ZERO,0,1,1);
    strcpy(data_array[data_counter],array_to_store);
    data_counter++;
}

/*This function is able to manage the transformation of a number in the special hex base, starting from three types of representations:
1) a number represented as a string.
2) machine code of 20 bits.
3) a number represented as an integer.
The three flags have three different functions: the first flag tells the function which of the three representations has been inserted,
the second tells the function as if the word in question is absolute, relocatable or external (ARE) and 
the third tells the function if the number passed to it is derived from a data (.data or .string) instruction, and therefore should not be printed immediately.*/
void codification_to_hex(char *number,char *word,int num, int flag,int absolute,int data_flag)
{   
    int n;
    long long bin = 0, i = 1;
    int reminder1,temp;
    long int hexadecimal = 0, j = 1 , remainder2;
    char ch = 'A'; /*ch contains, one at a time, the letters needed for the special hex basis*/
    char binary[ARRAY_SIZE]; /* array containing the machine code (binary) of the decimal number that was passed as the argument of the function*/
    char hexa[20];/* array containing the hexa representation of the decimal number that was passed as the argument of the function*/
    char alpha_hexa[ARRAY_SIZE];/* array containing the special basis representation illustrated in the maman of the decimal number(or 20 bits machine code) that was passed as the argument of the function*/
    if (flag == 0)
    n = atoi(number);
    else
    n = num;
    temp = n;
    if (flag != 1)
    {
        /*This stage converts the decimal number passed to the funtion to the its 20 bits binary equivalent */
        if (n<0)
        n*=(-1);
        while (n!=0) 
        {
            reminder1 = n % 2;
            n /= 2;
            bin += reminder1 * i;
            i *= 10;
        }
        sprintf(binary, "%04d%016lld",100, bin);
        if (temp<0)
        {
        two_complement(binary);
        }
    }
    else 
    {
        strcpy(binary,word);
    }
    /*this stage converts the 20 bits binary machine code to its hex equivalent */
    for (i=19;i>3;i--)
    {
        if (binary[i]=='0')
        remainder2 = 0;
        else
        remainder2 = 1;
        hexadecimal += remainder2 * j;
        j *= 2;
    }
    sprintf(hexa,"%05lx",hexadecimal);
    i = 0,j = 0;
    /*this stage converts the hex number to the special hex basis*/
    while (j<14)
    {
        if ((j%3)==0) {
        alpha_hexa[j] = ch;
        ch+=1;
        }
        if ((j%3)==2)
        alpha_hexa[j] = '-';
        if ((j%3)==1 && j!=1){
        alpha_hexa[j] = hexa[i];
        i++;
        } 
        if (j==1){
        if (absolute==1)
        alpha_hexa[j] = '4';
        else if (absolute==2)
        alpha_hexa[j] = '2';
        else if (absolute==3)
        alpha_hexa[j] = '1';
        i++;
        }
        j++;
    }
    alpha_hexa[j] = '\0';
    if (!data_flag)
    {
        fprintf(objectFile, "%04d %s\n",line_counter,alpha_hexa);
        line_counter++;
    }
    else
    {
        strcpy(array_to_store,alpha_hexa);
    }


}
/*this function converts a binary number to its ones' complement equivalent*/
void one_complement(char binary[])
{
    int i;
    for(i = 19; i > 3; i--){
    if(binary[i] == '0')
    binary[i] = '1';
    else
    binary[i] = '0';
   }
}
/*this function converts a binary number to its two's complement equivalent*/
void two_complement(char binary[])
{
    int i,carry = 1;
    one_complement(binary);
    for(i = 19; i > 3; i--){
      if(binary[i] == '1' && carry == 1){
        binary[i] = '0';
      }
      else if(binary[i] == '0' && carry == 1){
         binary[i] = '1';
         carry = 0;
      }
   }
}
/*this function encodes the instructions of type .data and inserts them in the data_array */
void data_codification(char *new_line)
{
    char delim[] = " ";
	char *ptr = strtok(new_line,delim);
    while(ptr != NULL)
    {
        if ( isdigit(ptr[0]) ||  ptr[0] == '-'  || ptr[0] == '+')
        {
            codification_to_hex(ptr,EMPTY,ZERO,0,1,1);
            strcpy(data_array[data_counter],array_to_store);
            data_counter++;
        }
        ptr = strtok(NULL,delim);
    }
}
/*this function encodes the instructions of type .string and inserts them in the data_array */
void string_codification(char *new_line)
{
    char delim[] = " ";
	char *ptr = strtok(new_line,delim);
    while(ptr != NULL)
    {
        
        if (ptr[0] == '\"') 
        string_to_hex(ptr);
        ptr = strtok(NULL,delim);
    }
}
/*this function encodes the first word of every command*/
void first_word(char *word)
{
    int i = check_command(word);
    codification_to_hex(instruction_list[i].opcode,EMPTY,ZERO,0,1,0);
    
}
/*this function encodes the second word of every command*/
void second_word()
{ 
    int i,j,register1,register2;
    char word[ARRAY_SIZE],array_register1[ARRAY_SIZE], array_register2[ARRAY_SIZE], temp_current1[ARRAY_SIZE],temp_current2[ARRAY_SIZE],temp_lable[LABEL_LIMIT] = "", temp_reg[LABEL_LIMIT] = "";;
    method case_type1,case_type2;
    method temp_case1;
    case_type1 = case_type(current_line.words[1]);
    case_type2 = case_type(current_line.words[2]);
    strcpy(temp_current1,current_line.words[1]);
    strcpy(temp_current2,current_line.words[2]);

    /*in this stage we fill the ARE and the funct fields in the word*/
    word[0] = word[2] = word[3] = '0';
    word[1] = '1';
    j = check_command(current_line.words[0]);
    word[4]=instruction_list[j].funct[0];
    word[5]=instruction_list[j].funct[1];
    word[6]=instruction_list[j].funct[2];
    word[7]=instruction_list[j].funct[3];
    
    i = 8;
    /*if case_type2 is equal to zero, it means there is only one operand. In these cases the first
    operand is not, as usually, the source operand but the destination operand. For this reason we invert the operands */
    if (case_type2 == 0)
    {
        temp_case1 = case_type1;
        case_type2 = temp_case1;
        case_type1 = 0;
        strcpy(temp_current2,current_line.words[1]);
        strcpy(temp_current1,current_line.words[2]);
    }
    /*in this stage of the function we fill the source and destination operands fields*/
    if (case_type1 == registerDirect)
    {
       
        register1 = check_register(temp_current1);
        register_codification(register1, array_register1);
        j = 0;
        while (i<12)
            { 
                word[i++] = array_register1[j++];
            }
        i+=2;
    }
    else if (case_type1 == third_method)
    {
        check_third(temp_current1,temp_lable,temp_reg);
        register1 = check_register(temp_reg);
        register_codification(register1, array_register1);
        j = 0;
        while (i<12)
            { 
                word[i++] = array_register1[j++];
            }
        i+=2;
        
    }
    else
    {
        while (i<12)
        {
            word[i] = '0';
            i++;
        }
        i+=2;
    }
    if (case_type2 == registerDirect)
    {  
        register2 = check_register(temp_current2);
        register_codification(register2, array_register2);
        j = 0;
        while (i<18)
            { 
                word[i++] = array_register2[j++];
            }
    }
    else if (case_type2 == third_method)
    {
        check_third(temp_current2,temp_lable,temp_reg);
        register2 = check_register(temp_reg);
        register_codification(register2, array_register2);
        j = 0;
        while (i<18)
            { 
                word[i++] = array_register2[j++];
            }
    }
    else 
    {
        while (i<18)
        {
            word[i++] = '0';
            
        }
    }
    /* in this stage we fill the addressing methods fields*/
    if (case_type1 == 0)
    {
        word[12] = '0';
        word[13] ='0';
    }
    if (case_type1 == registerDirect)
    {
        word[12] = '1';
        word[13] ='1';
    }
    if (case_type2 == registerDirect)
    {
        word[18] = '1';
        word[19] ='1';
    }
    if (case_type1 == immediate  )
    {
        word[12] = '0';
        word[13] ='0';
    }
    if (case_type2 == immediate)
    {
        word[18] = '0';
        word[19] ='0';
    }
    if (case_type1 == direct )
    {
        word[12] = '0';
        word[13] ='1';
    }
    if (case_type2 == direct)
    {
        word[18] = '0';
        word[19] ='1';
    }
    if (case_type1 == third_method)
    {
        word[12] = '1';
        word[13] ='0';
    }
    if (case_type2 == third_method)
    {
        word[18] = '1';
        word[19] ='0';
    }
    word[20] = '\0';
    codification_to_hex(EMPTY,word,ZERO,1,1,0);
    
}
/*this function converts to binary a register number*/
void register_codification(int register1, char array_register1[5])
{
    long long binary_register = 0, j = 1;
    int reminder;
    while (register1 != 0)
        { 
            reminder = register1 % 2;
            register1 /= 2;
            binary_register += reminder * j;
            j *= 10;
        }
    sprintf(array_register1, "%04lld", binary_register);
}
/*this function handle the added words if the operand is of type immediate*/
void immediate_word(char *immediate)
{
    char temp_immediate[40];
    strcpy(temp_immediate,immediate+1);
    codification_to_hex(temp_immediate,"",0,0,1,0);
}   
/*this function handle the added words if the operand is of type index*/
void index_word(char *index,simbolPtr ptr)
{
    char temp_lable[LABEL_LIMIT] = "", temp_reg[LABEL_LIMIT] = "";
    check_third(index,temp_lable,temp_reg);
    direct_word(temp_lable,ptr);

}
/*this function handle the added words if the operand is of type direct*/
void direct_word(char *direct,simbolPtr ptr)
{
    simbolPtr sPtr;
    sPtr = findSimbol(ptr,direct);
    if (sPtr == NULL)
       printf("In line %d there is an error: the operand label %s of the command is not present in the symbol table.\n",line,direct);
    else if (sPtr->external == 1)
    {
        codification_to_hex(EMPTY , EMPTY,sPtr->base_address,2,3,0);
        codification_to_hex(EMPTY , EMPTY,sPtr->offset,2,3,0);
        fprintf(externalFile,"\n%s BASE %d\n%s OFFSET %d",sPtr->symbolName,(line_counter-2),sPtr->symbolName,(line_counter-1));
    }
    else
    {
        codification_to_hex(EMPTY , EMPTY,sPtr->base_address,2,2,0);
        codification_to_hex(EMPTY , EMPTY,sPtr->offset,2,2,0);
    }
    
    
}   


void entry_symbol(char *symbol,simbolPtr ptr)
{
    simbolPtr sPtr;
    sPtr = findSimbol(ptr,symbol);
    if (sPtr!= NULL)
        fprintf(entryFile, "%s,%04d,%04d\n",sPtr->symbolName,sPtr->base_address,sPtr->offset);
}




