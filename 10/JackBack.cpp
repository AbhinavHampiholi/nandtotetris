 
 /*
 
 NON TERMINAL TOKENS

 class, classVarDec, subroutineDec, parameterList, subroutineBody, varDec;
 statements, whileSatement, ifStatement, returnStatement, letStatement, doStatement
 expression, term, expressionList.


TERMINALS
keyword, symbol, integerConstant, stringConstant, or identifier.


class: 'class' className '{' classVarDec* subroutineDec* '}'
classVarDec: ('static' | 'field') type varName (',' varName)* ';'
type: 'int' | 'char' | 'boolean' | className
subroutineDec: ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
parameterList: ((type varName) (',' type varName)*)?




*******************************SUBROUTINES**********************************

CompileClass
CompileClassVarDec
CompileSubroutine
compileParameterList
compileVarDec 
compileStatements
compileDo 
compileLet 
compileWhile 
compileReturn 
compileIf 
CompileExpression 
CompileTerm
CompileExpressionList

 */

#include<iostream>
#include<regex>
#include<fstream>
#include<string>
#include <algorithm>
#include <iterator>
#include <cctype>

using namespace std;

string filename;
string outputfile;

class Tok{
    public:
    string type;
    string data;
    int tokenNumber;
};

vector <Tok> Token;

int tno = 0;

void filltokens();
void parameterList();
void compileVarDec();
bool isop();
bool isUnary();
bool isKeywordConst();
void compileExpression();
void compileExpressionList();
void compileSubroutineCall();
void compileTerm();
void compileReturn();
void compileDo();
void compileWhile();
void compileIf();
void compileLet();
void compileStatements();
void compileSubroutineBody();
void compileClassVarDec();
void compileSubroutineDec();
void compileclass();



void filltokens(){
    ifstream fin (filename);
    
    int tnumber = 0;
    string line;
    
    while(getline(fin, line, '\n')){
        if(line.find("tokens")==std::string::npos){
            Tok foo;
            foo.tokenNumber = tnumber;
            tnumber++;
            string t = line.substr(1,line.find(">")-1);
            foo.type = t;
            line = line.substr(line.find(">"));
            t = line.substr(2,line.find("<")-3);
            foo.data = t;
            Token.push_back(foo);
        }
    }
}

void parameterList(){
    ofstream gout;
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    gout<<"<parameterList>\n";
    if(Token[tno].data == "int" || Token[tno].data == "boolean" || Token[tno].data == "char" || Token[tno].type == "identifier"){
        gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
        tno++;
        
        if(Token[tno].type != "identifier"){
            ofstream fout("err.txt");
            fout<<"Syntax Error: <identifier> expected\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
        }
        gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
        tno++;

        while(Token[tno].data == ","){
            gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
            tno++;
            if(!(Token[tno].data == "int" || Token[tno].data == "boolean" || Token[tno].data == "char" || Token[tno].type == "identifier")){
                ofstream fout("err.txt");
                fout<<"Syntax Error: <type> expected\n";
                fout.close();
                cout<<"compilation error\n";
                exit(0);
            }
            gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
            tno++;

            if(Token[tno].type != "identifier"){
                ofstream fout("err.txt");
                fout<<"Syntax Error: <identifier> expected\n";
                fout.close();
                cout<<"compilation error\n";
                exit(0);
            }
            gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
            tno++;
        }
    }
    gout<<"</parameterList>\n";
    gout.close();

}

void compileVarDec(){
    ofstream gout;
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    gout<<"<varDec>\n";

    if(Token[tno].data != "var"){
        ofstream fout("err.txt");
        fout<<"Syntax Error: <var> expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    if(!(Token[tno].data == "int" || Token[tno].data == "boolean" || Token[tno].data == "char" || Token[tno].type == "identifier")){
        ofstream fout("err.txt");
        fout<<"Syntax Error: <type> expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    if(Token[tno].type != "identifier"){
        ofstream fout("err.txt");
        fout<<"Syntax Error: <identifier> expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    while(Token[tno].data == ","){
            gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
            tno++;

            if(Token[tno].type != "identifier"){
                ofstream fout("err.txt");
                fout<<"Syntax Error: <identifier> expected\n";
                fout.close();
                cout<<"compilation error\n";
                exit(0);
            }
            gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
            tno++;
        }
    if(Token[tno].data != ";"){
        ofstream fout("err.txt");
        fout<<"Syntax Error: ; missing\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;
    gout<<"</varDec>\n";
    gout.close();

}


bool isop(){
    if(Token[tno].data == "+" || Token[tno].data == "-" || Token[tno].data == "/" ||Token[tno].data == "*" ||Token[tno].data == "=" ||Token[tno].data == "&" ||Token[tno].data == "|" ||Token[tno].data == "<" ||Token[tno].data == ">")
        return true;
    return false;
}

bool isUnary(){
    if(Token[tno].data == "-" || Token[tno].data == "~")
        return true;
    return false;
}

bool isKeywordConst(){
    if(Token[tno].data == "true" || Token[tno].data == "false" || Token[tno].data == "null" || Token[tno].data == "this")
        return true;
    return false;
}

void compileExpression(){
    ofstream gout;
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    gout<<"<expression>\n";
    gout.close();
    compileTerm();

    while(isop()){
        gout.open (outputfile, std::ofstream::out | std::ofstream::app);
        gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
        tno++;
        gout.close();
        compileTerm();
    }
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    gout<<"</expression>\n";
    gout.close();
}


void compileExpressionList(){
    ofstream gout;
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    gout<<"<expressionList>\n";
    gout.close();
    if(Token[tno].type == "integerConstant" || Token[tno].type == "stringConstant" || isKeywordConst() || Token[tno].type == "identifier" || Token[tno].data == "(" || isUnary()){
        //gout.close();
        compileExpression();
        while(Token[tno].data==","){
            gout.open (outputfile, std::ofstream::out | std::ofstream::app);
            gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
            tno++;
            gout.close();
            compileExpression();
        }
    }
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    gout<<"</expressionList>\n";
    gout.close();
}

void compileSubroutineCall(){
    ofstream gout;
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;
    if(!(Token[tno].data == "." || Token[tno].data == "(")){
            ofstream fout("err.txt");
            fout<<"Syntax Error: <.> or <(> expected in subroutine call\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
    }
    if(Token[tno].data == "."){
        gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
        tno++;
        if(Token[tno].type != "identifier"){
            ofstream fout("err.txt");
            fout<<"Syntax Error: identifier expected in subroutine call\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
        }
        gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
        tno++;
        if(Token[tno].data!="("){
            ofstream fout("err.txt");
            fout<<"Syntax Error: ( expected in subroutine call\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
        }
        gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
        tno++;
        gout.close();
        compileExpressionList();
        gout.open (outputfile, std::ofstream::out | std::ofstream::app);
        if(Token[tno].data!=")"){
            ofstream fout("err.txt");
            fout<<"Syntax Error: ) expected in subroutine call\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
        }
        gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
        tno++;
        gout.close();
    }

    if(Token[tno].data == "("){
        gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
        tno++;
        gout.close();
        compileExpressionList();
        gout.open (outputfile, std::ofstream::out | std::ofstream::app);
        if(Token[tno].data!=")"){
            ofstream fout("err.txt");
            fout<<"Syntax Error: ) expected in subroutine call\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
        }
        gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
        tno++;
        gout.close();
    }
}

void compileTerm(){
    ofstream gout;
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    gout<<"<term>\n";
    //gout.close();
    if(isUnary()){
        gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
        tno++;
        if(isUnary()){
            ofstream fout("err.txt");
            fout<<"Syntax Error: unexpected Unary Charecter\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
        }
        gout.close();
        compileTerm();
        gout.open (outputfile, std::ofstream::out | std::ofstream::app);
        gout<<"</term>\n";
        gout.close();
    }
    else{
        if(Token[tno].type == "integerConstant" || Token[tno].type == "stringConstant" || isKeywordConst() || Token[tno].type == "identifier" || Token[tno].data == "("){
            if(Token[tno].type == "integerConstant" || Token[tno].type == "stringConstant" || isKeywordConst()){
                gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
                tno++;
            }
            else if(Token[tno].data == "("){
                gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
                tno++;
                gout.close();
                compileExpression();
                gout.open (outputfile, std::ofstream::out | std::ofstream::app);
                if(Token[tno].data != ")"){
                    ofstream fout("err.txt");
                    fout<<"Syntax Error: ) expected\n";
                    fout.close();
                    cout<<"compilation error\n";
                    exit(0);
                }
            
                gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
                tno++;
            }

            else if(Token[tno].type == "identifier"){
                
                if(Token[tno+1].data=="." || Token[tno+1].data == "("){
                    gout.close();
                    compileSubroutineCall();
                    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
                }
                
                else if(Token[tno+1].data == "["){
                    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
                    tno++;
                    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
                    tno++;
                    gout.close();
                    compileExpression();
                    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
                    if(Token[tno].data != "]"){
                        ofstream fout("err.txt");
                        fout<<"Syntax Error: ] expected\n";
                        fout.close();
                        cout<<"compilation error\n";
                        exit(0);
                    }
                
                    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
                    tno++;
                }
                
                else if(Token[tno+1].data!="[" && Token[tno+1].data!="." && Token[tno+1].data!="("){
                    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
                    tno++;
                }
            }

        }
        else{
            ofstream fout("err.txt");
            fout<<"Syntax Error: term expected\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
        }

        gout<<"</term>\n";
        gout.close();
    }
}



void compileReturn(){
    ofstream gout;
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    gout<<"<returnStatement>\n";
    if(Token[tno].data != "return"){
            ofstream fout("err.txt");
            fout<<"Syntax Error: <return> expected\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
    }
    
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;
    gout.close();

    if(Token[tno].data == "(" || Token[tno].type == "stringConstant" || Token[tno].type == "integerConstant" || Token[tno].type == "identifier"){
        compileExpression();
    }
    
    
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    if(Token[tno].data != ";"){
            ofstream fout("err.txt");
            fout<<"Syntax Error: ; expected\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
    }
    
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;
    gout<<"</returnStatement>\n";
    gout.close();
}

void compileDo(){//requires compileSubroutineCall
    ofstream gout;
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    gout<<"<doStatement>\n";
    
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;
    gout.close();
    compileSubroutineCall();//to be written
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    if(Token[tno].data != ";"){
            ofstream fout("err.txt");
            fout<<"Syntax Error: ; expected\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
        }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;   
    gout<<"</doStatement>\n";
    gout.close();
}

void compileWhile(){
    ofstream gout;
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    gout<<"<whileStatement>\n";
    
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    if(Token[tno].data != "("){
            ofstream fout("err.txt");
            fout<<"Syntax Error: ( expected\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
    }
    
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;
    gout.close();
        
    compileExpression();
        
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);

    if(Token[tno].data != ")"){
            ofstream fout("err.txt");
            fout<<"Syntax Error: ) expected\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
        }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    if(Token[tno].data != "{"){
            ofstream fout("err.txt");
            fout<<"Syntax Error: { expected\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
    }
    
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;
    gout.close();
        
    compileStatements();
        
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);

    if(Token[tno].data != "}"){
            ofstream fout("err.txt");
            fout<<"Syntax Error: } expected\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
        }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;
    gout<<"</whileStatement>\n";
    gout.close();
}

void compileIf(){
    ofstream gout;
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    gout<<"<ifStatement>\n";
    
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;
    if(Token[tno].data != "("){
        ofstream fout("err.txt");
        fout<<"Syntax Error: ( expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    gout.close();
    compileExpression();
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);


    if(Token[tno].data != ")"){
        ofstream fout("err.txt");
        fout<<"Syntax Error: ) expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    if(Token[tno].data != "{"){
        ofstream fout("err.txt");
        fout<<"Syntax Error: { expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    gout.close();
    compileStatements();
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    
    if(Token[tno].data != "}"){
        ofstream fout("err.txt");
        fout<<"Syntax Error: } expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    if(Token[tno].data == "else"){
        gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
        tno++;

        if(Token[tno].data != "{"){
        ofstream fout("err.txt");
        fout<<"Syntax Error: { expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    gout.close();
    compileStatements();
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    
    if(Token[tno].data != "}"){
        ofstream fout("err.txt");
        fout<<"Syntax Error: } expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    }

    gout<<"</ifStatement>\n";
    gout.close();

}

void compileLet(){
    ofstream gout;
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    gout<<"<letStatement>\n";
    
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    if(Token[tno].type != "identifier"){
        ofstream fout("err.txt");
        fout<<"Syntax Error: <identifier> expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;
    gout.close();

    if(Token[tno].data == "["){
        gout.open (outputfile, std::ofstream::out | std::ofstream::app);
        gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
        tno++;
        gout.close();
        
        compileExpression();
        gout.open (outputfile, std::ofstream::out | std::ofstream::app);

        if(Token[tno].data != "]"){
            ofstream fout("err.txt");
            fout<<"Syntax Error: ] expected\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
        }
        gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
        tno++;
        gout.close();
    }
    
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    if(Token[tno].data != "="){
            ofstream fout("err.txt");
            fout<<"Syntax Error: = expected\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
        }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;    
    gout.close();

    compileExpression();

    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    if(Token[tno].data != ";"){
            ofstream fout("err.txt");
            fout<<"Syntax Error: ; expected\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
        }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;   
    gout<<"</letStatement>\n";
    gout.close();
}

void compileStatements(){
    ofstream gout;
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    gout<<"<statements>\n";
    gout.close();

    while(Token[tno].data == "let" || Token[tno].data == "if" || Token[tno].data == "while" || Token[tno].data == "do"  || Token[tno].data == "return"){
        if(Token[tno].data == "let")
            compileLet();
        if(Token[tno].data == "if")
            compileIf();
        if(Token[tno].data == "while")
            compileWhile();
        if(Token[tno].data == "do")
            compileDo();
        if(Token[tno].data == "return")
            compileReturn();
    }
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    gout<<"</statements>\n";
    gout.close();

}






void compileSubroutineBody(){
    ofstream gout;
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    gout<<"<subroutineBody>\n";

    if(Token[tno].type != "symbol" || Token[tno].data != "{"){
        ofstream fout("err.txt");
        fout<<"Syntax Error: { expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;
    gout.close();

    while(Token[tno].data == "var"){
        compileVarDec();
    }
    
    compileStatements();

    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    
    if(Token[tno].type != "symbol" || Token[tno].data != "}"){
        ofstream fout("err.txt");
        fout<<"Syntax Error: } expected\n";
        fout<<tno<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;
    gout<<"</subroutineBody>\n";
    gout.close();
}

void compileClassVarDec(){
    ofstream gout;
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    gout<<"<classVarDec>\n";
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;
    
    if(!(Token[tno].data == "int" || Token[tno].data == "boolean" || Token[tno].data == "char" || Token[tno].type == "identifier")){
        ofstream fout("err.txt");
        fout<<"Syntax Error: <type> expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    if(Token[tno].type != "identifier"){
        ofstream fout("err.txt");
        fout<<"Syntax Error: <identifier> expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    while(Token[tno].data == ","){
        gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
        tno++;
        if(Token[tno].type != "identifier"){
            ofstream fout("err.txt");
            fout<<"Syntax Error: <identifier> expected\n";
            fout.close();
            cout<<"compilation error\n";
            exit(0);
        }
        gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
        tno++;
    }
    
    if(Token[tno].data != ";"){
        ofstream fout("err.txt");
        fout<<"Syntax Error: ; missing\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;
    
    gout<<"</classVarDec>\n";
    //tno++;
    gout.close();
}

void compileSubroutineDec(){
    ofstream gout;
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    gout<<"<subroutineDec>\n";
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    if(Token[tno].data != "void" && Token[tno].data != "int" && Token[tno].data != "char" && Token[tno].data != "boolean" && Token[tno].type != "identifier") {
        ofstream fout("err.txt");
        fout<<"Syntax Error: <identifier> expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    if(Token[tno].type != "identifier"){
        ofstream fout("err.txt");
        fout<<"Syntax Error: <identifier> expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    if(Token[tno].data != "("){
        ofstream fout("err.txt");
        fout<<"Syntax Error: ( expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;
    gout.close();

    parameterList(); 

    gout.open (outputfile, std::ofstream::out | std::ofstream::app);

    if(Token[tno].data != ")"){
        ofstream fout("err.txt");
        fout<<"Syntax Error: ) expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    gout.close();

    compileSubroutineBody();

    gout.open (outputfile, std::ofstream::out | std::ofstream::app);
    
    gout<<"</subroutineDec>\n";
    //tno++;
    gout.close();
}

void compileclass(){
    //ofstream gout (outputfile);
    ofstream gout;
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);

    if(Token[tno].data != "class") {
        ofstream fout("err.txt");
        fout<<"Syntax Error: <identifier> expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    
    gout<<"<class>\n";
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;
    
    if(Token[tno].type != "identifier"){
        ofstream fout("err.txt");
        fout<<"Syntax Error: <identifier> expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;

    if(Token[tno].type != "symbol" || Token[tno].data != "{"){
        ofstream fout("err.txt");
        fout<<"Syntax Error: { expected\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;
    gout.close();

    while(Token[tno].data == "static" || Token[tno].data == "field"){
        compileClassVarDec(); //to be defined and has to increment tno
    }

    while(Token[tno].data == "constructor" || Token[tno].data == "function" || Token[tno].data == "method"){
        compileSubroutineDec(); //to be defined and has to increment tno
    }
    //ofstream gout;
    gout.open (outputfile, std::ofstream::out | std::ofstream::app);

    if(Token[tno].type != "symbol" || Token[tno].data != "}"){
        ofstream fout("err.txt");
        fout<<"Syntax Error: } expected\n";
        fout<<tno<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
        fout.close();
        cout<<"compilation error\n";
        exit(0);
    }
    gout<<"<"<<Token[tno].type<<"> "<<Token[tno].data<<" <"<<Token[tno].type<<">\n";
    tno++;
    gout<<"</class>\n";
    gout.close();
    
}



int main(int argc, char *argv[]){
    if(argc==0){
        cerr<<"Error:Filename not provided\n"; return 0;
    }

    for(int i = 1; i < argc; i++){
        filename = argv[i];
        outputfile = filename + ".xml";
        Token.clear();
        tno = 0;
        filltokens();
        compileclass();
    }


    return 0;
}