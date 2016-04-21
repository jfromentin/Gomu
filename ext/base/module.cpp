#include "module.hpp"
#include "../../interpreter.hpp"

void* module_types(void* v){
  Gomu::Module* module=(Gomu::Module*)v;
  Gomu::ArrayValue *res=new Gomu::ArrayValue(module->ntype);
  res->type=Gomu::type_string;
  for(size_t i=0;i<module->ntype;++i){
    res->tab[i]=new string(module->types[i]);
  }
  return res;
}

Value symbols(Context& context,Value& v){
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
