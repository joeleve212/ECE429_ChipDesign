import sys
inFileName = sys.argv[1]         #Use first command line arg as name of input file
inFile = open(inFileName,"r")     
lines = inFile.readlines()      #populate lines with strings from each line
mem = []
for i in range(len(lines)):     #scroll through each line of file
    if(i!=len(lines)-1):        #if not last line,
        lines[i] = lines[i][0:len(lines[i])-1] #remove newline from each line
    mem.append(int(lines[i],16))
    # print(mem[i])  #can be used to see what's being loaded into memory
inFile.close()

pc = 0
acc = 0
cf = 0

while pc <64:                   #keeps going until finished going through all instructions
    instruction = mem[pc & 0x3F]#using pc as the index, grab next instruction
    pc += 1                     #increment program counter each instruction         
    opcode = instruction >> 6   #take 2 MSBs as the op code
    arg = instruction & 0x3F    #take the 6 LSBs as the argument
    
    #show values of all variables at start of instruction 
    print('pc,ir,acc,cf:', pc-1, hex(instruction), acc, cf >> 8, sep='\t')
    
    if opcode == 0:         # NOR operation
        acc = (acc | mem[arg]) ^ 0xFF
    elif opcode == 1:       # ADD operation
        acc += mem[arg]
        cf = acc & 0x100    #update carry flag w/ 9th bit of adder
        acc &= 0xFF         #truncate accumulator to first 8 bits
    elif opcode == 2:       # STA/store operation
        mem[arg] = acc
        print('+', arg, hex(mem[arg]))
    else:                   # JCC operation - jump to pc = arg if carry is 0, then reset carry
        if cf == 0:              #if carry is 0,
            pc = arg               #move program counter to desired location
        cf = 0                     #reset carry