/**
 * This file is part of Gomu.
 *
 *  Copyright 2016 by Jean Fromentin <jean.fromentin@math.cnrs.fr>
 *
 * Gomu is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Gomu is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Gomu. If not, see <http://www.gnu.org/licenses/>. 
 */

#include <iostream>
#include <cstring>
#include <readline/readline.h>
#include <readline/history.h>
#include <pthread.h>
#include "interpreter.hpp"
 
using namespace std;
using namespace Gomu;
 
//! Context for completion
static Context* completion_context;

//! Interpreter for complerion
static Interpreter* completion_interpreter;

//! Position of completed word
static size_t completion_pos;

//! Completion functions called by readline
static char** completion(const char* str,int start,int end);
static char* completion_generator(const char* str,int state); 

//! Main function
int main(){
  Interpreter interpreter;
  Context context(&interpreter);
  completion_context=&context;
  completion_interpreter=&interpreter;
  try{
    init_kernel(context,interpreter);
    context.load_module("base");
  }
  catch(Error err){
    err.disp(cout,"");
  }
  rl_basic_word_break_characters=(char*)" .,;:()[]{}=+-*<>/#@%$!?";
  rl_completion_entry_function=completion_generator;
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
  completion_pos=pos;
  rl_completion_suppress_append=1;
  return rl_completion_matches(str,completion_generator);
}
 
static char* completion_generator(const char* str,int state){
    char* res=new char[1024];
    string comp;
    try{
      comp=completion_interpreter->complete(string(rl_line_buffer),string(str),completion_pos,state,*completion_context);
    }
    catch(Error& err){
      return nullptr;
    }
    if(comp.empty()){
        rl_filename_completion_desired=0;
        return nullptr;
    }
    return strcpy(res,comp.c_str());
}
