import sys

inFile = open("exInput.txt","r")
lines = inFile.readlines()
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

for i in range(64):
    instruction = mem[pc & 0x3F]
    pc += 1
    opcode = instruction >> 6
    arg = instruction & 0x3F
    
    print('pc,ir,acc,cf:', pc-1, hex(instruction), acc, cf >> 8, sep='\t')
    
    if opcode == 0:
        acc = (acc | mem[arg]) ^ 0xFF
    elif opcode == 1:
        acc += mem[arg]
        cf = acc & 0x100
        acc &= 0xFF
    elif opcode == 2:
        mem[arg] = acc
        print('+', arg, hex(mem[arg]))
    else:
        if instruction == 0xFF:
            print(acc)
        elif cf == 0:
            pc = arg
        cf = 0



# def interpStrHex(hexVal):
#     //
#     //
#     return decVal