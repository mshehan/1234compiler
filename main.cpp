/*
Matthew Shehan
Assignment 2
5/2/17
mshehan@ucsc.edu
*/
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <libgen.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <regex>
#include <algorithm>

#include "auxlib.h"
#include "string_set.h"
#include "lyutils.h"
#include "astree.h"

void print_usage();
bool file_exists (const std::string& name);
int check_exec(int argc,char** argv);
string change_file_ext(char* file_name,string ext);
FILE* tokfile; 
const string CPP = "/usr/bin/cpp -nostdinc";


int main(int argc, char* argv[]){
  int STATUS = EXIT_SUCCESS;
  yy_flex_debug = 0;
  yydebug = 0; 
  int option = 0; //stores the options from the command line
  bool darg_flag = false;
  string debug_flags = "", darg = ""; //used to store the values of args that take params
  // grabbing options from the command line
  while ((option = getopt(argc, argv,"ly@:D:")) != -1) {
    switch (option) {
       case 'l' :
           yy_flex_debug = 1;
           DEBUGF('y',"yy_flex_debug output turned on");   
           break;
       case 'y' :
           yydebug = 1; 
           break;
       case '@' :
           set_debugflags(optarg);
           break;
       case 'D' :  
           darg = string(optarg);
           darg_flag = true;
           break;
       default: 
           print_usage();
           STATUS = EXIT_FAILURE;
           exit(STATUS);
    }
  }
  // makes sure that there is a file with .oc that exists
  int which_arg = check_exec(argc,argv); 
  // make sure that if there are no .oc files then won't run
  if(which_arg != -1 && !file_exists(argv[which_arg])){
    print_usage();
  }  
  // done reading command line. CPP call being made.

    char* filename = basename(argv[which_arg]); // oc file name
    string command = ""; 
    //sets the darg flag and passes it to the C preprocessor
    //if one needs to be passed
    if(darg_flag){
      command += CPP + " -D" + darg + " " + filename;
    }else{
      command += CPP + " " + filename;  
    }
    DEBUGF ('a',"command=\"%s\"\n", command.c_str());
    
    yyin = popen(command.c_str(), "r");
    
    string tok_file = change_file_ext(filename,"tok"); // add ext tok to filename
    string str_file = change_file_ext(filename,"str"); 
    string ast_file = change_file_ext(filename, "ast"); 
    tokfile = fopen(tok_file.c_str(), "w");
    int value = yyparse();
    if(value == 1){
      cerr << "yyparse error\n";
      exit(EXIT_FAILURE);
    }
    
    FILE* ast_dump = fopen(ast_file.c_str(), "w");
    FILE* str_dump = fopen(str_file.c_str(), "w");
    string_set::dump(str_dump);
    astree::print(ast_dump,yyparse_astree,0); 

  return STATUS; 
}

void print_usage(){
  cerr << "\noc [-ly][-@flag...][-D string] <program>.oc\n"; 
  exit(EXIT_FAILURE); 
}

int check_exec(int argc,char** argv){
  std::regex oc_match(".+\\.oc");
  int which_arg = -1;
  for(int i = 0; i < argc; i++){
    if((std::regex_match(argv[i],oc_match))){
      which_arg = i;
      break;
    }
  } 
  return which_arg; 
}
//tells wheather file exists before trying to read from it. 
bool file_exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

string change_file_ext(char* file_name,string ext){
  string str_file = string(file_name);
  str_file.replace(str_file.end()-2,str_file.end(),ext);
  return str_file; 
}
