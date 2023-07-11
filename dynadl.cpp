/*
 * ExprInterface.cpp
 *
 *  Created on: Jan 4, 2022
 *      Author: anton
 */


#include <iostream>
#include <fstream>
#include "antlr4-runtime.h"
#include <tree/ParseTree.h>
#include "dynadlLexer.h"
#include "dynadlParser.h"
#include "dynadlListener.h"
#include "dynadlBaseListener.h"
#include "dynadlWalkListener.h"
#include "dataTypes.h"
using namespace antlrcpp;
using namespace antlr4;
using namespace std;


int usage(int ac){
    cout << endl << "Unexpected number or type of input parameters : " << ac << endl;
    cout << "Example usage: ./dynadl sample.adl {--generate | --runtime} {posix | ros | px4}" << endl;
    exit(-1);
}

void print_logo(void){
   cout << endl; 
   cout << "         _                           _____   _      " << endl;      
   cout << "        | |                   /\\    |  __ \\ | |     " << endl;     
   cout << "      __| | _   _  _ __      /  \\   | |  | || |     " << endl;     
   cout << "     / _` || | | || |_ \\    / /\\ \\  | |  | || |     " << endl;     
   cout << "    | (_| || |_| || | | |  / ____ \\ | |__| || |____ " << endl; 
   cout << "     \\__,_| \\__, ||_| | | /_/    \\_\\|_____/ |______|" << endl;
   cout << "             __/ |                             " << endl;
   cout << "            |___/                              " << endl;
   cout << endl;
}

int main(int argc, const char *argv[])
{
ifstream ins;
bool generate = false;
Platforms platform;
bool runtime = false;
bool ros = false;
bool posix = false;


if(argc != 4){
    usage(argc);
}

print_logo();
cout <<  "dynADL starting" << endl << endl;
cout << "Mode selected: "  << argv[2] << " for " << argv[3] << endl << endl;
if(!strcmp(argv[2],"--generate")){
    generate = true;
}
else if(!strcmp(argv[2],"--runtime")){
    runtime = true; 
}
else{
    usage(argc);
}

if(!strcmp(argv[3],"ros")){
    platform = ROS;
    cout << "ROS platform selected" << endl;
    if(runtime){
        cout << "ROS platform runtime is not supported yet." << endl;
        cout << "Only code generation is supported for ROS." << endl;
        cout << "Please try the --generate switch." << endl;
        exit(1);
    }
}
else if(!strcmp(argv[3],"px4")){
    platform = PX4;
    cout << "px4 is not supported at this time" << endl;
    exit(1);
}
else if(!strcmp(argv[3],"posix")){
    platform = POSIX;
    cout << "Posix platform selected" << endl;
}
else{
    usage(argc);
}

cout << endl;

// Create the input stream.
ins.open(argv[1]);
ANTLRInputStream input(ins);
// Create a lexer which scans the input stream
// to create a token stream.
dynadlLexer lexer(&input);
CommonTokenStream tokens(&lexer);
// Print the token stream.
/* cout << "Tokens:" << endl;
tokens.fill();
for (Token *token : tokens.getTokens())
{
std::cout << token->toString() << std::endl;
} */
// Create a parser which parses the token stream
// to create a parse tree.
dynadlParser parser(&tokens);
tree::ParseTree *tree = parser.prog();
// Print the parse tree in Lisp format.
//cout << endl << "Parse tree (Lisp format):" << endl;
//std::cout << tree->toStringTree(&parser) << endl;

if(generate){
   cout << "Generation mode" << endl;
}
else{
   cout << "Run-time mode" << endl;
}

dynadlWalkListener listener(platform);
tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);
//listener.printDefinitions();
//listener.printPackages();
//listener.printFunctions();
if(generate){
   listener.generate();
}
else if(runtime){
   /* Start from function maiin */ 
   listener.executeRuntimeScript(string("main"));
}

cout << argv[0]  << " Exiting" << endl << endl;
flush(cout);
return 0;
}
