# DLX-Simulator

This project is a simulator of DLX instructions, programmed in C++ using Qt for user interface. The simulator is capable of reading and parsing DLX instructions, classify them by R,J,I type, and properly execute each one while displaying real time values of each register and memory location.

# Introduction

THe purpose of this project is to implement a full simulator of DLX instructions. It’s main objective is to make it easier for the users to better understand how each instruction works, what value each of the 32 registers have at any given time, and what values does the memory store at any given address location. The simulator does also give a summary output after each instruction execution, showing the affected registers, and their new values.
A program counter (PC) is also implemented to simulate branching and jumping instructions.
Instructions are loaded by writing in the input field, they are validated and classified as R, J, or I type, then loaded into memory for execution. 
Stepping function is implemented to step through each instruction individually, or execute them all at the same time.
For simulation purposes, memory size is limited to 1KB, registers are all integers.

# INSTRUCTION FORMATS

All 3 DLX instruction formats are supported

•	“R” type instructions: All ALU operations such as ADD, AND, SUB, OR, SEQ, SLE, SLL, SLT, SNE, SRA,SRL and XOR
•	“I” type instructions: ADDI, ANDI, BEQZ, BNEZ, JALR, JR, LHI, LW, ORI, SEQI, SLEI, SLLI,SLTI,SNEI,SRAI,SRLI,SUBI,SW,XORI
•	“J” type instructions: J,JAL

# User Interface

![ScreenShot1](https://github.com/WavRX/DLX-Simulator/blob/main/ScreenShots/4%20copy.PNG)

The simulator works by taking an input through the input box. 
Input is a string representing an instruction, Ex: XOR r1,r2,r3
Once the instruction is written in the input box, “Load into mem” button should be pressed to load it in the instruction queue (memory).
Before an instruction is loaded, its syntax is validated, and classified by R,J,I type.
Classification is done by parsing the input string, extracting the first operation in the string, for example ADD, SUB, ADDI…, and check if it belongs to any of the instruction types. If it does, it is accepted and loaded into memory to be processes, if not, an error message is displayed.

## Example of good and bad instruction validation


![ScreenShot1](https://github.com/WavRX/DLX-Simulator/blob/main/ScreenShots/goodinstruction%20copy.PNG)
![ScreenShot1](https://github.com/WavRX/DLX-Simulator/blob/main/ScreenShots/bad%20instruction%20copy.PNG)

## Registers

There are 32 integer registers, they are displayed under the “Registers” field. Each register is labeled from 0 to 31, with its real time value next to it. Values are in HEX format, 4 bytes. 

## Memory

Memory is limited to 1KB in size. It is displayed under “Memory” field. Each memory location is shown with an address and value. Each location is a WORD in size, byte addressable, following Big Endian format.

## Instructions in memory

Once instructions are checked for syntax errors, they are placed in “Instructions in memory” table.
This is where all the instructions are queued and are ready to be executed.
Three buttons are available:
•	Execute: To let the simulator execute all the instructions in memory at once.
•	Step: To step through each instruction at a time, useful to observe changes in registers, memory and PC.
•	Stop: To stop the execution of the program, in case there is an infinite loop.

# Instructions overview

After each instruction is validated, depending on its type, RD, RS1, RS2, and appropriate fields are extracted from the instruction. They are then used to perform the task.
For example: In an ADD operation, RS1 and RS2 are extracted from the ADD instruction, added to together and stored in RD. RD is then updated in the register table and result is show. 

Same process is done for all the remaining instructions.


