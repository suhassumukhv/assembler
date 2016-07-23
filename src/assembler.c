/**
 * 		This file is part of assembler.
 * 
 *     	assembler is free software: you can redistribute it and/or modify
 *     	it under the terms of the GNU General Public License as published by
 *     	the Free Software Foundation, either version 3 of the License, or
 *     	(at your option) any later version.
 * 
 *     	assembler is distributed in the hope that it will be useful,
 *     	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     	GNU General Public License for more details.
 * 
 *     	You should have received a copy of the GNU General Public License
 *     	along with assembler. If not, see <http://www.gnu.org/licenses/>.
 *		
 * 		Release Since: 1.0
 *		Release Version: 1.1
 *		Release Date: May 2015
 *		Author: Suhas Sumukh
 *		2015. NOT COPYRIGHT PROTECTED. FREE DISTRIBUTION, IN WHATSOEVER FORM
 *		Look at the note above for additional details.
 *
 */ 

/**
 * assembler is a Assembly Compiler
 * It compiles Assembly Language into Machine Language executable on a
 * hypothetical machine.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "config.h"


/**
 * assembler uses two passes to compile
 * Creates a Symbol Table in the first pass
 * Uses Symbol Table to compile assembly code
 * into machine code in the second pass.
 * machine code produced in binary 
 * converted into hex on user request 
 *
 */ 
 
 
/* Struct for Symbol Table */
struct symbolRecord
{
	char symbol[50];
	int address;	
	int value;	
};

/* Struct for opcode table */ 
struct opcodeRecord
{
	char *mnemonic;
	char *opcode;
}opcodeTable[11];
 
/**
 * Function for help option
 *
 * @author Suhas Sumukh
 * @since 1.0
 * @modified 4/11/15 Suhas Sumukh : added versatility
 */
void print_helpbox(void)
{
	printf("%s is a program that converts assembly level code into object code in hex(by default)\nfor a hypothetical computer architecture.\n", PROGRAM_NAME);
	printf("Converts .a.txt assembly code into .o.txt machine code\n\n");
	printf("Usage: %s [option] file.\n", PROGRAM_NAME);
	printf("Options supported:\n");
	printf("\t--help \t\tDisplay this information.\n");
	printf("\t-h \t\tDisplay this information.\n");
	printf("\t-b \t\tcompile into machine binary.\n");
	printf("\t-x \t\tcompile into machine hexadexcimal.\n");
	printf("\t--version \tDisplay version information.");
}

/**
 * Function to print version information of the compiler
 *
 * @since 1.1
 */
void printVersion()
{
	printf("%s %d.%d\n", PROGRAM_NAME, PROGRAM_VERSION_MAJOR, PROGRAM_VERSION_MINOR);
	printf("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n");
	printf("This software is neither COPYRIGHT protected nor has any WARRANTY.\n");
	printf("This is free software. \nFree as in free speech. Free as in free beer.\n");
	printf("This software is a part of OPEN SOURCE DEVELOPMENT PROJECT.\n");
	printf("\nWritten by %s.\n", AUTHOR);
}

/**
 * function to get option number
 *
 * @param arg name of option
 * @return option number
 * @since 1.0
 */
int getOption(char* arg)
{
		if(!strcmp(arg,"--help"))
			return 1;		
		else if(!strcmp(arg,"-h"))
			return 2;		
		else if(!strcmp(arg,"-b"))
			return 3;		
		else if(!strcmp(arg,"-x"))
			return 4;		
		else if(!strcmp(arg,"--version"))
			return 5;
		else
			return 0;
}

/**
 * Function to fill op codes
 *
 * @author Suhas Sumukh
 * @since 1.0 
 */
void loadOpcodeTable()
{
	opcodeTable[0].mnemonic = "NEG"; opcodeTable[0].opcode = "000000"; 
	opcodeTable[1].mnemonic = "AND"; opcodeTable[1].opcode = "000001";
	opcodeTable[2].mnemonic = "XOR"; opcodeTable[2].opcode = "000010";
	opcodeTable[3].mnemonic = "OR"; opcodeTable[3].opcode = "000011";
	opcodeTable[4].mnemonic = "LSR"; opcodeTable[4].opcode = "000100";
	opcodeTable[5].mnemonic = "LSL"; opcodeTable[5].opcode = "000101";	
	opcodeTable[6].mnemonic = "MOV"; opcodeTable[6].opcode = "001101";	
	opcodeTable[7].mnemonic = "ADD"; opcodeTable[7].opcode = "010000";
	opcodeTable[8].mnemonic = "SUB"; opcodeTable[8].opcode = "010001";
	opcodeTable[9].mnemonic = "HLT"; opcodeTable[9].opcode = "001010";
	opcodeTable[10].mnemonic = "JMP"; opcodeTable[10].opcode = "001111";
} 

/**
 * Function to fill Symbol Table
 * check for symbols, values, addresses
 *
 * @param fp          pointer to file being compiled
 * @param symbolTable stores all symbols, values, addresses
 * @return size of symbol table
 * @modified 4/19/15 Suhas Sumukh : patch to remove erroneous symbol duplicates
 */
int fillSymbolTable(FILE *fp,struct symbolRecord symbolTable[],int size)
{
	/*
	 * To declare: (program counter),(value),(symbol)
	 * Take a (@code word)
	 * Compare with "DC" and "DS"
	 * If true, Go back get value and address and Symbol
	 * Now write symbolTable with (@code value),(@code address),(@code symbol)
	 * Encounter "JMP", then increment (@code address) by 2 and move on 
	 * For others, find where operands are immediate values or not
	 */	 
	unsigned int programCounter = 0; 
	char ch;
	char word[100];
	char str[100];
	int symbolTableCounter = 0;		/* How many records are there in (@code symbolTable) */
	int status;
	
	while((ch=fgetc(fp)) != EOF)
	{
		fseek(fp,-1,SEEK_CUR);
		if(fscanf(fp,"%s",word) == EOF)
			break;
		if(!strcmp(word,"DS") || !strcmp(word,"DC"))
		{			
			strcpy(symbolTable[symbolTableCounter].symbol,str);
			fscanf(fp,"%x",&(symbolTable[symbolTableCounter].value));		
			(symbolTable + symbolTableCounter)->address =  programCounter;	
			
			if(!strcmp(word,"DS"))
			{			
				programCounter = programCounter + (2 * symbolTable[symbolTableCounter].value);
			}
			else
			{
				programCounter = programCounter + 2;
			}			
			++symbolTableCounter;		
		}
		else if(!strcmp(word,"JMP"))
		{			
			programCounter = programCounter + 4;			
		}
		else if(!strcmp(word,"HLT"))
		{			
			programCounter = programCounter + 2;
		}
		else if(word[strlen(word)-1] == ':')
		{
			word[strlen(word)-1]='\0';
			strcpy(symbolTable[symbolTableCounter].symbol,word);			
			(symbolTable + symbolTableCounter)->address =  programCounter;			
			(symbolTable + symbolTableCounter)->value = -1;
			++symbolTableCounter;
		}	
		else
		{
			status = false;
			strcpy(str,word);		/* Useful in case of DC and DS. Stores previous word. */
			int i;
			for(i=0;i<9;i++)
			{
				if(!strcmp(word,opcodeTable[i].mnemonic))
				{
					status = true;
					break;
				}				
			}
			
			if(status)
			{			
				fscanf(fp,"%s",str);
				fscanf(fp,"%s",str);
				
				/* 
				 * +4 for immediate values(#25,symbol,&symbol) and +2 for non-immediate values(R0-R7,*R0-*R7)
				 * checking for non-immediate values is easier
				 * check - In the string str, 
				 * <ul>
				 * <li>its length should be 2 or 3
				 * <li>find first occurrence of R
				 * <li>find next character - should be 0-7
				 * </ul>
				 */				
				char *s = strchr(str,'R');
				if(((strlen(str)==2) || (strlen(str)==3)) && ((s!=NULL) && (s[1]>='0') && (s[1]<='7')))
				{
					programCounter = programCounter + 2;
				}
				else
				{
					programCounter = programCounter + 4;
				}
			}
		}
		
		if(symbolTableCounter>=size)
		{
			fprintf(stderr,"Variable Overload\nCompilation Terminted\n");
			exit(1);
		}			
	}	
	return symbolTableCounter;
}

/**
 * Function to write a line into object file
 * object file can be in bin or hex
 * 
 */
void Writer(char fileName[],char line[])
{	
	FILE *fp = fopen(fileName,"a+");
	fprintf(fp,"%s\n",line);
	fclose(fp);
}

/**
 * Function to convert 16-bit bin present in string to hex
 */
char* convertBinToHex(char line[])
{
	char* hexTable[16][2]={
		{"0000","0"},
		{"0001","1"},
		{"0010","2"},
		{"0011","3"},
		{"0100","4"},
		{"0101","5"},
		{"0110","6"},
		{"0111","7"},
		{"1000","8"},
		{"1001","9"},
		{"1010","A"},
		{"1011","B"},
		{"1100","C"},
		{"1101","D"},
		{"1110","E"},
		{"1111","F"}											
	};	
	int i,j,k=0;
	int counter=0;
	char bin[5];
	char hex[5];
	
	for(i=0;i<16;i++)
	{		
		if(counter<4)
		{
			bin[counter]=line[i];
			counter++;
		}
		if((counter==4)||(i==15))
		{
			counter =0;
			bin[4]='\0';
			for(j=0;j<16;j++)
			{
				if(!strcmp(hexTable[j][0],bin))
				{
					hex[k++]=*hexTable[j][1];
					break;
				}
			}
		}			
	}
	hex[k]='\0';
	strcpy(line,hex);
	return line;
}

/**
 * Function that guides the (@code Writer)
 */
void fileWrite(char fileName[],int option,char line[])
{
	if(option == 3)
		Writer(fileName,line);
	else if(option==-1 || option==4)
		Writer(fileName,convertBinToHex(line));	
}

/**
 * Function to convert hex to bin
 * generates 16-bit binary
 */
char* convertDecToBin(char line[],int value)
{
	int i,rem;
	for(i=15;i>=0;i--)
	{
		rem = value>>i;
		if(rem&1)
			line[15-i]='1';
		else
			line[15-i]='0';
	}
	line[16]='\0';
	return line;
}
 
/**
 * Function to generate object code from input by file assembly code
 *
 * @param fp          pointer to file being compiled
 * @param symbolTable contains symbols, values, addresses
 * @param option      choice entered by user
 * @param fileName    name of file into which object code is written
 * @since 1.0
 * @modified 4/16/15 Suhas Sumukh : changes to accommodate struct (@code symbolTable) read 
 * @modified 4/19/15 Suhas Sumukh : added critical patch to stop duplicates caused by read errors
 */
void generateObjectCode(FILE *fp, struct symbolRecord symbolTable[], int symbolTableSize, int option, char fileName[])
{
	/*
	 * object code can be either in binary or hexadecimal based on user's choice
	 * convert Assembly into binary line of 16-bit, and give it to (@code Writer)
     * generate 16-bit binary or hexadecimal code to (@code Writer)
	 */
	char ch;
	char word[100];
	char str[100];
	int value;
	char line[17];
	
	while((ch=fgetc(fp)) != EOF)
	{
		fseek(fp,-1,SEEK_CUR);
		if(fscanf(fp,"%s",word) == EOF)
			break;	
		if(!strcmp(word,"DS"))
		{	
			fscanf(fp,"%x",&value);	
			while(value--)
			{	
				strcpy(line,"0000""0000""0000""0000");
				fileWrite(fileName,option,line);			
			}			
		}
		else if(!strcmp(word,"DC"))
		{	
			fscanf(fp,"%x",&value);
			convertDecToBin(line,value);
			fileWrite(fileName,option,line);
		}
		else if(!strcmp(word,"HLT"))
		{	
			strcpy(line,opcodeTable[9].opcode);
			strcat(line,"0000""0000""00");
			fileWrite(fileName,option,line);
		}
		else if(!strcmp(word,"JMP"))
		{				
			strcpy(line,opcodeTable[10].opcode);
			strcat(line,"0000""0000""00");
			fileWrite(fileName,option,line);
			fscanf(fp,"%s",word);
			int i;
			for(i=0;i<symbolTableSize;i++)
			{
				if(!strcmp(word,symbolTable[i].symbol))
				{
					value = symbolTable[i].address;
					break;
				}				
			}
			convertDecToBin(line,value);
			fileWrite(fileName,option,line);
		}
		else
		{	
			int status = false;
			int i;
			for(i=0;i<9;i++)
			{	
				if(!strcmp(word,opcodeTable[i].mnemonic))
				{	
					status = true;
					break;
				}				
			}
	
			if(status)		// It is an opcode!!
			{	
				fscanf(fp,"%s",str);	
				strcpy(line,opcodeTable[i].opcode);
				char *s = strchr(str,'R');				
				value = (int)s[1] - 48;		// because ASCIIcode of 0: 48
				strcat(line,(convertDecToBin(s+1,value)+13));				
				if(str[0]=='*')
					strcat(line,"01");
				else 
					strcat(line,"00");
				fscanf(fp,"%s",str);
				
				// str can hold R4,*R4,&A,A,#123				
				// Checking all Register values
				/*
				 * Rule for operands
				 * <ul>
				 * <li><i>00</i> Register has operand
				 * <li><i>01</i> Register has pointer to operand
				 * <li><i>10</i> Immediate value
				 * </ul>
				 */
				s = strchr(str,'R');
				if(((strlen(str)==2) || (strlen(str)==3)) && ((s!=NULL) && (s[1]>='0') && (s[1]<='7')))
				{
					value = (int)s[1] - 48;		// because ASCIIcode of 0: 48
					strcat(line,convertDecToBin(s+1,value)+13);				
					if(str[0]=='*')
						strcat(line,"01");
					else 
						strcat(line,"00");
					fileWrite(fileName,option,line);
				}
				else							// immediate values
				{
					if(str[0]=='#')
					{
						sscanf(str+1,"%x",&value);
					}
					else 
					{
						if(str[0]=='&')
							s = str+1;
						else 
							s = str;
						for(i=0;i<symbolTableSize;i++)
						{
							if(!strcmp(s,symbolTable[i].symbol))
							{
								value = symbolTable[i].address;
								break;
							}
						}					
					}
					strcat(line,"00010");
					fileWrite(fileName,option,line);					
					convertDecToBin(line,value);
					fileWrite(fileName,option,line);				
				}
			}
		}		
	}
}  
 
/**
 * Main method 
 * Entry point of application
 * 
 * @since 1.0
 * @modified 4/09/15 Suhas Sumukh : added patch
 * @modified 4/09/15 Suhas Sumukh : passed pointer to struct, removed struct copy
 */ 
int main(int argc,char* argv[])
{
	int option = -1;
	FILE *fp, *fp1;
	struct symbolRecord symbolTable[MAX_SIZE];
	char fileName[MAX_LENGTH];
	
	if(argc == 1)
	{
		fprintf(stderr, "%s: no input file\nprogram terminated.\n", PROGRAM_NAME);
		exit(EXIT_FAILURE);
	}
	else if(argc > 3)
	{
		fprintf(stderr, "%s: overload.\nUse option --help for proper usage\n", PROGRAM_NAME);
		exit(EXIT_FAILURE);
	}
	else if((argc == 2) || (argc==3))
	{
		if(argv[1][0]=='-')
		{
			option = getOption(argv[1]);
		}		
	}
	
	/*
	 * Interpretation of values contained in the variable (@code option)
	 * <ul>
	 * <li>(@code option) is -1 : Option not present, default
	 * <li>(@code option) is 0 : Option present but invalid 
	 * <li>(@code option) is any positive integer: Option present and valid and numbered
	 * </ul>
	 */	 
	if(option == 1 || option == 2)
	{
		print_helpbox();
		exit(EXIT_SUCCESS);
	}
	else if(option == 5)
	{
		printVersion();
		exit(EXIT_SUCCESS);
	}
	else if(option == -1 && argc ==3)
	{
		fprintf(stderr, "%s: overload.\nUse option -h for proper usage\n", PROGRAM_NAME);
		exit(EXIT_FAILURE);
	}
	else if(option == 0)
	{
		fprintf(stderr, "%s: invalid option\n", PROGRAM_NAME);
		exit(EXIT_FAILURE);
	}	
	
	if(argc == 2)
	{
		strcpy(fileName,argv[1]);		
	}
	else
	{
		strcpy(fileName,argv[2]);
	}	
	
	loadOpcodeTable();
	if(strstr(fileName, ".a.txt") == NULL)
	{
		fprintf(stderr, "%s: file format not recognised\ncompilation terminated.\n", PROGRAM_NAME);	
		exit(EXIT_FAILURE);
	}	
	fp = fopen(fileName,"r");	
	if(fp == NULL)
	{
		fprintf(stderr, "%s: %s file not found\ncompilation terminated.\n", PROGRAM_NAME, fileName);
		exit(EXIT_FAILURE);
	}		
	
	int symbolTableSize;		
	/* Fills (@code symbolTable) */
	symbolTableSize = fillSymbolTable(fp,symbolTable, MAX_SIZE);
	
	strstr(fileName,".a.txt")[0]='\0';
	strcat(fileName,".o.txt");
	
	rewind(fp);
	
	fp1 = fopen(fileName,"w");
	fclose(fp1);
	
	generateObjectCode(fp,symbolTable,symbolTableSize,option,fileName);	
	fclose(fp);
	
	return EXIT_SUCCESS;
}