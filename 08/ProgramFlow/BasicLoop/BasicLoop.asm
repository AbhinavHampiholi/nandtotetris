@0
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@0
D=A+D
@SP
AM=M-1
D=D+M
A=D-M
M=D-A
(LOOP_STARTvm1.txt)
@0
D=A
@ARG
A=M+D
D=M
@SP
M=M+1
A=M-1
M=D
@0
D=A
@LCL
A=M+D
D=M
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
M=M+D
@LCL
D=M
@0	
D=A+D
@SP
AM=M-1
D=D+M
A=D-M
M=D-A
@0
D=A
@ARG
A=M+D
D=M
@SP
M=M+1
A=M-1
M=D
@1
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
M=M-D
@ARG
D=M
@0
D=A+D
@SP
AM=M-1
D=D+M
A=D-M
M=D-A
@0
D=A
@ARG
A=M+D
D=M
@SP
M=M+1
A=M-1
M=D
@5
D=A
@SP
AM=M-1
D=D+M
A=D-M
M=D-A
@5
D=M
@LOOP_STARTvm1.txt
D;JNE
@0
D=A
@LCL
A=M+D
D=M
@SP
M=M+1
A=M-1
M=D
