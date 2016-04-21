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

#include "module.hpp"

namespace Gomu{

  //**************
  //* ArrayValue *
  //**************
  
   ArrayValue::ArrayValue(size_t s){
    size=s;
    type=nullptr;
    if(s==0) tab=nullptr;
    else tab=new void*[size];
  }

  //*********
  //* Error *
  //*********
   
  void
  Error::disp(ostream& os,const string& cmd) const{
    switch(type){
      case errBug:
      os<<"\033[31mBug\033[0m : "<<msg;
      os<<" (\033[34m"<<file<<"\033[0m:\033[32m"<<line<<"\033[0m)";
      return;
    case errContext:
      os<<"\033[31mContext error\033[0m : "<<msg;
      os<<" (\033[34m"<<file<<"\033[0m:\033[32m"<<line<<"\033[0m)";
      return;
    case errRuntime:
      os<<"\033[31mRuntime error\033[0m : "<<msg;
      return;
    case errSyntax:
      os<<"\033[31mSyntax error\033[0m : "<<msg;
      os<<"(\033[34m"<<file<<"\033[0m:\033[32m"<<line<<"\033[0m)";
      break;
    default:
      os<<"\033[31mError\033[0m : ";
      break;
    }
    os<<endl<<"@ ";
    if(first>0) os<<cmd.substr(0,first);
    os<<"\033[32m"<<cmd.substr(first,last-first+1)<<"\033[0m";
    if(last+1<cmd.length()) os<<cmd.substr(last+1,string::npos);
    return;
  }
  
  //**********
  //* Module *
  //**********

  //------------------
  // Module::Module()
  //------------------
  
  Module::Module(){
    types=nullptr;
    functions=nullptr;
    member_functions=nullptr;
    contextual_functions=nullptr;
  }
  
  //---------------------
  // Module::init(Type*)
  //---------------------
  
  void Module::init(Type* types){
    size_t i=0;
    if(types==nullptr) ntype=0;
    else{
      while(types[i].disp!=nullptr) ++i;
      ntype=i;
      i=0;
    }
    if(functions==nullptr) nfunc=0;
    else{
      while(functions[i].ptr!=nullptr) ++i;
      nfunc=i;
      i=0;
    }
    if(member_functions==nullptr) nmfunc=0;
    else{
      while(member_functions[i].ptr!=nullptr) ++i;
      nmfunc=i;
      i=0;
    }
    if(contextual_functions==nullptr) ncfunc=0;
    else{
      while(contextual_functions[i].ptr!=nullptr) ++i;
      ncfunc=i;
    }
  }
  //********
  //* Type *
  //********
  
  //-------------------------------------------------------
  // Type::Type(string,DispFunc,DelFunc,CopyFunc,CompFunc)
  //-------------------------------------------------------
  
  Type::Type(string _name,DispFunc _disp,DelFunc _del,CopyFunc _copy,CompFunc _comp){
    name=_name;
    disp=_disp;
    del=_del;
    copy=_copy;
    comp=_comp;
  }

  //-------------------------
  // Type::Type(const Type&)
  //-------------------------
  
  Type::Type(const Type& t){
    name=t.name;
    disp=t.disp;
    del=t.del;
    copy=t.copy;
    comp=t.comp;
  }


  //**************
  //* TupleValue *
  //**************
  
  TupleValue::TupleValue(size_t s){
    size=s;
    tab=(s==0)?nullptr:new Value[s];
  }
   

  //*********
  //* Value *
  //*********

  //---------------
  // Value::copy()
  //---------------
  
  Value
  Value::copy(){
    Value res;
    res.type=type;
    res.ptr=type->copy(ptr);
    return res;
  }

  //---------------
  // Value::disp()
  //---------------
  
  string
  Value::disp(){
    if(type==type_symbol) return ((Value*)ptr)->disp();
    if(ptr!=nullptr and type!=nullptr) return type->disp(ptr);
    else return "";
  }

  //***********************
  //* Auxiliary functions *
  //***********************

  //------------------
  // get_slong(void*)
  //------------------
  
  uint64
  get_slong(void* v){
    fmpz* z=(fmpz*)v;
    if(fmpz_fits_si(z)) return fmpz_get_si(z);
    else  RuntimeError("Integer too huge to fit slong");
  }

  //------------------
  // to_boolean(bool)
  //------------------
  
  void*
  to_boolean(bool b){
    char* res=new char;
    *res=(b?1:0);
    return res;
  }

  //-------------------
  // to_integer(slong)
  //-------------------
  
  void*
  to_integer(slong n){
    fmpz* z=new fmpz;
    fmpz_init(z);
    fmpz_set_si(z,n);
    return z;
  }

}
