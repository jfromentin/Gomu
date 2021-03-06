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
#include "kernel.hpp"
#include "../../interpreter.hpp"

//------------------------------------
// assignment(Context&,Value&,Value&)
//------------------------------------

Value assignment(Context& context,Value& lhs,Value& rhs){
  Value* rhse=rhs.eval();
  if(lhs.ptr!=nullptr){
    //Prevent auto assignement
    if(((Value*)lhs.ptr)->ptr==rhse->ptr) return rhs;
    Symbol* symbol=(Symbol*)lhs.ptr;
    if(symbol->locked) ContextError("The symbol is locked");
    ((Value*)lhs.ptr)->pdel();
  }
  if(rhs.type==type_symbol){
    copyValue((Value*)lhs.ptr,rhse);
  }
  else{
    ((Value*)lhs.ptr)->type=rhs.type;
    ((Value*)lhs.ptr)->ptr=rhs.ptr;
    rhs.type=type_void;
    rhs.ptr=nullptr;
  }
  return lhs;
}

//----------------------
// del(Context&,Value&)
//----------------------

Value del(Context&,Value& v){
  ((Value*)v.ptr)->pdel();  
  ((Value*)v.ptr)->type=type_void;
  return Value(type_void,nullptr);
}

//--------------------------
// execute(Context&,Value&)
//--------------------------

Value execute(Context& context,Value& file){
  const string &filename="worksheet/"+*((string*)file.ptr);
  ifstream fs;
  fs.open(filename.c_str(),fstream::in);
  string cmd;
  size_t line=1;
  bool error=false;
  Value* res;
  while(not error and getline(fs,cmd)){
    try{
      res=context.interpreter->eval_basic(cmd,context);
    }
    catch(Error err){
      error=true;
      context.interpreter->purge_tree();
      cout<<"Line "<<line<<" : ";
      err.disp(cout,cmd);
      cout<<endl;
    }
    if(not error){
      res->pdel();
    }
  }
  fs.close();
  return Value(type_void,nullptr);

}

//---------
// symbols
//---------

Value member_symbols(Context& context,Value& v){
  Type* type=(Type*)v.ptr;
  string str=type->name+'.';
  deque<string> stack;
  auto it=context.symbols.lower_bound(str);
  while(it->first.substr(0,str.size())==str){
    stack.push_back(it->first);
    ++it;
  }
  ArrayValue* res=new ArrayValue(stack.size());
  res->type=type_string;
  for(size_t i=0;i<stack.size();++i){
    res->tab[i]=new string(stack[i]);
    }
  return Value(type_array,res);
}

//-----------------------
// type(Context&,Value&)
//-----------------------

Value type(Context&,Value& v){
  if(v.type==type_symbol) return Value(type_type,((Value*)v.ptr)->type); 
  else return Value(type_type,v.type);
}


//-------------------------------
// equal(Context&,Value&,Value&)
//-------------------------------

Value equality(Context&,Value& v1,Value& v2){
  Value res;
  res.type=type_boolean;
  Value* a=v1.eval();
  Value* b=v2.eval();
  if(a->type!=b->type){
    res.ptr=to_boolean(false);
    return res;
  }
  Type* type=a->type;
  res.ptr=to_boolean(type->comp(a->ptr,b->ptr)==0);
  return res;
}
