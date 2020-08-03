//LATEST VERSION 6:27pm

#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <map>
#include <stdlib.h>

using namespace std;

/***********************************************************
 * CHANGES MADE TO KAMAKOTI SLIDES
 * in compileDoStatement(); push kind index NOT push kind type
 * in compileClassVarDec(); kind "field" is replaced by "this"
 * *********************************************************/


struct globalSymbolTable{
    string name;
    string type; //int, boolean, char, classname
    string kind; //static, field
    int number;
    static int staticCount;
    static int fieldCount;
    static int totalCount;

};

struct subroutineSymbolTable{
    string name;
    string type; //int, boolean, char, classname
    string kind; //argument, local, var
    int number;
    static int argumentCount;
    static int totalCount;
    static int localCount;

};

int globalSymbolTable:: staticCount = 0;
int globalSymbolTable:: fieldCount = 0;
int globalSymbolTable:: totalCount = 0;

int subroutineSymbolTable:: argumentCount = 0;
int subroutineSymbolTable:: totalCount = 0;
int subroutineSymbolTable:: localCount = 0;

class Tok{
    public:
    bool isToken;
    bool isStart;
    bool isEnd;
    string type;
    string data;
};

string filename;
string outputfile;
string currentClassName;
string currentSubroutineName;
string currentSubroutineType;
vector <Tok> Token;
int tno;
int labelnum = 0;
string ss; //standing in for a stringStream
map <string, globalSymbolTable> table;
map <string, subroutineSymbolTable> subroutineTable;

void makeTokens();
void compileTerm();
bool isop();
void compileExpression();
void compileParameterList();
void compileVarDec();
void compileLetStatement();
void compileIfStatement();
void compileWhileStatement();
int compileExpressionList();
void compileDoStatement();
void compileReturnStatement();
void compileStatements();
void compileSubroutineBody();
void compileClassVarDec();
void compileSubroutineDec();
void compileClass();



void makeTokens(){
    ifstream fin (filename);
    string line;
    
    while(getline(fin, line, '\n')){
        Tok foo;
        if(count(line.begin(), line.end(), '<') == 1){
            foo.isToken = false;
            if(count(line.begin(), line.end(), '/')==1){
                foo.isStart = false;
                foo.isEnd = true;
                foo.type = "endTag";
            }
            else{
                foo.isStart = true;
                foo.isEnd = false;
                foo.type = "startTag";
            }
            foo.data = line.substr(1,line.size()-2);
        }
        else{
            foo.isToken = true;
            foo.isStart = false;
            foo.isEnd = false;
            string t = line.substr(1,line.find(">")-1);
            foo.type = t;
            line = line.substr(line.find(">"));
            t = line.substr(2,line.find("<")-3);
            foo.data = t;
        }
        //cout<<foo.type<<" "<<foo.data<<"*"<<endl;
        Token.push_back(foo);
    }
    fin.close();
}

void compileTerm(){
    std::cout<<"entering term\n";
    tno++; //past the <term>
    if(Token[tno].data == "-" || Token[tno].data == "~"){
        string op = Token[tno].data;
        tno++;
        compileTerm();
        tno++;
        if(op == "-")
            ss+="neg\n";
        if(op == "~")
            ss+="not\n";
    }
    if(Token[tno].data == "("){
        tno++;
        compileExpression();
        tno++; //past the )
        tno++; //past the </term>
        std::cout<<"exiting: term "<<Token[tno-1].data<<" "<<tno<<endl;

        return;
    }
    if(Token[tno].type == "integerConstant"){
        ss+="push constant " + Token[tno].data + "\n";
        tno++; //past intconstant
        tno++;//past the </term>
        std::cout<<"exiting: term "<<Token[tno-1].data<<" "<<tno<<endl;

        return;
    }
    //if(tno>=288 && tno<=295)std::cout<<tno<<" "<<Token[tno].data<<endl;
    if(Token[tno].data == "true" || Token[tno].data == "false" || Token[tno].data == "null" || Token[tno].data == "this"){
        if(Token[tno].data == "true"){
            ss+="push constant 0\n";
            ss+="not\n";
        }
        if(Token[tno].data == "false"){
            ss+="push constant 0\n";
        }
        if(Token[tno].data == "null"){
            ss+="push constant 0\n";
        }
        if(Token[tno].data == "this"){
            ss+="push pointer 0\n";
        }
        tno++; //past the keywordConstant
        tno++; //past the </term>
       std::cout<<"exiting: term "<<Token[tno-1].data<<" "<<tno<<endl;

        return;
    }
    
    if(Token[tno].type == "identifier" && Token[tno + 1].type!="symbol"){
        string id1 = Token[tno].data;
        string type, kind;
        int index;
        
        if(subroutineTable.count(id1)==1){
            kind = subroutineTable[id1].kind;
            type = subroutineTable[id1].type;
            index = subroutineTable[id1].number;
        }

        else if(table.count(id1)==1){
            kind = table[id1].kind;
            type = table[id1].type;
            index = table[id1].number;
        }
        
        if(subroutineTable.count(id1)==0 && table.count(id1)==0){
            ofstream fout("err.txt");
            fout<<"Undeclared variable : "<<id1;
            std::cout<<"compilation error\n";
            fout.close();
            std::exit(0);
        }

        ss+="push " + kind + " " + to_string(index) + "\n";
        tno++; //past the keyword identifier 
        tno++; //past the </term>
           std::cout<<"exiting: term "<<Token[tno-1].data<<" "<<tno<<endl;

        return;
    }

    if(Token[tno].type == "identifier" && Token[tno + 1].data== "["){
        
        string id1 = Token[tno].data;
        string type, kind;
        int index;
        
        if(subroutineTable.count(id1)==1){
            kind = subroutineTable[id1].kind;
            type = subroutineTable[id1].type;
            index = subroutineTable[id1].number;
        }

        else if(table.count(id1)==1){
            kind = table[id1].kind;
            type = table[id1].type;
            index = table[id1].number;
        }
        
        if(subroutineTable.count(id1)==0 && table.count(id1)==0){
            ofstream fout("err.txt");
            fout<<"Undeclared variable : "<<id1;
            std::cout<<"compilation error\n";
            fout.close();
            std::exit(0);
        }
        
        
        tno++;
        tno++; //going past the [
        compileExpression();
        ss+="push " + kind + " " + to_string(index) + "\n";
        ss+="add\n";
        ss+="pop pointer 1\n";
        ss+="push that 0\n";
        tno++; //going past the ]
        tno++; //going past the </term>
           std::cout<<"exiting: term "<<Token[tno-1].data<<" "<<tno<<endl;

        return;
    }

    if(Token[tno].type == "stringConstant"){
        string thestr = Token[tno].data;
        int len = thestr.length();
        ss+="push constant " + to_string(len) + "\n";
        ss+="call String.new 1\n";
        for(int i = 0; i<len; i++){
            int cha;
            cha = thestr[i];
            ss+="push constant " + to_string(cha) + "\n";
            ss+="call String.appendChar 2\n";
        }
        tno++; //past the keyword stringConstant
        tno++; //past the </term>
           std::cout<<"exiting: term "<<Token[tno-1].data<<" "<<tno<<endl;

        return;
    }

    if(Token[tno].type == "identifier" && (Token[tno + 1].data== "(" || Token[tno + 1].data== "." )){
        string id1 = Token[tno].data;
        tno++;
        if(Token[tno].data == "."){
            tno++;//past the .
            string id2 = Token[tno].data;
            tno++; //past the identifier name
            string type, kind;
            if(subroutineTable.count(id1)==1){
                kind = subroutineTable[id1].kind;
                type = subroutineTable[id1].type;
                int index = subroutineTable[id1].number;
                ss+= "push " + kind + " " + to_string(index) + "\n";
            }

            else if(table.count(id1)==1){
                kind = table[id1].kind;
                type = table[id1].type;
                int index = table[id1].number;
                ss+= "push " + kind + " " + to_string(index) + "\n";
            }

            tno++;//past the (
            int np = compileExpressionList();
            tno++; //past the )
            //tno++; //past the ;

            if(table.count(id1)==0 && subroutineTable.count(id1)==0){
                ss+="call " + id1 + "." + id2 + " " + to_string(np) + "\n";
                //ss+="pop temp 0\n";
            }

            else{
                ss+="call " + type + "." + id2 + " " + to_string(np+1) + "\n";
                //ss+="pop temp 0\n";
            }
            tno++; //past the </term>
               std::cout<<"exiting: term "<<Token[tno-1].data<<" "<<tno<<endl;

            return;
        }
        else{
            tno++; //past the (
            ss+="push pointer 0\n";
            int np = compileExpressionList();
            tno++; //past the )
            //tno++; //past the ;
            ss+="call " + currentClassName + "." + id1 + " " + to_string(np+1) + "\n";
            //ss+= "pop temp 0\n";
            tno++; //past the <\term>
               std::cout<<"exiting: term "<<Token[tno-1].data<<" "<<tno<<endl;

            return;
        }
    }



}

bool isop(){
    if(Token[tno].data == "+" || Token[tno].data == "-" || Token[tno].data == "/" ||Token[tno].data == "*" ||Token[tno].data == "=" ||Token[tno].data == "&amp;" ||Token[tno].data == "|" ||Token[tno].data == "&lt;" ||Token[tno].data == "&gt;")
        return true;
    return false;
}

void compileExpression(){
   std::cout<<"entered Expression\n";
    tno++;
    if(Token[tno].data == "term"){
        compileTerm();
        while(isop()){
            string op = Token[tno].data;
            tno++; //past the operator
            compileTerm();
            if(op=="+")
                ss+="add\n";
            if(op=="-")
                ss+="sub\n";
            if(op=="&amp;")
                ss+="and\n";
            if(op=="|")
                ss+="or\n";
            if(op=="&gt;")
                ss+="gt\n";
            if(op=="&lt;")
                ss+="lt\n";
            if(op=="*")
                ss+="call Math.multiply 2\n";
            if(op=="/")
                ss+="call Math.divide 2\n";
            if(op=="=")
                ss+="eq\n";
        }
    }
   std::cout<<"exiting: Expression "<<Token[tno].data<<" "<<tno<<endl;

    tno++; //past the </expression>
}

/*
void compileParameterList(){
   std::cout<<"entered ParameterList\n";
    tno++; //advancing past <parameterList>
    if(Token[tno].data!="/parameterList"){
        do{
        string variableType = Token[tno].data;
        tno++;

        string variable = Token[tno].data;
        tno++;

        subroutineSymbolTable foo;
        foo.kind = "argument";
        foo.name = variable;
        foo.type = variableType;
        foo.number = subroutineSymbolTable::argumentCount;
        subroutineTable[variable] = foo;
        subroutineSymbolTable::argumentCount++;
        subroutineSymbolTable::totalCount++;

        tno++;
        }while(Token[tno].data ==",");
    }
    else
    tno++; //advancing past </parameterList>
    std::cout<<"exiting: ParameterList"<<Token[tno-1].data<<" "<<tno<<endl;
}
*/

void compileParameterList(){
    std::cout<<"entered ParameterList\n";
    tno++; //advancing past <parameterList>
    if(Token[tno].data!="/parameterList"){
        string variableType = Token[tno].data;
        tno++;

        string variable = Token[tno].data;
        tno++;

        subroutineSymbolTable foo;
        foo.kind = "argument";
        foo.name = variable;
        foo.type = variableType;
        foo.number = subroutineSymbolTable::argumentCount;
        subroutineTable[variable] = foo;
        subroutineSymbolTable::argumentCount++;
        subroutineSymbolTable::totalCount++;
        while(Token[tno].data ==","){
            tno++;
            string variableType = Token[tno].data;
            tno++;

            string variable = Token[tno].data;
            tno++;

            subroutineSymbolTable foo;
            foo.kind = "argument";
            foo.name = variable;
            foo.type = variableType;
            foo.number = subroutineSymbolTable::argumentCount;
            subroutineTable[variable] = foo;
            subroutineSymbolTable::argumentCount++;
            subroutineSymbolTable::totalCount++;
        }
    tno++;
    }
    else
    tno++;//advancing past </parameterList>
    
    std::cout<<"exiting: ParameterList"<<Token[tno-1].data<<" "<<tno<<endl;

}

void compileVarDec(){
   std::cout<<"entered VarDec\n";
    tno++; //past <vardec>
    tno++; //past keyword var
    do{
        string variableType = Token[tno].data;
        tno++; //past identifier
        string variable = Token[tno].data;
        tno++; //past the identifier
        subroutineSymbolTable foo;
        foo.kind = "local";
        foo.name = variable;
        foo.type = variableType;
        foo.number = subroutineSymbolTable::localCount;
        subroutineTable[variable] = foo;

        subroutineSymbolTable::localCount++;
        subroutineSymbolTable::totalCount++;
    }while(Token[tno].data == ","); //its either ; or ,
    

    tno++; //past the ;
   std::cout<<"exiting: VarDec "<<Token[tno].data<<" "<<tno<<endl;
    tno++;
}

void compileLetStatement(){
   std::cout<<"entered LetStatement\n";
    tno++;//going over <let>
    tno++;//going over let
    string variable = Token[tno].data;
    
    string kind;
    int index;

    if(subroutineTable.count(variable)==1){
        kind = subroutineTable[variable].kind;
        index = subroutineTable[variable].number;
    }

    else if(table.count(variable)==1){
        kind = table[variable].kind;
        index = table[variable].number;
    }

    if(subroutineTable.count(variable)==0 && table.count(variable)==0){
        ofstream fout("err.txt");
        fout<<"Undeclared variable : "<<variable;
        std::cout<<"compilation error\n";
        fout.close();
        std::exit(0);
    }
    tno++;
    if(Token[tno].data == "["){
        tno++;
        compileExpression();
        tno++; //going past the ]
        ss+="push " + kind + " " + to_string(index) + "\n";
        ss+="add\n";
        tno++; //going past =
        compileExpression();
        ss+= "pop temp 0\npop pointer 1\npush temp 0\npop that 0\n";
        tno++;
        tno++; //going past the </let>
    }
    else{
        tno++;
        compileExpression();
        ss+= "pop " + kind + " " + to_string(index) + "\n";
        tno++;//going past ;
        tno++; //going past </let>
    }
   std::cout<<"exiting: LetStatement "<<Token[tno-1].data<<" "<<tno<<endl;

}

void compileIfStatement(){
   std::cout<<"entered: ifStatement\n";
    tno++; //past the <if>
    tno++; //past the keyword if
    tno++; //past the (
    int tlabelnum = labelnum;
    labelnum+=2;
    compileExpression();
    tno++;//past the )
    tno++;//past the {
    ss+="not\nif-goto " + currentClassName + "." + to_string(tlabelnum) + "\n";
    compileStatements();
    tno++;//past the }
    ss+="goto " + currentClassName + "." + to_string(tlabelnum+1) + "\n";
    ss+= "label " + currentClassName + "." + to_string(tlabelnum) + "\n";
    if(Token[tno].data == "else"){
        tno++;  //past the else
        tno++; //past the {
        compileStatements();
        tno++;//past the }
    }
    ss+="label " + currentClassName + "." + to_string(tlabelnum + 1) + "\n";
   std::cout<<"exiting: ifStatement "<<Token[tno].data<<" "<<tno<<endl;
    tno++; //past the </ifstatement>
}

void compileWhileStatement(){
   std::cout<<"entered whileStatement\n";
    tno++; //past the <while>
    tno++; //past the keyword while
    tno++; //past the (
    int tlabelnum = labelnum;
    labelnum+=2;
    ss+="label " + currentClassName + "." + to_string(tlabelnum) + "\n";
    compileExpression();
    tno++; //past the )
    ss+="not\nif-goto " + currentClassName + "." + to_string(tlabelnum+1) + "\n";
    tno++; //past the {
    compileStatements();
    ss+="goto " + currentClassName + "." + to_string(tlabelnum) + "\n";
    ss+="label " + currentClassName + "." + to_string(tlabelnum + 1) + "\n";
    tno++; //past the }
   std::cout<<"exiting: whileStatement "<<Token[tno].data<<" "<<tno<<endl;
    tno++; //past the </while>

}

int compileExpressionList(){
   std::cout<<"entered ExpressionList\n";
    int np = 0;
    tno++; //going past <expressioonList>
    if(Token[tno].data == "expression"){
        compileExpression();
        np++;
        while(Token[tno].data == ","){
            tno++;
            compileExpression();
            np++;
        }
    }
   std::cout<<"exiting: ExpressionList "<<Token[tno].data<<" "<<tno<<endl;
    tno++; //going past </expList>
    return np;
}

void compileDoStatement(){
   std::cout<<"entered Dostatement\n";
    tno++;//past the <do>
    tno++; // past keyword do
    string id1 = Token[tno].data;
    tno++;
    if(Token[tno].data == "."){
        tno++;//past the .
        string id2 = Token[tno].data;
        tno++; //past the identifier name
        string type, kind;
        if(subroutineTable.count(id1)==1){
            kind = subroutineTable[id1].kind;
            type = subroutineTable[id1].type;
            int index = subroutineTable[id1].number;
            ss+= "push " + kind + " " + to_string(index) + "\n";
        }

        else if(table.count(id1)==1){
            kind = table[id1].kind;
            type = table[id1].type;
            int index = table[id1].number;
            ss+= "push " + kind + " " + to_string(index) + "\n";
        }

        tno++;//past the (
        int np = compileExpressionList();
        tno++; //past the )
        tno++; //past the ;

        if(table.count(id1)==0 && subroutineTable.count(id1)==0){
            ss+="call " + id1 + "." + id2 + " " + to_string(np) + "\n";
            ss+="pop temp 0\n";
        }

        else{
            ss+="call " + type + "." + id2 + " " + to_string(np+1) + "\n";
            ss+="pop temp 0\n";
        }
        tno++; //past the </do>
    }
    else{
        tno++; //past the (
        ss+="push pointer 0\n";
        int np = compileExpressionList();
        tno++; //past the )
        tno++; //past the ;
        ss+="call " + currentClassName + "." + id1 + " " + to_string(np+1) + "\n";
        ss+= "pop temp 0\n";
        tno++; //past the <\do>
    }
   std::cout<<"exiting: Do "<<Token[tno-1].data<<" "<<tno<<endl;

}

void compileReturnStatement(){
   std::cout<<"entering ReturnStatement\n";
    tno++; //going past <ret>
    tno++; //going past keyword return
    if(Token[tno].data=="expression"){
        compileExpression();
        ss+="return\n";
    }
    else{
        ss+="push constant 0\nreturn\n";
    }

    tno++;//past the ;
   std::cout<<"exiting: Return "<<Token[tno].data<<" "<<tno<<endl;

    tno++;//past the </ret>
}

void compileStatements(){
   std::cout<<"entered Statements\n";
    tno++;
    while(Token[tno].data == "letStatement" || Token[tno].data == "doStatement" || Token[tno].data == "ifStatement" || Token[tno].data == "whileStatement" || Token[tno].data == "returnStatement"){
        if(Token[tno].data == "letStatement"){
            compileLetStatement();
        }

        if(Token[tno].data == "ifStatement"){
            compileIfStatement();
        }

        if(Token[tno].data == "whileStatement"){
            compileWhileStatement();
        }

        if(Token[tno].data == "doStatement"){
            compileDoStatement();
        }
        
        if(Token[tno].data == "returnStatement"){
            compileReturnStatement();
        }
    }
    
   std::cout<<"exiting: Statements "<<Token[tno].data<<" "<<tno<<endl;

    tno++;
}

void compileSubroutineBody(){
   std::cout<<"entered SubroutineBody\n";
    tno++; //past <subroutineBody>
    tno++; //past {
    while(Token[tno].data == "varDec"){
        compileVarDec();
    }

    ss+= "function " + currentClassName + "." + currentSubroutineName + " " + to_string(subroutineSymbolTable::localCount) + "\n";
    
    if(currentSubroutineType == "constructor"){
        ss+= "push constant " + to_string(globalSymbolTable::fieldCount) + "\n";
        ss+= "call Memory.alloc 1\n";
        ss+= "pop pointer 0\n";
    }

    if(currentSubroutineType == "method"){
        ss+= "push argument 0\n";
        ss+= "pop pointer 0\n";
    }

    compileStatements();    
    tno++; //past }
   std::cout<<"exiting: SubroutineBody "<<Token[tno].data<<" "<<tno<<endl;
    tno++; //past </subroutineBody>
}

void compileClassVarDec(){
   std::cout<<"entered ClassVarDec\n";
    tno++; //going past <ClassVarDec>
    string variableKind;//static or field
    variableKind = Token[tno].data; //keyword field/static
    if(variableKind == "field") variableKind = "this";
    tno++;
    string variableType = Token[tno].data; //keyword int/boolean/char/
    tno++;
    string variable = Token[tno].data; //identifier varName
    globalSymbolTable::totalCount++;
    globalSymbolTable foo;
    foo.name = variable;
    foo.type = variableType;
    foo.kind = variableKind;
    
    if(variableKind == "static"){
        foo.number = globalSymbolTable::staticCount;
        globalSymbolTable::staticCount++;
    }
    
    else{
        foo.number = globalSymbolTable::fieldCount;
        globalSymbolTable::fieldCount++;
    }
    
    if(table.count(variable) == 1){
        ofstream fout("err.txt");
        fout<<"Multiple declaration of variable : "<<variable;
        std::cout<<"compilation error\n";
        fout.close();
        std::exit(0); 
    }
    table[variable] = foo;
    //cout<<table[variable].name<<" "<<table[variable].number<<endl;
    tno++;
    while(Token[tno].data == ","){
        tno++;
        variable = Token[tno].data;
        globalSymbolTable::totalCount++;
        globalSymbolTable bar;
        bar.name = variable;
        bar.type = variableType;
        bar.kind = variableKind;
        
        if(variableKind == "static"){
            bar.number = globalSymbolTable::staticCount;
            globalSymbolTable::staticCount++;
        }
        
        else{
            bar.number = globalSymbolTable::fieldCount;
            globalSymbolTable::fieldCount++;
        }
        
        if(table.count(variable) == 1){
            ofstream fout("err.txt");
            fout<<"Multiple declaration of variable : "<<variable;
            std::cout<<"compilation error\n";
            fout.close();
            std::exit(0); 
        }
        table[variable] = bar;
        //cout<<table[variable].name<<" "<<table[variable].number<<endl;
       std::cout<<"exiting: classVarDec "<<Token[tno].data<<" "<<tno<<endl;
        tno++;
    }
    //currently reading a ;
    tno++; //at </classVarDec>
    tno++; //next token;
}


void compileSubroutineDec(){
   std::cout<<"entered SubroutineDec\n";
    tno++; //going past <subroutineDec>
    //<keyword> constructor
    subroutineTable.clear();
    subroutineSymbolTable:: argumentCount = 0;
    subroutineSymbolTable:: totalCount = 0;
    subroutineSymbolTable:: localCount = 0;

    currentSubroutineType = Token[tno].data;
    tno++;

    string subReturn = Token[tno].data; //could contain a className
    tno++;

    currentSubroutineName = Token[tno].data;
    tno++;

    if(currentSubroutineType == "method"){
        subroutineSymbolTable foo;
        foo.name = "this";
        foo.type = currentClassName;
        foo.kind = "argument";
        foo.number = 0;
        subroutineTable["this"] = foo;
        subroutineSymbolTable:: argumentCount++;
    }
    
    tno++; //advancing past the (
    compileParameterList();
    tno++; //advancing past the )
    compileSubroutineBody();
   std::cout<<"exiting: SubroutineDec "<<Token[tno].data<<" "<<tno<<endl;
    tno++;
}



void compileClass(){
   std::cout<<"Entered compileClass\n";
    tno++; //skips <class>
    tno++; //skips <keyword> class
    currentClassName = Token[tno].data;// <identifier> className
    tno++; //{
    tno++; //skips the { 
    while(Token[tno].data == "classVarDec"){
        compileClassVarDec();
    }
    
    while(Token[tno].data == "subroutineDec"){
        compileSubroutineDec();
    }
    
    //end of program. Skipped checking for "}" because that is already done in the JackAnalyser
}


int main(int argc, char *argv[]){
    if(argc==0){
        cerr<<"Error:Filename not provided\n"; return 0;
    }

    for(int i = 1; i < argc; i++){
        filename = argv[i];
        outputfile = filename + ".vm";
        Token.clear();
        tno = 0;
        makeTokens();
        compileClass();
        ofstream fout(outputfile);
        fout<<ss;
        fout.close();
    }


    return 0;
}