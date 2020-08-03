// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.


//it would be nice if it found out which number was bigger
//add the bigger number to itself a smaller number of times
//Version2

@0
D = A
@R2
M = D
@R1
D=M
@R0
D=D-M

@r1isbigger
D;JGE  //if r1 is smaller than r2
@R1
D = M
@count
M = D
(LOOP)
@count
D=M
@END
D;JLE
@R0
D=M
@R2
M=M+D
@count
M = M - 1
@LOOP
0;JMP


(r1isbigger) //r1 is bigger
@R0
D = M
@count2
M = D
(LOOP2)
@count2
D=M
@END
D;JLE
@R1
D=M
@R2
M=M+D
@count2
M = M - 1
@LOOP2
0;JMP
(END)
@END
0;JMP

