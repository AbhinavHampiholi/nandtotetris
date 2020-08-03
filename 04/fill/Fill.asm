// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

@SCREEN
D = A
@count
M = D

(LOOP)
@KBD
D = M
@BLACKEN
D;JNE
@WHITEN
0;JMP

(BLACKEN)
@count   //Checks Overflow
D=M
@KBD
D=A-D
D=D-1
@LOOP
D;JLE

@count//increments counter
D=M
A=D
M=M-1
@count
M=M+1
@LOOP
0;JMP


(WHITEN)
@count  //Checks Underflow
D=M
@SCREEN
D=D-A
@LOOP
D;JLT

@count//decrements counter
D=M
A=D
M=0
@count
M=M-1
@LOOP
0;JMP
