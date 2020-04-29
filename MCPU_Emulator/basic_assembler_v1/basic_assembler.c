/* Monstrous first pass assembler
 * By James A. Douglas, Noisiest of Channels
 * 4/21/2020
 *
 * Reads in the first argument as a file,
 * then scans instructions according to the following rules:
 *
 *    1. Instruction name must be all caps (for now)
 *    2. No comments
 *    3. Commas or whitespace can separate instructions
 *    4. Address should be '#' followed by one of four forms
 *       a. decimal number
 *       b. hex number leading with 0x
 *       c. binary number leading with 0b
 *       d. * followed (optionally) by + or - and offset as hex or decimal
 *    
 * Outputs to stdout (for now).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
   uint8_t instructions[64] = {0};
   FILE* input;
   
   if(argc<2) return 1; //gotta have input filename
 
   input = fopen(argv[1],"r");
   if(input==NULL)return 1;
   
   int i = 0;
   int count = 0;
   char instr[100];
   char addr[100];
   while(!feof(input)) 
   {     
      int count = fscanf(input, " %100s #%100s %*[,]",instr,addr);
      if(count==EOF)
      {
         break;
      }
      else if(count<2)
      {
         puts("Illegal instruction-address syntax (good luck finding it :D)");
         fclose(input);
         return 1;
      }
      else
      {
         if(strcmp(instr,"NOR")==0) //instruction gettin' part
         {
            //twiddle thumbs anxiously
         }
         else if(strcmp(instr,"ADD")==0)
         {
            instructions[i]|=0x40;
         }
         else if(strcmp(instr,"STA")==0)
         {
            instructions[i]|=0x80;
         }
         else if(strcmp(instr,"JCC")==0)
         {
            instructions[i]|=0xC0;
         }
         else
         {
            puts("Illegal instruction syntax (good luck finding it :D)");
            fclose(input);
            return 1;
         }
         
         long raw_addr;
         if(addr[0]=='*') //address gettin' part
         {
            raw_addr = i + strtol(addr+1,NULL,0); //might be hacky since no number returns a 0
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
            fclose(input);
            return 1;
         }
         
         instructions[i] += raw_addr;
         i++;
      }
   }
      
   fclose(input);
   
   for(i=0;i<64;i++)
   {
      printf("%#X\n",instructions[i]);
   }
   
   return 0;
}
