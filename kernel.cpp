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

#include "kernel.hpp"
#include "interpreter.hpp"

namespace Gomu{

  //*********
  //* Array *
  //*********

  string
  array_disp(void* v){
    string str;
    ArrayValue* arr=(ArrayValue*)v;
    if(arr->size==0)
      return "[]";
    str="[";
    str+=arr->type->disp(arr->tab[0]);
    for(size_t i=1;i<arr->size;++i){
      str+=", ";
      str+=arr->type->disp(arr->tab[i]);
    }
    str+=']';
    return str;
  }

  void
  array_del(void* v){
    ArrayValue* arr=(ArrayValue*)v;
    Value val;
    val.type=arr->type;
    for(size_t i=0;i<arr->size;++i){
      val.ptr=arr->tab[i];
      val.pdel();
    }
    delete[] arr->tab;
    delete arr;
  }

  void*
  array_copy(void* v){
    ArrayValue* arr=(ArrayValue*)v;
    size_t size=arr->size;
    ArrayValue* res=new ArrayValue(size);
    Type* type=res->type=arr->type;
    res->tab=new void*[size];
    for(size_t i=0;i<size;++i){
      res->tab[i]=type->copy(arr->tab[i]);
    }
    return (void*)res;
  }

  int
  array_comp(void* v1,void* v2){
    ArrayValue* arr1=(ArrayValue*)v1;
    ArrayValue* arr2=(ArrayValue*)v2;
    size_t size1=arr1->size;
    size_t size2=arr2->size;
    if(size1!=0 and size2!=0){
      if(arr1->type!=arr2->type) RuntimeError("Types of array mismatch for comparison");
    }
    if(size1==size2){
      Type* type=arr1->type;
      for(size_t i=0;i<size1;++i){
	int c=type->comp(arr1->tab[i],arr2->tab[i]);
	if(c!=0) return c;
      }
      return 0;
    }
    if(size1<size2) return -1;
    return 1;
  }

  
  //***********
  //* Boolean *
  //***********

  string
  boolean_disp(void* v){
    if((*(char*)v)==1)
      return "\033[35mtrue\033[0m";
    else
      return "\033[35mfalse\033[0m";
   }

  int
  boolean_comp(void* u,void* v){
    char a=*(char*)u;
    char b=*(char*)v;
    if(a==b) return 0;
    if(a<b) return 1;
    return -1;
  }

  //**********************
  //* ContextualFunction *
  //**********************

  string
  contextual_function_disp(void* v){
    ContextualFunction* f=(ContextualFunction*)v;
    return disp_signature(f->signature)+" -> "+type_disp(f->tr);
  }
  
  //************
  //* Function *
  //************

  string
  function_disp(void* val){
    Function* function=(Function*)(val);
    return disp_signature(function->signature)+" -> "+type_disp(function->tr);
  }
  
  //***********
  //* Integer *
  //***********

  string
  integer_disp(void* v){
    char* disp=fmpz_get_str(NULL,10,(fmpz*)v);  
    string res="\033[34m";
    res+=disp;
    res+="\033[0m";
    free(disp);
    return res;
  }

  void*
  integer_copy(void* v){
    fmpz* res=new fmpz;
    fmpz_init_set(res,(fmpz*)v);
    return res;
  }

  //****************
  //* MetaFunction *
  //****************
  
   string
   meta_function_disp(void* v){
    MetaFunction* meta=(MetaFunction*)v;
    auto it=meta->functions.begin();
    string str=*it;
    for(++it;it!=meta->functions.end();++it){
      str+='\n';
      str+=*it;
    }
    return str;
  }

  //*******
  //* Set *
  //*******

  string
  set_disp(void* v){
    string str;
    SetValue* setval=(SetValue*)v;
    Type* type=setval->data.key_comp().type;
    if(setval->data.empty()) return "{}";
    str="{";
    auto it=setval->data.begin();
    str+=type->disp(*it);
    for(++it;it!=setval->data.end();++it){
      str+=',';
      str+=type->disp(*it);
    }
    str+='}';
    return str;
  }

  void
  set_del(void* v){
    SetValue* setval=(SetValue*)v;
    Type* type=setval->data.key_comp().type;
    for(auto it=setval->data.begin();it!=setval->data.end();++it){
      type->del(*it);
    }
    delete setval;
  }

  void*
  set_copy(void* v){
    SetValue* setval=(SetValue*)v;
    Type* type=setval->data.key_comp().type;
    SetValue* setres=new SetValue(type);
    for(auto it=setval->data.begin();it!=setval->data.end();++it){
      setres->data.insert(type->copy(*it));
    }
    return setres;
  }

  int
  set_comp(void* v1,void* v2){
    SetValue* set1=(SetValue*)v1;
    SetValue* set2=(SetValue*)v2;
    Type* type=set1->data.key_comp().type;
    if(type!=set2->data.key_comp().type) RuntimeError("Recurssive Types mismatch in set");
    if(set1->data.size()==set2->data.size()){
      auto it1=set1->data.begin();
      auto it2=set2->data.begin();
      for(;it1!=set1->data.end();++it1){
	int c=type->comp(*it1,*it2);
	if(c!=0) return c;
      }
      return 0;
    }
    else{
      if(set1->data.size()<set2->data.size()) return -1;
      return 1;
    }
  }

  //**********
  //* String *
  //**********

  int
  string_comp(void* u,void* v){
    string& a=*(string*)u;
    string& b=*(string*)v;
    if(a==b) return 0;
    if(a<b) return -1;
    return 1;
  }

  //*********
  //* Tuple *
  //*********

  string
  tuple_disp(void* v){
    TupleValue* t=(TupleValue*)v;
    if(t->size==0) return "()";
    string str="(";
    str+=t->tab[0].disp();
    for(size_t i=1;i<t->size;++i){
      str+=',';
      str+=t->tab[i].disp();
    }
    return str+')';
  }

  void
  tuple_del(void* v){
    TupleValue* t=(TupleValue*)v;
    for(size_t i=0;i<t->size;++i){
      t->tab[i].pdel();
    }
  }
  
  void*
  tuple_copy(void* v){
    TupleValue* t=(TupleValue*)v;
    TupleValue* res=new TupleValue(t->size);
    for(size_t i=0;i<t->size;++i){
      res->tab[i]=t->tab[i].copy();
    }
    return res;
  }

  int
  tuple_comp(void* v1,void *v2){
    TupleValue* t1=(TupleValue*)v1;
    TupleValue* t2=(TupleValue*)v2;
    if(t1->size==t2->size){
      for(size_t i=0;i<t1->size;++i){
	if(t1->tab[i].type==t2->tab[i].type){
	  Type* type=t1->tab[i].type;
	  int c=type->comp(t1->tab[i].ptr,t2->tab[i].ptr);
	  if(c!=0) return c;
	}
	else{
	  if(t1->tab[i].type->name<t2->tab[i].type->name) return -1;
	  return 1;
	}
      }
    }
    else{
      if(t1->size<t2->size) return -1;
      return 1;
    }
  }

  //********
  //* Type *
  //********
 
  int
  type_comp(void* T1,void* T2){
    string s1=((Type*)T1)->name;
    string s2=((Type*)T2)->name;
    return string_comp(&s1,&s2);
  }
  
  //***********************
  //* Auxialiry functions *
  //***********************

    //----------------------------------
  // disp_signature(const Signature&)
  //----------------------------------
  
  string
  disp_signature(const Signature& s){
    if(s.size()==0) return "()";
    string str="(";
    str+=type_disp(s[0]);
    for(uint32_t i=1;i<s.size();++i){
      str+=','+type_disp(s[i]);
    }
    return str+')';
  }
    
  //------------------------------------
  // init_kernel(Context&,Interpreter&)
  //------------------------------------
  
  void init_kernel(Context& context,Interpreter& interpreter){
    //Operators
    interpreter.add_operator("=","operator=",opBinary,15);
    interpreter.add_operator("<","operator<",opBinary,8);
    interpreter.add_operator(">","operator>",opBinary,8);
    interpreter.add_operator("<=","operator<=",opBinary,8);
    interpreter.add_operator(">=","operator>=",opBinary,8);
    interpreter.add_operator("==","operator==",opBinary,8);
    interpreter.add_operator("!=","operator!=",opBinary,8);
    interpreter.add_operator("+","operator+",opBinary,4);
    interpreter.add_operator("-","operator-",opBinary,4);
    interpreter.add_operator("*","operator*",opBinary,5);
    interpreter.add_operator("/","operator/",opBinary,5);
    interpreter.add_operator("%","operator%",opBinary,5);
    interpreter.add_operator("::","operator::",opBinary,1);
    interpreter.add_operator("!","factorial",opPostUnitary,2);
    context.add_contextual_function("Void","load",{"String"},(void*)load_module);
    context.add_contextual_function("Void","unload",{"Module"},(void*)unload);
    context.add_contextual_function("Void","reload",{"Module"},(void*)reload);
  }
  
  //-----------------------
  // load(Context&,Value&)
  //-----------------------
  
  Value
  load_module(Context& context,Value& v){
    context.load_module(*((string*)(v.eval()->ptr)));
    return Value(type_void,nullptr); 
  }

  //-------------------------
  // unload(Context&,Value&)
  //-------------------------
  
  Value
  unload(Context& context,Value& v){
    context.unload_module((Module*)(v.ptr));
    return Value(type_void,nullptr);
  }

  //-------------------------
  // reload(Context&,Value&)
  //-------------------------
  
  Value
  reload(Context& context,Value& v){
    context.reload_module((Module*)(v.ptr));
    return Value(type_void,nullptr);
  }
}


