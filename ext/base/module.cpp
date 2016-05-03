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

#include <fstream>
#include "module.hpp"
#include "../../interpreter.hpp"

using namespace Gomu;

//-----------------------------
// module_check(Context,Value)
//-----------------------------

Value module_check(Context& context,Value& value){
  Module* module=(Module*)value.ptr;
   string filename="ext/"+module->name+"/check";
   ifstream fs;
  fs.open(filename.c_str(),fstream::in);
  if(!fs) RuntimeError("File "+filename+" does not exist");
  string cmd;
  Value* v;
  bool error=false;
  size_t line=1;
  while(not error and getline(fs,cmd)){
    if(not cmd.empty() and cmd[0]!='#'){
      try{
	v=context.interpreter->eval_basic(cmd,context);
      }
      catch(Error err){
	error=true;
	context.interpreter->purge_tree();
	cout<<"[\033[34m"<<filename<<"\033[0m:\033[32m"<<line<<"\033[0m] ";
	err.disp(cout,cmd);
	cout<<endl;
	return Value(type_void,nullptr);
      }
      if(not error){
	if(v->type==type_boolean){
	  bool flag=*(bool*)v->ptr;
	  if(not flag){
	    cout<<"\033[31mfailed\033[0m line "<<line<<"\033[0m"<<endl;
	    return Value(type_void,nullptr);
	  }
	}
	v->pdel();
      }
    }
    ++line;
  }
  cout<<"\033[32mpassed\033[0m"<<endl;
  return Value(type_void,nullptr);
}

//---------------------
// module_types(void*)
//---------------------

void* module_types(void* v){
  Module* module=(Module*)v;
  ArrayValue *res=new ArrayValue(module->ntype);
  res->type=type_string;
  for(size_t i=0;i<module->ntype;++i){
    res->tab[i]=new string(module->types[i]);
  }
  return res;
}


