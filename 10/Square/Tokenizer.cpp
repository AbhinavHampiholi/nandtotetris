/*
    Author: ABHINAV ARUN HAMPIHOLI
    Last Modified: 4th November 2019
    Comments: This is a tokenizer, that takes as input multiple .jack file and spits out a T.xml file which 
    contains all the lexical elements of the .jack file.
*/

#include<iostream>
#include<regex>
#include<fstream>
#include<string>
#include <algorithm>
#include <iterator>
#include <cctype>

using namespace std;

string filename; //global variable containing name of the .jackfile
vector <string> Keywords;

vector <string> Symbols;

/*
functions to be written 
1.remove comments
2.replace every symbol with <space> symbol <space>
3.remove extra whitespaces
4.process "word by word" and output onto filename.xml
*/


void removeSingle(){ //removes single comments and stores output into pass1.txt
    //cout<<"In remove single\n";
    string line;
    ifstream assem ("pass1.txt");
    ofstream pass1 ("pass2.txt");
    
        while(getline(assem, line, '\n')){
        
        if(line.size()==0) continue;

        if(line.size()<2){pass1<<line<<endl;continue;}

        if((line[0]=='/' && line[1]=='/')){
        continue;
        //pass1<<line<<endl;
        }
        line = line.substr(0,line.find("//"));
        line = std::regex_replace(line, std::regex(" +$"), "");
        pass1<<line<<endl;

    }
    assem.close();
    pass1.close();  
}

//removes multiline comments and stores output into pass2.txt
void removeMulti(){
    //cout<<"In remove Multi\n";
    ifstream pass1("Nocarriage.txt");
    std::string content( (std::istreambuf_iterator<char>(pass1) ), (std::istreambuf_iterator<char>()    ) );
    
    int copen, cclose;

    copen = content.find("/*");
    cclose = content.find("*/");
    while(copen!=-1){
        //cout<<content[copen];
        content.erase((copen), (cclose-copen+1));
        //cout<<"AfterErase:"<<content[copen];
        content.erase(copen,1);
        copen = content.find("/*");
        cclose = content.find("*/");
        //cout<<"Current content:"<<content;
    }
    pass1.close();
    ofstream fout("pass1.txt");
    //content = ReplaceString(content, " ", "");
    content = std::regex_replace(content, std::regex("^(?:[\t ]*(?:\r?\n|\r))+"), "");
    fout<<content;
    fout.close();
    

}

void addwhitespaces(){
    char ch;
    ifstream fin("pass2.txt");
    ofstream fout("pass3.txt");
    bool quote = false;
    string dq;
    dq.push_back('"');
    while (fin >> noskipws >> ch) {
        string c;
        c +=ch;
        if(c==dq){
            quote = !quote;
        }

        if(find(Symbols.begin(), Symbols.end(), c)!=Symbols.end() && quote == false){
            fout<<" "<<c<<" "; continue;
        }
        
        else{
            fout<<ch;
        }
    }

    fin.close();
    fout.close();
}

void removeextraws(){
    ifstream fin ("pass3.txt");
    ofstream fout ("pass4.txt");
    string line;
    string out;
    char ch;
    bool space = false;
    while (fin >> noskipws >> ch){
        if(ch==' '&& space == true){
            continue;
        }
        else{
            if(ch==' ') space = true;
            else space = false;
            fout<<ch;
        }
    }

    fin.close();
    fout.close();
}

void removeblanklines(){
    ifstream fin("pass4.txt");
    ofstream fout ("pass5.txt");
    string line;
    while(getline(fin, line, '\n')){
        //line = std::regex_replace(line, std::regex(" +$"), "");
        size_t p = line.find_first_not_of("\t");
        line.erase(0, p);
        p = line.find_last_not_of("\t");
        //cout<<"here";
        if (string::npos != p)
        line.erase(p+1);
        
        line = std::regex_replace(line, std::regex("^ +| +$|( ) +"), "$1");
        //line = std::regex_replace(line, std::regex("^ +"), "");
        //cout<<line;
        //line = regex_replace(line, std::regex(" +"), " ");
        if(line!="\n" && line[0]!='\t') fout<<line<<"\n";
        if(line[0]=='\t') fout<<line.substr(1);

    }
    fin.close();
    fout.close();
}

string processstring(string str){
    if(find(Keywords.begin(), Keywords.end(), str)!=Keywords.end()){
        return "<keyword> " + str + " </keyword>\n";
    }
    if(find(Symbols.begin(), Symbols.end(), str)!=Symbols.end()){
        if(str=="&") str = "&amp;";
        if(str == "<") str = "&lt;";
        if(str == ">") str = "&gt;";
        
        return "<symbol> " + str + " </symbol>\n";
    }
    if(isdigit(str[0])){
        return "<integerConstant> " + str + " </integerConstant>\n";
    }
    if(str[0]=='"'){
        return "<stringConstant> " + str.substr(1, str.size() - 2) + " </stringConstant>\n";
    }
    else{
        return "<identifier> " + str + " </identifier>\n";
    }

}

void writexml(){
    ifstream fin("pass6.txt");
    string outputfile = filename + ".xml";
    ofstream fout(outputfile);
    string tobeoutput;
    string line;
    char dq = '"';
    fout<<"<tokens>\n";
    while (getline(fin, line, ' ')) {
        //cout<<"line to be processed:"<<line<<"e"<<endl;
        //cout<<"here";
        //if(line=="\n" || line == "\t") continue;
        //if(find(line.begin(), line.end(), '\n')!=line.end()) continue;
        if(line[0]!=dq){
            fout<<processstring(line);//to be written
        }
        else{
            line+=" ";
            string getmore;
            while(getline(fin, getmore, ' ') && getmore[getmore.size()-1]!=dq){
                line+= getmore + " ";
            }
            line+=getmore;
            fout<<processstring(line);
        }
    }
    fout<<"</tokens>\n";
    fin.close();
    fout.close();

}

void DeleteEmptyLines(const std::string &FilePath)
{
    std::ifstream inp(FilePath);
    std::string line, text;
    while (std::getline(inp, line))
        if (!(line.empty() || line.find_first_not_of(' ') == std::string::npos))
            text += line + "\n";
    inp.close();
    std::ofstream out(FilePath);
    out << text;
}

void replaceallnewlines(){
    ifstream fin ("pass5.txt");
    ofstream fout ("pass6.txt");
    cout<<"replacing\n";
    char ch;
    while (fin >> noskipws >> ch){
        if(ch=='\n') fout<<' ';
        else fout<<ch;
    }
    fin.close();
    fout.close();
}

void removecr(){
    ifstream fin (filename);
    ofstream fout ("Nocarriage.txt");
    string line;
    string out;
    char ch;
    bool space = false;
    while (fin >> noskipws >> ch){
       if(ch!='\r') fout<<ch;
    }

    fin.close();
    fout.close();
}

//= {"int", "boolean", "char", "void"
//"var", "static", "field", "let" , "do" , "if" , "else" , "while" , "return" , "true", "false", "null",
//"this", "class", "constructor", "method", "function"};

void pushkeyandsym(){
    Keywords.push_back("int");
    Keywords.push_back("boolean");
    Keywords.push_back("char");
    Keywords.push_back("void");
    Keywords.push_back("var");
    Keywords.push_back("static");
    Keywords.push_back("field");
    Keywords.push_back("let");
    Keywords.push_back("do");
    Keywords.push_back("if");
    Keywords.push_back("else");
    Keywords.push_back("while");
    Keywords.push_back("return");
    Keywords.push_back("true");
    Keywords.push_back("false");
    Keywords.push_back("null");
    Keywords.push_back("this");
    Keywords.push_back("that");
    Keywords.push_back("class");
    Keywords.push_back("constructor");
    Keywords.push_back("method");
    Keywords.push_back("function");



    Symbols.push_back("(");
    Symbols.push_back(")");
    Symbols.push_back("[");
    Symbols.push_back("]");
    Symbols.push_back("{");
    Symbols.push_back("}");
    Symbols.push_back(",");
    Symbols.push_back(";");
    Symbols.push_back("=");
    Symbols.push_back(".");
    Symbols.push_back("+");
    Symbols.push_back("-");
    Symbols.push_back("*");
    Symbols.push_back("/");
    Symbols.push_back("&");
    Symbols.push_back("|");
    Symbols.push_back("<");
    Symbols.push_back(">");
    Symbols.push_back("~");



}

int main(int argc, char *argv[]){
    
    if(argc==0){
        cerr<<"Error:Filename not provided\n"; return 0;
    }

    for(int i = 1; i < argc; i++){

    filename = argv[i];
    pushkeyandsym();
    removecr();
    removeMulti();
    removeSingle();
    //DeleteEmptyLines("pass2.txt");
    addwhitespaces();
    removeextraws();
    removeblanklines();
    DeleteEmptyLines("pass5.txt");
    replaceallnewlines();
    writexml();
    remove("pass1.txt");
    remove("pass2.txt");
    remove("pass3.txt");
    remove("pass4.txt");
    remove("pass5.txt");
    remove("pass6.txt");
    remove("Nocarriage.txt");
    }
    return 0;
}
