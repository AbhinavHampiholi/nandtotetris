(Sys.initvm1.txt)
@4000	
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
@THIS
M=D
@5000
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
@THAT
M=D
@0Sys.mainvm1.txt
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@SP
M=M+1
A=M-1
M=D
@ARG
D=M
@SP
M=M+1
A=M-1
M=D
@THIS
D=M
@SP
M=M+1
A=M-1
M=D
@THAT
D=M
@SP
M=M+1
A=M-1
M=D
@SP
D=M
@0
D=D-A
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.mainvm1.txt
0;JMP
(0Sys.mainvm1.txt)
@6
D=A
@SP
AM=M-1
D=D+M
A=D-M
M=D-A
(Sys.mainvm1.txtLOOPvm1.txt)
@Sys.mainvm1.txtLOOPvm1.txt
0;JMP
(Sys.mainvm1.txt)
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
@SP
M=M+1
A=M-1
M=D
@4001
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
@THIS
M=D
@5001
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
@THAT
M=D
@200
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@1
D=A+D
@SP
AM=M-1
D=D+M
A=D-M
M=D-A
@40
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@2
D=A+D
@SP
AM=M-1
D=D+M
A=D-M
M=D-A
@6
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@3
D=A+D
@SP
AM=M-1
D=D+M
A=D-M
M=D-A
@123
D=A
@SP
M=M+1
A=M-1
M=D
@1Sys.add12vm1.txt
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@SP
M=M+1
A=M-1
M=D
@ARG
D=M
@SP
M=M+1
A=M-1
M=D
@THIS
D=M
@SP
M=M+1
A=M-1
M=D
@THAT
D=M
@SP
M=M+1
A=M-1
M=D
@SP
D=M
@1
D=D-A
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.add12vm1.txt
0;JMP
(1Sys.add12vm1.txt)
@5
D=A
@SP
AM=M-1
D=D+M
A=D-M
M=D-A
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
@2
D=A
@LCL
A=M+D
D=M
@SP
M=M+1
A=M-1
M=D
@3
D=A
@LCL
A=M+D
D=M
@SP
M=M+1
A=M-1
M=D
@4
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
AM=M-1
D=M
A=A-1
M=M+D
@SP
AM=M-1
D=M
A=A-1
M=M+D
@SP
AM=M-1
D=M
A=A-1
M=M+D
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
(Sys.add12vm1.txt)
@4002
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
@THIS
M=D
@5002
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
@THAT
M=D
@0
D=A
@ARG
A=M+D
D=M
@SP
M=M+1
A=M-1
M=D
@12
D=A
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
