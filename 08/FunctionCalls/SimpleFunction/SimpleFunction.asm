(SimpleFunction.testvm1.txt)
@0
D=A
@SP
M=M+1
A=M-1
M=D
@0
D=A
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
@1
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
@SP
A=M-1
M=!M
@0
D=A
@ARG
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
@1
D=A
@ARG
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
M=M-D
@LCL
D=M
@FRAME
M=D
@5
D=A
@FRAME
A=M-D
D=M
@RET
M=D
@SP
AM=M-1
D=M
@ARG
A=M
M=D
D=A
@SP
M=D+1
@FRAME
A=M-1
D=M
@THAT
M=D
@2
D=A
@FRAME
A=M-D
D=M
@THIS
M=D
@3
D=A
@FRAME
A=M-D
D=M
@ARG
M=D
@4
D=A
@FRAME
A=M-D
D=M
@LCL
M=D
@RET
A=M
0;JMP