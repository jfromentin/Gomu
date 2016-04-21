#include <iostream>
#include <cstring>
#include <readline/readline.h>
#include <readline/history.h>
#include <pthread.h>
#include "interpreter.hpp"
//#include "modules/base.hpp"
//#include "modules/permutations.hpp"
//#include "modules/matrix.hpp"
//#include "modules/combinatorics.hpp"
//#include "maths/polynomial.hpp"
 
 
using namespace std;
using namespace Gomu;
 
static Context* completionContext;
static size_t completionPos;
static string completionString;
static Interpreter* completionInterpreter;
static char** completion(const char* str,int start,int end);
static char* completionGenerator(const char* str,int state); 
 
int main(){
  Context context;
  Interpreter interpreter;
  completionContext=&context;
  completionInterpreter=&interpreter;
  //  lexer.setContext(context);*/
  try{
    init_kernel(context,interpreter);
  }
  catch(Error err){
    err.disp(cout,"");
  }
  rl_basic_word_break_characters=(char*)" .,;:()[]{}=+-*<>/#@%$!?";
    // rl_completion_suppress_append=1;
     
    rl_completion_entry_function=completionGenerator;
    rl_filename_completion_desired=0;
    rl_attempted_completion_function = completion;
    string cmd;
    char* c_cmd;
    while((c_cmd = readline("> "))!=NULL){
        //enable autocomplete
        rl_bind_key('\t',rl_complete);
        cmd=c_cmd;
        free(c_cmd);
        if(cmd.compare("quit")==0)
            break;
	interpreter.eval(cmd,context);
        add_history(cmd.c_str());
    }
  return 0;
}
 
static char** completion(const char* str,int beg,int pos){
    completionPos=pos;
    //prevent adding extra character
    rl_completion_suppress_append=1;
    return rl_completion_matches(str,completionGenerator);
}
 
static char* completionGenerator(const char* str,int state){
    char* res=new char[1024];
    string comp;
    try{
      comp=completionInterpreter->complete(string(rl_line_buffer),string(str),completionPos,state,*completionContext);
    }
    catch(Error& err){
      return nullptr;
    }
    if(comp.empty()){
        rl_filename_completion_desired=0;
        //rl_bind_key('\t',rl_abort);
        return nullptr;
    }
    return strcpy(res,comp.c_str());
}
