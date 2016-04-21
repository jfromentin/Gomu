#include "../../interpreter.hpp"
#include "array.hpp"
#include "string.hpp"
#include "module.hpp"

using namespace std;

Value type(Context&,Value& v){
  if(v.type==type_symbol) return Value(type_type,((Value*)v.ptr)->type); 
  else return Value(type_type,v.type);
}

Value del(Context&,Value& v){
  ((Value*)v.ptr)->del();  
  ((Value*)v.ptr)->type=type_void;
  return Value(type_void,nullptr);
}



 Value assignment(Context& context,Value& lhs,Value& rhs){
    Value* rhse=rhs.eval();
    if(lhs.ptr!=nullptr){
      //Prevent auto assignement
      if(((Value*)lhs.ptr)->ptr==rhse->ptr) return rhs;
      Symbol* symbol=(Symbol*)lhs.ptr;
      if(symbol->locked) ContextError("The symbol is locked");
      ((Value*)lhs.ptr)->del();
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

extern "C"{
  
  Gomu::Type types[]={
    TYPE_SENTINEL
  };

 
  //--- Functions ---//
  Gomu::Module::Function functions[]={
    {"Integer","len",{"String"},(void*)string_len},
    //{"Integer","len",{"Array"},(void*)array_len},
    FUNC_SENTINEL
  };
  
  //--- Member functions ---//
  Gomu::Module::Function member_functions[]={
    {"Integer","len",{"String"},(void*)string_len},
    {"Array","types",{"Module"},(void*)module_types},
    FUNC_SENTINEL
  };
  
  //--- Meta functions ---//
  Gomu::Module::Function contextual_functions[]={
    {"Type","type",{"Generic"},(void*)type},
    {"Generic","operator=",{"Symbol","Generic"},(void*)assignment},
    {"Void","delete",{"Symbol"},(void*)del},
    {"Array","symbols",{"Type"},(void*)symbols},
    FUNC_SENTINEL
  };
}

