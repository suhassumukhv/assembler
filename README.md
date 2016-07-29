#assembler
###Introduction

A hypothetical, 16 bit word architecture. There are 8 general purpose registers (R0 to R7). 
The general Instruction format is: 

xxxxxx	xxxMM	xxxMM	
Opcode	Op2(dest)	Op1(source)

M indicates modes. There are following modes:	
00 => Register has operand	
01 => Register has pointer to operand	
10 => Immediate value (stored in the next 16 bits immediately following this instruction in hexadecimal).	

###Opcode table
OPCODE |  MNEMONIC	|   USAGE
--- | :---: | ---
000000 |	NEG |	NEG X, Y => X = ~Y
000001 |	AND |	AND X, Y => X = Y&X
000010 |	XOR	| XOR X, Y => X = Y^X
000011	| OR	| OR X, Y => X = Y|X
000100	| LSR 	| LSR X, Y => X = Y >> X
000101	| LSL 	| ASL X, Y => X = Y<<X
001010	| HLT 	| HLT
001101	| MOV 	| MOV X, Y => X = Y
001111	| JMP	| JMP label
010000	| ADD |	ADD X, Y => X = Y+X
010001	| SUB |	SUB X, Y => X = Y-X

###Looking at a sample
C DS 1		
MOV R1, &A	
MOV R2, &B	
MOV R3, &C	
MOV R4, *R1			
MOV R5, *R2			
JMP GREAT			
MOV *R3, R5			
HLT						
GREAT: MOV *R3, R4				
HLT			
A DC 25			
B DC 30			

The equivalent machine code generated is: 	

0000 0000 0000 0000		
0011 0100 1000 0010		
0000 0000 0001 1110		
0011 0101 0000 0010		
0000 0000 0010 0000		
0011 0101 1000 0010		
0000 0000 0000 0000		
0011 0110 0000 0101		
0011 0110 1000 1001		
0011 1100 0000 0000		
0000 0000 0001 1010		
0011 0101 1011 0100		
0010 1000 0000 0000		
0011 0101 1011 0000		
0010 1000 0000 0000		
0000 0000 0010 0101		
0000 0000 0011 0000		
	
or, in *hexadecimal*, it is:	
0000	
3482	
001E	
3502	
0020	
3582	
0000	
3605	
3689	
3C00	
001A	
35B4	
2800	
35B0	
2800	
0025	
0030	
	
**Note**: ***A DS 2*** reserves 2 words of memory*(Define Storage)*, filling them all up with zeroes.	
			***DC*** is used to *declare constants*
			
###Documentation
#####Installation
On a terminal, locate to directory having *assembler* code
```bash
$ mkdir build
$ cmake ..
$ make
```
You could check if the executable `ac` has been created.
```
$ file ./bin/ac
```
I'd suggest installing the file
```
$ make install
```
To clean up the build files
```
$ make clean
```
If you want to clean project build files.
Make sure you have installed it.
```
$ cd ..
$ rm -rf build 
```
To uninstall.
```
$ make uninstall
```
Works only if you haven't deleted the `build` directory. Else follow the steps again to create the `build` directory.
#####Usage
```
$ ac --help
```
Use `help` option to display all available options.
```
$ cat > source.a.txt << DELIM
> MOV R3, #25
> HLT
> DELIM
$ ac source.a.txt	
$ cat source.o.txt  # displays result in hexadecimal
```
Do a `man ac` to read documentation, a summary of commands and useful examples.