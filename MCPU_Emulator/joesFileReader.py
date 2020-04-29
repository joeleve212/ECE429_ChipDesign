import sys

inFile = open("exInput.txt","r")
lines = inFile.readlines()

for i in range(len(lines)):
    lines[i] = lines[i][0:len(lines[i])-1] #remove newline from each line
    print(lines[i])

inFile.close()