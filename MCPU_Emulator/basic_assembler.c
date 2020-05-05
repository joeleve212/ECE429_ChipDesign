/* Monstrous Third pass assembler
 * By James A. Douglas, Noisiest of Channels
 * and Jacob Leazott, Just A 0 and 1 In An Undefined Number System
 * 5/4/2020
 *
 * Reads in the first argument as a file,
 * then scans instructions according to the following rules:
 *
 *    1. Instruction name must be all caps
 *    2. Single-line comments must be preceded by a semicolon
 *    3. Commas or whitespace can separate instructions
 *    4. Address should be '#' followed by one of four forms
 *       a. decimal number
 *       b. hex number leading with 0x
 *       c. binary number leading with 0b
 *       d. * followed (optionally) by + or - and offset as hex or decimal
 *    
 * Outputs to second argument as a file (overwrites anything already there).
 * 
 * Third argument determines output format
 *
 *		Default Case : 0x00
 *		1 : 00 (hex)
 *		2 : 0b00000000\n
 *		3 : 00000000 (binary)
 *		4 : 0000 0000 (binary)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INSTRUCTIONS 64

// Functions
FILE* remove_comments(const char* fin, const char* fout);
uint8_t interpret_instr(char* instr);
uint8_t interpret_addr(char* addr, int i);
void write_output(FILE* fout, uint8_t instructions[64], int format_hex);
char* hex_to_bin(int hex);


int main(int argc, char* argv[])
{	
   uint8_t instructions[MAX_INSTRUCTIONS] = {0};
   FILE* input;
   int output_format;
   
   if(argc<3) exit(EXIT_FAILURE); //gotta have input/output filename
   
   if(argc<4)
   {
	   output_format = 0;
   }
   else {
		output_format = atoi(argv[3]);
   }
   
   // Remove all comments from start, reduce headaches
   input = remove_comments(argv[1],argv[2]);
   
   int ins_counter = 0; // Instruction counter variable
   char instr[100]; // Instruction string
   char addr[100]; // Address string
   // While there is still more file to read
   while(!feof(input)) 
   {     
      int count = fscanf(input, " %100s #%100s ,",instr,addr);
      if(count==EOF)
      {
         break;
      }
      else if(count<2)
      {
         puts("Illegal instruction-address syntax (good luck finding it :D)");
         exit(EXIT_FAILURE);
      }
      else
      {
         if(ins_counter>=MAX_INSTRUCTIONS)
         {
            puts("There are too many instructions in this file!!");
            exit(EXIT_FAILURE);
         }
         // Instruction Interpreter
         instructions[ins_counter] += interpret_instr(instr);
		 // Address Interpreter
         instructions[ins_counter] += interpret_addr(addr,ins_counter);
         ins_counter++;
      }
   }
   
   fclose(input);
   remove(argv[2]);
   input = fopen(argv[2],"w"); //clear and reopen in write mode
   if(input==NULL)exit(EXIT_FAILURE);
   
   write_output(input, instructions, output_format);
   
   return 0;
}

/*************************************************************
 * Removes comments from our entire input file
**************************************************************/
FILE* remove_comments(const char* fin, const char* fout){
   FILE *input, *output;
   char line[1024];
   
   input = fopen(fin,"r");
   if(input == NULL)exit(EXIT_FAILURE);
   
   output = fopen(fout,"w+");
   if(output == NULL)exit(EXIT_FAILURE);
   
   while(!feof(input))
   {
      //take in string of up to 1024 characters
      char * res = fgets(line,1024,input);
      if(res==NULL)break;
      
      res = strchr(line,';');
      if(res!=NULL)*res = '\0';
      
      fputs(line,output);
   }
   fclose(input);
   rewind(output);
   return output;
}

/*************************************************************
 * Associates our assembly instructions into hex op codes
**************************************************************/
uint8_t interpret_instr(char* instr) //get the instruction
{
   if(strcmp(instr,"NOR")==0) //instruction gettin' part
   {
      return 0;
   }
   else if(strcmp(instr,"ADD")==0)
   {
      return 0x40;
   }
   else if(strcmp(instr,"STA")==0)
   {
      return 0x80;
   }
   else if(strcmp(instr,"JCC")==0)
   {
      return 0xC0;
   }
   else
   {
      puts("Illegal instruction syntax (good luck finding it :D)");
      exit(EXIT_FAILURE);
   }
}

/*************************************************************
 * Interprets the address given in our assembly code
**************************************************************/
uint8_t interpret_addr(char* addr,int i) //get the address
{
   long raw_addr;
   if(addr[0]=='*') 
   {
      raw_addr = i + strtol(addr+1,NULL,0); //might be hacky
   }
   else if(strncmp(addr,"0b",2)==0)
   {
      raw_addr = strtol(addr+2,NULL,2);
   }
   else
   {
      raw_addr = strtol(addr,NULL,0);
   }
   
   if(raw_addr < 0 || raw_addr >= 64)
   {
      puts("Illegal address value/syntax (good luck finding it :D)");
      exit(EXIT_FAILURE);
   }
   
   return raw_addr;
}

/*************************************************************
 * Writes to our output file our instructions in the selected
 * format.
**************************************************************/
void write_output(FILE* fout, uint8_t instructions[64], int format_hex)
{
   int i;
   for(i=0;i<MAX_INSTRUCTIONS;i++)
   {
      if(instructions[i]==0) //special case so 0 has 0x beginning
      {	
		 // Switch based on users formatting input argument
		 switch(format_hex){
			case 1:
				fputs("00\n", fout);
				break;
			case 2:
				fputs("0b00000000\n", fout);
				break;
			case 3:
				fputs("00000000\n", fout);
				break;
			case 4:
				fputs("0000 0000\n", fout);
				break;
			default:
				fputs("0X00\n",fout); 
		 }
      }
      else
      {
		// Switch based on users formatting input argument
		switch(format_hex){
			char *tmp;
			case 1:
				fprintf(fout,"%X\n",instructions[i]);
				break;
			case 2:
				// Kinda hacky but works well enough
				tmp = strdup(hex_to_bin(instructions[i]/16));
				fprintf(fout,"0b%.4s%.4s\n", tmp, hex_to_bin(instructions[i]%16));
				break;
			case 3:
				tmp = strdup(hex_to_bin(instructions[i]/16));
				fprintf(fout,"%.4s%.4s\n", tmp, hex_to_bin(instructions[i]%16));
				break;
			case 4:
				tmp = strdup(hex_to_bin(instructions[i]/16));
				fprintf(fout,"%s %s\n", tmp, hex_to_bin(instructions[i]%16));
				break;
			default:
				fprintf(fout,"%#X\n",instructions[i]);
		 }
      }
   }
}
/*************************************************************
 * Simple single hex digit to binary converter
**************************************************************/
char* hex_to_bin(int hex){
	static char tmp[4];
	if (hex >= 8)
	{
		tmp[0] = '1';
		hex -= 8;
	} 
	else {
		tmp[0] = '0';
	}
	if (hex >= 4)
	{
		tmp[1] = '1';
		hex -= 4;
	} 
	else {
		tmp[1] = '0';
	}
	if (hex >= 2)
	{
		tmp[2] = '1';
		hex -= 2;
	} 
	else {
		tmp[2] = '0';
	}
	if (hex >= 1)
	{
		tmp[3] = '1';
		hex -= 1;
	} 
	else 
	{
		tmp[3] = '0';
	}
	return tmp;
}
