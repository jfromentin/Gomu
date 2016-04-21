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

#include "interpreter.hpp"

namespace Gomu{

  //************************************************************
  //* Definition of fundamental types  declared in modules.hpp *
  //************************************************************
  
  Type *type_array=new Type("Array",array_disp,array_del,array_copy,array_comp);
  Type *type_boolean=new Type("Boolean",boolean_disp,boolean_del,boolean_copy,boolean_comp);
  Type *type_context=new Type("Context",context_disp,context_del,context_copy,context_comp);
  Type *type_generic=new Type("Generic",nullptr,nullptr,nullptr,nullptr);
  Type *type_integer=new Type("Integer",integer_disp,integer_del,integer_copy,integer_comp);
  Type *type_function=new Type("Function",function_disp,function_del,function_copy,function_comp);
  Type *type_contextual_function=new Type("ContextualFunction",contextual_function_disp,contextual_function_del,contextual_function_copy,contextual_function_comp);
  Type *type_meta_function=new Type("MetaFunction",meta_function_disp,meta_function_del,meta_function_copy,meta_function_comp);
  Type *type_module=new Type("Module",module_disp,module_del,module_copy,module_comp);
  Type *type_set=new Type("Set",set_disp,set_del,set_copy,set_comp);
  Type *type_string=new Type("String",string_disp,string_del,string_copy,string_comp);
  Type *type_symbol=new Type("Symbol",nullptr,nullptr,nullptr,nullptr);
  Type *type_tuple=new Type("Tuple",tuple_disp,tuple_del,tuple_copy,tuple_comp);
  Type *type_type=new Type("Type",type_disp,type_del,type_copy,type_comp);
  Type *type_void=new Type("Void",void_disp,void_del,void_copy,void_comp);
  
  //***********************
  //* Context definitions *
  //***********************

  //--------------------
  // Context::Context()
  //--------------------
  
  Context::Context(){
    add_symbol("context",type_context,this)->hide=true;
    add_symbol("Array",type_type,type_array);
    add_symbol("Boolean",type_type,type_boolean)->hide=true;
    add_symbol("Context",type_type,type_context)->hide=true;
    add_symbol("Generic",type_type,type_generic)->hide=true;
    add_symbol("Integer",type_type,type_integer)->hide=true;
    add_symbol("Function",type_type,type_function)->hide=true;
    add_symbol("ContextualFunction",type_type,type_contextual_function)->hide=true;
    add_symbol("Module",type_type,type_module);
    add_symbol("Set",type_type,type_set);
    add_symbol("String",type_type,type_string);
    add_symbol("Symbol",type_type,type_symbol)->hide=true;
    add_symbol("Tuple",type_type,type_tuple);
    add_symbol("Type",type_type,type_type)->hide=true;
    add_symbol("Void",type_type,type_void)->hide=true;
  }

  //--------------------------------------------------------
  // Context::add_function(string,string,string_list,void*)
  //--------------------------------------------------------

  void
  Context::add_function(string ret,string name,string_list args,void* ptr){
    Type* tr=get_type(ret);
    Signature signature=get_signature(args);  
    Symbol* symbol=get_symbol(name);
    if(symbol==nullptr or (symbol!=nullptr and
			   not symbol->locked and
			   symbol->type!=type_meta_function and
			   symbol->type!=type_function)){
      Function* function=new Function(tr,signature,ptr);
      add_symbol(name,type_function,function,true);
      return;
    }
    string fullname=function_fullname(name,args);
    if(symbol->type==type_meta_function){
      if(not can_add(fullname))
	ContextError("A locked symbol named "+fullname+" already exists");
      MetaFunction* meta_function=(MetaFunction*)symbol->ptr;
      meta_function->functions.insert(fullname);
      Function* function=new Function(tr,signature,ptr);
      add_symbol(fullname,type_function,function,true);
      return;      
    }
    if(symbol->type==type_function){
      Function* function=new Function(tr,signature,ptr);
      add_symbol(fullname,type_function,function,true);
      MetaFunction* meta_function=new MetaFunction;
      meta_function->functions.insert(fullname);
      function=(Function*)symbol->ptr;
      fullname=function_fullname(name,function->signature);
      add_symbol(fullname,type_function,function,true);
      meta_function->functions.insert(fullname);
      symbol->type=type_meta_function;
      symbol->ptr=(void*)meta_function;
      return;
    }
    ContextError("A locked symbol named "+name+" already exist");
  }

  //-------------------------------------------------------------
  // Context::add_member_function(string,string,string_list,ptr)
  //-------------------------------------------------------------

  void
  Context::add_member_function(string ret,string name,string_list args,void* ptr){
    if(args.size()==0) ContextError("Member function must have at least one argument");
    string cname=*args.begin();
    Type* ctype=get_type(cname);
    if(ctype==nullptr) ContextError("The class type "+cname+" does not exist.");
    add_function(ret,cname+"."+name,args,ptr);
  }

  //--------------------------------------------
  // Context::add_symbol(name,Type*,void*,bool)
  //--------------------------------------------
  
  Symbol*
  Context::add_symbol(string name,Type* type,void* ptr,bool lock){
    Symbol& s=symbols[name];
    if(s.locked) ContextError("A locked symbol named "+name+" already exist");
    s.del();s.type=type;
    s.ptr=ptr;s.locked=lock;
    s.name=name;
    s.hide=false;
    return &s;
  }

  //-----------------------------------------------
  // Context::can_add_function(string,string_list)
  //-----------------------------------------------
  
  bool
  Context::can_add_function(string name,string_list targs){
    if(can_add(name)) return true;
    if(not can_add(function_fullname(name,targs))) return false;
    Symbol* symbol=get_symbol(name);
    return symbol->type==type_meta_function or symbol->type==type_function;
  }

  //-----------------------------------------------
  // Context::can_add_member_function(string,string_list)
  //-----------------------------------------------
  
  bool
  Context::can_add_member_function(string name,string_list targs){
    if(targs.size()==0) ContextError("A member function must have at least one argument");
    string cname=*(targs.begin());
    Type* ctype=get_type(cname);
    if(ctype==nullptr) ContextError("The class type "+cname+" does not exist");
    return can_add(cname+"."+name);
  }

 
  //-----------------------------------------
  // Context::eval_array(size_t,Node&,Node*)
  //-----------------------------------------
  
  void 
  Context::eval_array(size_t size,Node& current, Node* nodes){
    ArrayValue* array;
    if(size==0) array=new ArrayValue(0);
    else{
      size_t j=current.son;
      int k=0;
      Type* type=nodes[j].value.eval()->type;
      if(type==nullptr or type==type_void)
	ContextError("Non empty Array of undefinded type is forbidden");
      array=new ArrayValue(size);
      array->type=type;
      while(j!=-1){
	Node* node=&nodes[j];
	Value* val=node->value.eval();
	//Test if the type is correct
	if(val->type!=type){
	  //No -> delete array
	  for(int i=0;i<k;++i) type->del(array->tab[i]);
	  delete[] array->tab;
	  delete array;
	  current.value.ptr=nullptr;
	  current.value.type=type_void;
	  ContextError("Elements of an Array must have same type");
	}
	//We cannot stole pointer of value for a symbol
	if(node->value.type==type_symbol){
	  array->tab[k++]=type->copy(val->ptr);
	}
	else{
	  array->tab[k++]=val->ptr;
	  val->type=type_void;
	  val->ptr=nullptr;
	}
	j=node->bro;
      }
    }
    current.value.ptr=array;
    current.value.type=type_array; 
  }

  //-----------------------------------------------------------------------
  // Context::eval_contextual_function(ContextualFunction*,Value**,size_t)
  //-----------------------------------------------------------------------
  
  Value
  Context::eval_contextual_function(ContextualFunction* func,Value** args,size_t nargs){
    const Signature& signature=func->signature;
    if(nargs!=func->signature.size())
      ContextError("The number of arguments is incorect ("+to_string(nargs)+" instead of "+to_string(func->signature.size())+").");
    for(size_t i=0;i<nargs;++i){
      if(args[i]->type==type_symbol and signature[i]!=type_symbol){
	args[i]=(Value*)args[i]->ptr;
      }
      if(signature[i]!=type_generic and signature[i]!=args[i]->type)
	ContextError("Argument "+to_string(i+1)+" type is incorrect ("+type_disp(args[i]->type)+" instead of "+type_disp(signature[i])+").");
    }
    return func->eval(args,nargs,*this);
  }

  //--------------------------------------------------
  // Context::eval_function(Function*,Value**,size_t)
  //--------------------------------------------------
  
  Value
  Context::eval_function(Function* function,Value** args,size_t nargs){
    if(nargs!=function->signature.size())
      ContextError("Number of argument is "+to_string(nargs)+" instead of "+to_string(function->signature.size()));
    Type* targ;
    for(size_t i=0;i<nargs;++i){
      if(args[i]->type==type_symbol){
	targ=((Value*)(args[i]->ptr))->type;
      }
      else{
	targ=args[i]->type;
      }
      if(targ!=function->signature[i])
	ContextError("Argument "+to_string(i)+" is of type "+type_disp(targ)+" instead of "+type_disp(function->signature[i]));
    }
    return function->eval(args,nargs);
  }

  //-------------------------------------
  // Context::eval_function(Node&,Node*)
  //-------------------------------------
  
  void
  Context::eval_function(Node& current,Node* nodes){
    string name=current.str;
    Symbol* symbol=get_symbol(name);
    if(symbol==nullptr)
      ContextError("There is no symbol named "+name);
    Value* args[max_arguments_number];
    size_t nargs=set_arguments(current,nodes,args);
    current.value=eval_function(symbol,args,nargs);
  }

  //------------------------------------------------
  // Context::eval_function(Symbol*,Value**,size_t)
  //------------------------------------------------
  
  Value
  Context::eval_function(Symbol* symbol,Value** args,size_t nargs){
    if(symbol->type==type_contextual_function){
      return eval_contextual_function((ContextualFunction*)symbol->ptr,args,nargs);
    }
    else if(symbol->type==type_function){
      return eval_function((Function*)symbol->ptr,args,nargs);
    }
    else if(symbol->type==type_meta_function){
      string fullname=function_fullname(symbol->name,args,nargs);
      Value *val=get_symbol(fullname);
      if(val==nullptr) ContextError("There is no function "+fullname);
      if(val->type!=type_function) Bug("This case shall be impossible!");
      return eval_function((Function*)val->ptr,args,nargs);
    }
    else{
      Bug("The symbol named "+symbol->name+" is not callable");
    }
  }

  //--------------------------------------------
  // Context::eval_member_function(Node&,Node*)
  //--------------------------------------------
  
  void
  Context::eval_member_function(Node& current,Node* nodes){
    Value* args[max_arguments_number];
    size_t nargs=set_arguments(current,nodes,args);
    Value* first=args[0]->eval();
    Type* type=first->type;
    Value* val=get_symbol(type,current.str);
    if(val==nullptr){
      if(type==type_type){
	type=(Type*)first->ptr;
	val=get_symbol(type,current.str);
	//current.son=nodes[current.son].bro;
	if(val==nullptr)
	  ContextError("No member function "+current.str+" for tyipe "+type->name);
	current.value=eval_function((Function*)val->ptr,&args[1],nargs-1);
	return;
      }
      else{
	ContextError("No member function "+current.str+" for type "+type->name);
      }
    }
    if(val->type!=type_function)
      ContextError("The symbol named "+current.str+" is not callable");
    current.value=eval_function((Function*)val->ptr,args,nargs);
  }

  //---------------------------------------
  // Context::eval_set(size_t,Node&,Node*)
  //---------------------------------------
  
  void 
  Context::eval_set(size_t size,Node& current, Node* nodes){
    SetValue* set;
    if(size==0) set=new SetValue(type_generic);
    else{
      size_t j=current.son;
      int k=0;
      Type* type=nodes[j].value.eval()->type;
      if(type==nullptr or type==type_void)
	ContextError("Non empty Set of undefinded type is forbidden");
      set=new SetValue(type);
      while(j!=-1){
	Node* node=&nodes[j];
	Value* val=node->value.eval();
	//Test if the type is correct
	if(val->type!=type){
	  //No -> delete set
	  for(auto it=set->data.begin();it!=set->data.end();++it) type->del(*it);
	  delete set;
	  current.value.ptr=nullptr;
	  current.value.type=type_void;
	  ContextError("Elements of a Set must have same type");
	}
	//Check if the value is not in the set otherwise do nothing
	if(set->data.find(val->ptr)==set->data.end()){
	  if(node->value.type==type_symbol){
	    //We cannot stole pointer of value for a symbol
	    set->data.insert(type->copy(val->ptr));
	  }
	  else{
	    set->data.insert(val->ptr);
	    val->type=type_void;
	    val->ptr=nullptr;
	  }
	}
	j=node->bro;
      }
    }
    current.value.ptr=set;
    current.value.type=type_set;
  }

  //-----------------------------------------
  // Context::eval_tuple(size_t,Node&,Node*)
  //-----------------------------------------

  void 
  Context::eval_tuple(size_t size,Node& current, Node* nodes){
    TupleValue* tuple;
    if(size==0) tuple=new TupleValue(0);
    else{
      size_t j=current.son;
      int k=0;
      tuple=new TupleValue(size);
      while(j!=-1){
	Node* node=&nodes[j];
	Value* val=node->value.eval();
	//We cannot stole pointer of value for a symbol
	if(node->value.type==type_symbol){
	  tuple->tab[k++]=val->copy();
	}
	else{
	  tuple->tab[k++]=*val;
	  val->type=type_void;
	  val->ptr=nullptr;
	}
	j=node->bro;
      }
    }
    current.value.ptr=tuple;
    current.value.type=type_tuple;
  }

  //-------------------------------------
  // Context::get_signature(string_list)
  //-------------------------------------
  
  Signature
  Context::get_signature(string_list targs){
    Signature s(targs.size());
    size_t i=0;
    for(auto its=targs.begin();its!=targs.end();++its) s[i++]=get_type(*its);
    return s;
  }

  //---------------------------
  // Context::get_symbol(name)
  //---------------------------
  
  Symbol*
  Context::get_symbol(string name){
    auto it=symbols.find(name);
    if(it==symbols.end()) return nullptr;
    return &it->second;
  }

  //-------------------------
  // Context::get_type(name)
  //-------------------------
  
  Type*
  Context::get_type(string name){
    Value* v=get_symbol(name);
    if(v==nullptr) ContextError("Type "+name+" does not exist");
    if(v->type!=type_type) ContextError(name+" does not design a type");
    return (Type*)v->ptr;
  }

  //----------------------------
  // Context::load_module(name)
  //----------------------------
  
  void
  Context::load_module(string name){
    string filename="ext/"+name+".so";
    Symbol* symbol=get_symbol(name);
    if(symbol!=nullptr and symbol->locked==true) ContextError("A locked symbol "+name+" already exist");
    void* handle=dlopen(filename.c_str(),RTLD_NOW);
    if(not handle) RuntimeError("In loading module "+name+" : "+dlerror());
    Module* module=new Module;
    symbol=add_symbol(name,type_module,module);
    module->handle=handle;
    module->name=name;
    //Loading types
    Type* types=(Type*)dlsym(handle,"types");
    module->functions=(Module::Function*)dlsym(handle,"functions");
    module->member_functions=(Module::Function*)dlsym(handle,"member_functions");
    module->contextual_functions=(Module::Function*)dlsym(handle,"contextual_functions");
    module->init(types);
    //Test if we can load all types
    for(size_t i=0;i<module->ntype;++i){
      if(not can_add(types[i].name)){
	dlclose(handle);
	delete(module);
	ContextError("The type "+name+" cannot be added because a locked symbol has the same name");
      }
    }
    module->types=new string[module->ntype];
    for(size_t i=0;i<module->ntype;++i){
      Type* type=new Type(types[i]);
      module->types[i]=type->name;
      add_symbol(type->name,type_type,type);
    }
    load_module_functions(module,0);
    load_module_functions(module,1);
    load_module_functions(module,2);
  }

  //---------------------------------------------
  // Context::load_module_functions(Module*,int)
  //---------------------------------------------
  
  void Context::load_module_functions(Module* module,int src){
    bool (Context::*can_add_func)(string,string_list);
    void (Context::*add_func)(string,string,string_list,void*);
    Module::Function* module_func_ptr;
    size_t n;
    switch(src){
    case 0:
      can_add_func=&Context::can_add_function;
      add_func=&Context::add_function;
      module_func_ptr=module->functions;
      n=module->nfunc;
      break;
    case 1:
      can_add_func=&Context::can_add_member_function;
      add_func=&Context::add_member_function;
      module_func_ptr=module->member_functions;
      n=module->nmfunc;
      break;
    case 2:
      can_add_func=&Context::can_add_contextual_function;
      add_func=&Context::add_contextual_function;
      module_func_ptr=module->contextual_functions;
      n=module->ncfunc;
      break;
    default:
      Bug("Not yet implemented");
    }
    for(size_t i=0;i<n;++i){
      Module::Function& function=module_func_ptr[i];
      try{
	function.loaded=(this->*can_add_func)(function.name,function.targs);
      }
      catch(Error& err){
	err.disp(cout,"");
	function.loaded=false;
      }
      if(function.loaded){
	(this->*add_func)(function.tr,function.name,function.targs,function.ptr);
      }
      else{
	cout<<"Warning : ";
	if(src==0) cout<<"Function";
	else if(src==1) cout<<"Member function";
	else cout<<"Contextual function";
	cout<<" "<<function.name<<" not loaded"<<endl;
      }
    }
  }

  //----------------------------------------------
  // Context::unload_function(string,string_list)
  //----------------------------------------------

  void
  Context::unload_function(string name,string_list args){
    string fullname=function_fullname(name,args);
    unload_symbol(fullname);
    Symbol* symbol=get_symbol(name);
    if(symbol==nullptr) Bug("The meta function "+name+" does not exist");
    if(symbol->type==type_function){
      unload_symbol(name);
      return;
    }
    if(symbol->type!=type_meta_function) Bug("The symbol "+name+" is not of type Function or MetaFunction");
    MetaFunction* meta=(MetaFunction*)(symbol->ptr);
    meta->functions.erase(fullname);
    if(meta->functions.empty()) //There is no more function called name
      unload_symbol(name);
  }

  //---------------------------------
  // Context::unload_module(Module*)
  //---------------------------------
  
  void
  Context::unload_module(Module* module){
    unload_module_functions(module);
    for(size_t i=0;i<module->ntype;++i){
      unload_type(get_type(module->types[i]));
    }
    dlclose(module->handle);
    unload_symbol(module->name);
  }

  //-------------------------------------------
  // Context::unload_module_functions(Module*)
  //-------------------------------------------
  
  void
  Context::unload_module_functions(Module* module){
    for(size_t i=0;i<module->nfunc;++i){
      if(module->functions[i].loaded)
	unload_function(module->functions[i].name,module->functions[i].targs);
    }
    for(size_t i=0;i<module->nmfunc;++i){
      Module::Function& func=module->member_functions[i];
      if(func.loaded)
	unload_function(*func.targs.begin()+"."+func.name,func.targs);
    }
    for(size_t i=0;i<module->ncfunc;++i){
      if(module->contextual_functions[i].loaded)
	unload_symbol(module->contextual_functions[i].name);
    }
  }

  //-----------------------------
  // Context::unload_type(Type*)
  //-----------------------------
  
  void
  Context::unload_type(Type* type){
    for(auto it=symbols.begin();it!=symbols.end();++it){
      if(it->second.type==type)
	unload_symbol(it->first);
    }
    unload_symbol(type->name);
  }

  //--------------------------
  // Context::reload(Module*)
  //--------------------------
  
  void
  Context::reload_module(Module* module){
    //** Unload some module member
    unload_module_functions(module); //unload functions
    if(module->handle!=nullptr){
      dlclose(module->handle); //close previous module handle
      module->handle=nullptr;
    }
    //we postpone the unload of module->types
    
    //** Load the new version of the library
    string filename="ext/"+module->name+".so";
    void* handle=dlopen(filename.c_str(),RTLD_NOW);
    if(not handle){
      handle=nullptr;
      RuntimeError("In loading module "+module->name+" : "+dlerror());
    }
    Module nmod; //A temporary module for the newly loaded data
    
    Type* types=(Type*)dlsym(handle,"types"); 
    nmod.functions=(Module::Function*)dlsym(handle,"functions");
    nmod.member_functions=(Module::Function*)dlsym(handle,"member_functions");
    nmod.contextual_functions=(Module::Function*)dlsym(handle,"contextual_functions");
    nmod.init(types);//init the module, ie, compute size of arrays
    
    //Test if we can load the new types
    int link[nmod.ntype];
    for(size_t i=0;i<nmod.ntype;++i){
      string name=types[i].name;
      link[i]=-1;
      if(not can_add(name)){
	for(size_t j=0;j<module->ntype;++j){
	  if(module->types[j]==name){
	    link[i]=j;
	    break;
	  }
	}
	if(link[i]==-1){//We can't load a type
	  //close the library
	  dlclose(handle);
	  ContextError("The type "+name+" cannot be added because a locked symbol has the same name");
	}
      }
    }

    //Look for old types to unload, ie, types in the old version of the library
    //that are not present in the new version
    bool to_unload[module->ntype];
    for(size_t i=0;i<module->ntype;++i){
      to_unload[i]=true;
      for(size_t j=0;j<nmod.ntype;++j){
	if(link[j]==i){
	  to_unload[i]=false;
	  break;
	}
      }
    }
    
    //Unload marked types
    for(size_t i=0;i<module->ntype;++i){
      if(to_unload[i]) unload_type(get_type(module->types[i]));
    }
    
    //Update or load the types in the new version of the library
    delete[] module->types; //unload types array
    module->types=new string[nmod.ntype];
    for(size_t i=0;i<nmod.ntype;++i){
      Type& nmod_type=types[i];
      module->types[i]=nmod_type.name;
      if(link[i]==-1){
	Type *type=new Type(nmod_type);
	add_symbol(type->name,type_type,type);
      }
      else{
	Type* type=get_type(nmod_type.name);
	type->disp=nmod_type.disp;
	type->del=nmod_type.del;
	type->copy=nmod_type.copy;
	type->comp=nmod_type.comp;
      }
    }
    module->ntype=nmod.ntype;
    module->functions=nmod.functions;
    module->nfunc=nmod.nfunc;
    module->member_functions=nmod.member_functions;
    module->nmfunc=nmod.nmfunc;
    module->contextual_functions=nmod.contextual_functions;
    module->ncfunc=nmod.ncfunc;
    module->handle=handle;
    load_module_functions(module,0);
    load_module_functions(module,1);
    load_module_functions(module,2);
  }

  //---------------------------------------------
  // Context::set_arguments(Node&,Node*,Value**)
  //---------------------------------------------
  
  size_t
  Context::set_arguments(Node& current,Node* nodes,Value** args){
    size_t j=current.son;
    size_t nargs=0;
    while(j!=-1){
      args[nargs++]=&nodes[j].value;
      j=nodes[j].bro;
    }
    return nargs;
  }

  //**********************
  //* ContextualFunction *
  //**********************

  //---------------------------------------------------
  // ContextualFunction::eval(Value**,size_t,Context&)
  //---------------------------------------------------
  
  Value ContextualFunction::eval(Value* args[8],size_t nargs,Context& context){
    switch(nargs){
    case 0:
      return (*((CFunc0)ptr))(context);
    case 1:
      return (*((CFunc1)ptr))(context,*args[0]);
    case 2:
      return (*((CFunc2)ptr))(context,*args[0],*args[1]);
    default:
      Bug("Not yet implemented");
      break;
    }
  }

  //************
  //* Function *
  //************

  //--------------------------------
  // Function::eval(Value**,size_t)
  //--------------------------------
  
  Value Function::eval(Value* args[8],size_t nargs){
    Value res;
    res.type=tr;
    switch(nargs){
    case 1:
      res.ptr=(*((Func1)ptr))(args[0]->eval()->ptr);
      break;
    default:
      Bug("Not yet implemented");
    }
    return res;
  }

  //***************
  //* Interpreter *
  //***************

  //-------------------------------------------------------------------------
  // Interperter::add_operator(const string&,const string&,OperatorType,int)
  //-------------------------------------------------------------------------
  
  void
  Interpreter::add_operator(const string& op,const string& name,OperatorType type,int priority){
    OperatorInfo* opInfo=new OperatorInfo(name,type,priority);
    operator_tree.add(op,opInfo);
  }

  //------------------------------------------------------------------------
  // Interpreter::complete(const string&,const string&,size_t,int,Context&)
  //------------------------------------------------------------------------
  
  string
  Interpreter::complete(const string& cmd,const string& word,size_t pos,int state,Context& context){
    if(state==0){
      split_to_tokens(cmd);
      size_t i;
      for(i=0;i<nodes_number;++i){
	if(nodes[i].pos>pos) break;
      }
      if(i!=0) --i;
      if(i>0 and nodes[i-1].tokenType==tDot) --i;
      if(nodes[i].tokenType==tDot){
	//*.word
	if(i>0 and nodes[i-1].tokenType==tName){
	  Symbol* symbol=context.get_symbol(nodes[i-1].str);
	  if(symbol==nullptr) return "";
	  if(symbol->type==type_type){
	    completion.type=context.get_type(nodes[i-1].str);
	  }
	  else{
	    completion.type=symbol->type;
	  }
	  completion.prefix=completion.type->name+".";
	  completion.word=completion.prefix+word;
	  completion.it=context.symbols.lower_bound(completion.word);
	}
	else return "";
      }
      else{
	//word
	completion.prefix="";
	completion.word=word;
	completion.it=context.symbols.lower_bound(completion.word);
	completion.type=nullptr;
      }
    } 
    //Omit function fullname, hidden symbol and member symbols if no member call
    while((completion.it!=context.symbols.end()) and
	  ((completion.it->second.hide) or
	   (completion.it->first.find('(')!=string::npos) or
	   (completion.type==nullptr and completion.it->first.find('.')!=string::npos)
	   )
	  )
      ++completion.it;
    if(completion.it==context.symbols.end()) return "";
    
    if(completion.it->first.compare(0,completion.word.size(),completion.word)==0){
      string res=completion.it->first;
      ++completion.it;
      return res.substr(completion.prefix.size());
    }
    return "";
  }

  //-------------------------------------------------
  // Interpreter::construct_sequence(size_t&,size_t)
  //-------------------------------------------------
  
  size_t
  Interpreter::construct_sequence(size_t& pos,size_t last){
    Node& node=nodes[pos];
    BracketType bt=node.bracketType;
    node.expressionType=get_delimiter_expression_type(bt);
    int depth=1;
    size_t begin=pos;
    slong* next=&node.son;
    size_t i=++pos;
    for(;pos<=last and depth!=0;++pos){
      switch(nodes[pos].tokenType){
      case tOpenBracket:
	++depth;
	break;
      case tCloseBracket:
	--depth;
	break;
      case tComma:
	if(depth==1){
	  if(i>pos-1)//Empty
	    SyntaxError("An Expression is needed",nodes[i-1].pos,nodes[pos].pos);
	  (*next)=construct_tree(i,pos-1,max_precedence_level);
	  next=&(nodes[*next].bro);
	  i=pos+1;
	}     
	break;
      default:
	break;
      }
    }
    //Close bracket not found
    if(depth!=0){
      switch(bt){
      case bRound:
	SyntaxError("Tuple or list has not ending",nodes[begin].pos,nodes[last].pos);
      case bSquare:
	SyntaxError("Array has not ending",nodes[begin].pos,nodes[last].pos);
      case bCurly:
	SyntaxError("Set has not ending",nodes[begin].pos,nodes[last].pos);
      default:
	Bug("Bracket type unkown");
	break;
      }
    }
    size_t end=pos-1;
    //Close bracket mismatch
    if(nodes[end].bracketType!=bt)
      SyntaxError("Close bracket mismatch the opening one",nodes[begin].pos,nodes[end].pos);

    //The sequence is empty
    if(end==begin+1) return begin;
    //Add the last element
    if(i>=end)//Empty 
      SyntaxError("An expression is needed",nodes[i-1].pos,nodes[end].pos);
    (*next)=construct_tree(i,end-1,max_precedence_level);
    return begin;
  }

  //-------------------------------------------------
  // Interpreter::construct_tree(size_t&,size_t,int)
  //-------------------------------------------------
  
  size_t
  Interpreter::construct_tree(size_t& first,size_t last,int precedence_level){
    Node& node=nodes[first];
    size_t root=first;
    switch(node.tokenType){
    case tInteger:
    case tString:
      node.expressionType=expLeaf;
      ++first;
      break;
    case tName:
      ++first;
      if(first<=last and nodes[first].tokenType==tOpenBracket){
	if(nodes[first].bracketType==bRound){
	  //Function call
	  node.son=nodes[construct_sequence(first,last)].son;
	  node.expressionType=expFunction;
	  break;
	}
	else if(nodes[first].bracketType==bSquare){
	  //Get or set call
	  node.bro=nodes[construct_sequence(first,last)].son;
	  Node& nnode=nodes[first];
	  nnode.son=root;
	  nnode.str="get";
	  root=first;
	  nnode.expressionType=expArrayGet; 
	  node.expressionType=expLeaf;
	  break;
	}
	else{
	  Bug("Not yet implemented");
	}
      }
      else if(first<=last and (nodes[first].tokenType==tDot)){
	//Member function call
	++first;
	if(first>last or nodes[first].tokenType!=tName){
	  if(nodes[first-1].tokenType==tDot) SyntaxError("Name missing after dot",nodes[first-1].pos,nodes[first-1].pos);
	  SyntaxError("Name missing after ::",nodes[first-1].pos,nodes[first-1].pos+1);
	}
	string name=nodes[first].str;
	++first;
	if(first<=last and nodes[first].tokenType==tOpenBracket and nodes[first].bracketType==bRound){
	  Node& nnode=nodes[first-2];
	  nnode.son=root;
	  nnode.str=name;
	  nnode.expressionType=expMemberFunction;
	  root=first-2;
	  node.bro=nodes[construct_sequence(first,last)].son;
	  node.expressionType=expLeaf;
	  break;
	}
	else{
	  Node& nnode=nodes[first-1];
	  nnode.son=root;
	  nnode.expressionType=expMemberFunction;
	  nnode.str=name;
	  node.expressionType=expLeaf;
	  root=first-1;
	  break;
	}
      }
      else{
	node.expressionType=expLeaf;
      }
      break;
    case tOpenBracket:
      root=construct_sequence(first,last);
      //(Expr) -> Expr
      if(node.bracketType==bRound and node.son!=-1 and nodes[node.son].bro==-1){
	root=node.son;
      }
      break;
    case tOperator:
      if(node.operatorInfo->type!=opPreUnitary and node.str!="-")
	SyntaxError("Operator not allowed here",node.pos,node.pos+node.str.length()-1);
      node.expressionType=expFunction;
      if(node.str=="-"){
	node.str="negate";
	node.son=construct_tree(++first,last,1);
      }
      else{
	node.str=node.operatorInfo->func;
	node.son=construct_tree(++first,last,node.operatorInfo->precedence);
      }
      break;
    case tCloseBracket:
      SyntaxError("Missing openning bracket",node.pos,node.pos);
    case tComma:
      SyntaxError("Unexecepted comma outside a sequence",node.pos,node.pos);
    case tUnkown:
      SyntaxError("Get unkown token",node.pos,node.pos);
    case tEnd:
      //Ignore this token 
      break;
    default:
      Bug("not yet implemented");
      break;
    }
    size_t lhs;
    //We already read an expression
    while(first<=last){//There is something on the right
      //At this time inly operator are allowed
      lhs=root;
      root=first;
      Node& node=nodes[root];
      if(node.tokenType==tOperator){
	if(node.operatorInfo->type==opPreUnitary){
	  SyntaxError("A prefixed unitary operator is not allowed here",node.pos,node.pos+node.str.length()-1);
	}
	if(node.operatorInfo->precedence>precedence_level) return lhs;
	if(node.operatorInfo->type==opPostUnitary){
	  node.expressionType=expFunction;
	  node.son=lhs;
	  ++first;
	}
	else if(node.operatorInfo->type==opBinary){
	  if(first==last) SyntaxError("An expression is needed after binary operator",node.pos,node.pos+node.str.length()-1);
	  size_t rhs=construct_tree(++first,last,node.operatorInfo->precedence);
	  node.str=node.operatorInfo->func;
	  node.expressionType=expFunction;
	  node.son=lhs;
	  nodes[lhs].bro=rhs;
	}
      }
      else SyntaxError("Only operator are allowed after such expression",node.pos,node.pos+node.str.length()-1);
    }
    return root;
  }

  //-------------------------------------------------------
  // Interpreter::display_expression_tree(ostream&,size_t)
  //-------------------------------------------------------
  
  void
  Interpreter::display_expression_tree(ostream& os,size_t i) const{
    const Node& node=nodes[i];
    switch(node.expressionType){
    case expLeaf:
      os<<'[';
      display_token(os,i);
      os<<']';
      return;
    case expTuple:
      os<<"[tuple";
      break;
    case expArray:
      os<<"[array";
      break;
    case expSet:
      os<<"[set";
      break;
    case expFunction:
      os<<"[function '"<<node.str<<"'";
      break;
    case expMemberFunction:
      os<<"[member function '"<<node.str<<"'";
      break;
    default:
      Bug("ExpressionNode type unkown");
    }
    slong j=node.son;
    if(j>=0){//There is at least a son
      do{
	os<<',';
	display_expression_tree(os,j);
	j=nodes[j].bro;
      }while(j>=0);
    }
    os<<']';
  }

  //---------------------------------------------
  // Interpreter::display_token(ostream&,size_t)
  //---------------------------------------------

  void
  Interpreter::display_token(ostream& os,size_t i) const{
    const Node& node=nodes[i];
    switch(node.tokenType){
    case tInteger:
      os<<"(integer,"<<integer_disp(node.value.ptr)<<')';
      return;
    case tString:
      os<<"(string,"<<node.str<<')';
      return;
    case tName:
      os<<"(name,"<<node.str<<')';
      return;
    case tEnd:
      os<<"(end)";
      return;
    case tComma:
      os<<"(comma)";
      return;
    case tOpenBracket:
      os<<"(open "<<node.bracketType<<" bracket)";
      return;
    case tCloseBracket:
      os<<"(close "<<node.bracketType<<" bracket)";
      return;
    case tOperator:
      os<<"(operator, "<<*node.operatorInfo<<")";
      return;
    case tUnkown:
      os<<"(?,"<<node.str<<')';
      return;
    default:
      Bug("Token type unkown");
      return;
    }
  }

  //---------------------------------------
  // Interpreter::display_tokens(ostream&)
  //---------------------------------------
  
  void
  Interpreter::display_tokens(ostream& os) const{
    for(size_t i=0;i<nodes_number;++i){
      display_token(os,i);
      os<<endl;
    }
  }

  //---------------------------
  // Interpreter::eval(string)
  //---------------------------
  
  void Interpreter::eval(string cmd,Context& context){
    size_t root;
    Node* node;
    bool error=false;
    try{
      split_to_tokens(cmd);
      size_t first=0;
      root=construct_tree(first,nodes_number-1,max_precedence_level);
      eval_expression(root,context);
      node=&nodes[root];
      Value* value=node->value.eval();
      if(value->type!=nullptr and value->type!=type_void){
	cout<<value->disp()<<endl;
      }
    }
    catch(Error err){
      err.disp(cout,cmd);
      cout<<endl;
      error=true;
    }
    //Delete the return value
    if(not error){
      if(node->erase){
      node->value.del();
      node->erase=false;
      }
    }
  }

  //-----------------------------------------------
  // Interpreter::eval_expression(size_t,Context&)
  //-----------------------------------------------
  
  void Interpreter::eval_expression(size_t pos,Context& context){
    Node &node=nodes[pos];
    if(node.expressionType==expLeaf){
      if(node.tokenType==tName){
	node.value.type=type_symbol;
	Symbol* symbol=context.get_symbol(node.str);
	if(symbol!=nullptr){
	  node.value.ptr=symbol;
	}
	else{
	  node.value.ptr=context.add_symbol(node.str,type_void,nullptr,false);
	}
	return;
      }
      return;
    }
    size_t size=0;
    slong j=node.son;
    while(j!=-1){
      ++size;
      eval_expression(j,context);
      j=nodes[j].bro;
    }
    switch(node.expressionType){
    case expFunction:
      context.eval_function(node,nodes);
      break;
    case expMemberFunction:
      context.eval_member_function(node,nodes);
      break;
    case expArray:
      context.eval_array(size,node,nodes);
      break;
    case expSet:
      context.eval_set(size,node,nodes);
      break;
    case expTuple:
      context.eval_tuple(size,node,nodes);
      break;
    case expArrayGet:
      Bug("Exp array get not yet implemented");
      break;
    default:
      Bug("Not yet implemented");
    }
    j=node.son;
    while(j!=-1){
      if(nodes[j].erase){
	nodes[j].value.del();
	nodes[j].erase=false;
      }
      j=nodes[j].bro;
    }
  }

  //-------------------------------------------------
  // Interpreter::get_integer(size_t&,const string&)
  //-------------------------------------------------
  
  fmpz*
  Interpreter::get_integer(size_t& pos,const string& cmd){
    size_t oldPos=pos;
    size_t end=cmd.length();
    char l;
    do{
      l=cmd[++pos];
    }while(pos<end and '0'<=l and l<='9');
    string sint=cmd.substr(oldPos,pos-oldPos);
    fmpz* res=new fmpz;
    fmpz_init(res);
    fmpz_set_str(res,sint.c_str(),10);
    return res;
  }

  //----------------------------------------------
  // Interpreter::get_name(size_t&,const string&)
  //----------------------------------------------

  string
  Interpreter::get_name(size_t& pos,const string& cmd){
    size_t end=cmd.length();
    size_t npos=pos;
    char l=cmd[npos];
    do{
      ++npos;
      l=cmd[npos];
    }while(npos<end and (('a'<=l and l<='z') or 
			 ('A'<=l and l<='Z') or
			 ('0'<=l and l<='9') or
			 l=='_'));
    string str=cmd.substr(pos,npos-pos);
    pos=npos;
    return str;
  }

  //------------------------------------------------
  // Interpreter::get_string(size_t&,const string&)
  //------------------------------------------------
  
  string
  Interpreter::get_string(size_t& pos,const string& cmd){
    size_t npos=cmd.find('"',pos+1);
    if(npos==string::npos){
      SyntaxError("the string does not end by \"",pos,string::npos);
    }
    string str=cmd.substr(pos+1,npos-pos-1);
    pos=npos+1;
    return str;
  }

  //-----------------------------------------------------
  // Interpreter::set_token(Node&,size_t&,const string&)
  //-----------------------------------------------------
  
  void
  Interpreter::set_token(Node& node,size_t& pos,const string& cmd){
    node.value.type=nullptr;
    node.erase=false;
    size_t oldPos;
    node.pos=pos;
    node.son=-1;
    node.bro=-1;
    size_t end=cmd.length();
    if(pos>=end){
      node.tokenType=tEnd;
      return;
    }
    char l=cmd[pos];
    if('0'<=l and l<='9'){
      node.tokenType=tInteger;
      oldPos=pos;
      node.value.type=type_integer;
      node.value.ptr=(void*)(get_integer(pos,cmd));
      node.erase=true;
      node.str=cmd.substr(oldPos,pos-oldPos+1);
      return;
    }
    if(l=='"'){
      node.tokenType=tString;
      node.str=get_string(pos,cmd);
      node.value.type=type_string;
      string* pstr=new string;
      *pstr=node.str;
      node.value.ptr=(void*)pstr;
      node.erase=true;
      return;
    }
    if(('a'<=l and l<='z') or ('A'<=l and l<='Z') or l=='_'){
      node.tokenType=tName;
      node.str=get_name(pos,cmd);
      return;
    }
    switch(l){
    case ',':
      node.str=',';
      node.tokenType=tComma;
      ++pos;
      return;
    case '.':
      node.str='.';
      node.tokenType=tDot;
      ++pos;
      return;
    case '(':
      node.tokenType=tOpenBracket;
      node.bracketType=bRound;
      node.str=l;++pos;
      return;
    case ')':
      node.tokenType=tCloseBracket;
      node.bracketType=bRound;
      node.str=l;++pos;
      return;
    case '[':
      node.tokenType=tOpenBracket;
      node.bracketType=bSquare;
      node.str=l;++pos;
      return;
    case ']':
      node.tokenType=tCloseBracket;
      node.bracketType=bSquare;
      node.str=l;++pos;
      return;
    case '{':
      node.tokenType=tOpenBracket;
      node.bracketType=bCurly;
      node.str=l;++pos;
      return;
    case '}':
      node.tokenType=tCloseBracket;
      node.bracketType=bCurly;
      node.str=l;++pos;
      return;
    }
    //Try to find operator
    oldPos=pos;
    OperatorInfo* operatorInfo=get_operator(pos,cmd);
    if(operatorInfo!=nullptr){//Operator found
      node.tokenType=tOperator;
      node.operatorInfo=operatorInfo;
      node.str=cmd.substr(oldPos,pos-oldPos);
      return;
    }
    //Operator not found
    SyntaxError("Token unkown",oldPos,oldPos);
    return;
  }

  //---------------------------------------------
  // Interpreter::split_to_tokens(const string&)
  //---------------------------------------------
  
  void Interpreter::split_to_tokens(const string& cmd){
    nodes_number=0;
    size_t pos=0;
    while(true){
      if(nodes_number>=max_nodes_number) Bug("The command is too huge");
      Node& node=nodes[nodes_number++];
      set_token(node,pos,cmd);
      if(node.tokenType==tEnd) break;
    }
    //Remove node of token type tEnd
    --nodes_number;
  }
  
  //***********************
  //* Auxiliary functions *
  //***********************

  //--------------------------------------------
  // get_delimiter_expression_type(BracketType)
  //--------------------------------------------
  
  ExpressionType
  get_delimiter_expression_type(BracketType type){
    switch(type){
    case bRound:
      return expTuple;
    case bSquare:
      return expArray;
    case bCurly:
      return expSet;
    default:
      cout<<"BUG from getDelimiterExpressionType"<<endl;
      return expTuple;
    }
  }

  //---------------------------------
  // close_bracket_char(BracketType)
  //---------------------------------
  
  char
  close_bracket_char(BracketType type){
    switch(type){
    case bRound:
      return ')';
    case bSquare:
      return ']';
    case bCurly:
      return '}';
    default:
      cout<<"BUG from closeBracketChar"<<endl;
      return ')';
    }
  }

  //---------------------------------------
  // function_fullname(string,string_list)
  //---------------------------------------
  
  string
  function_fullname(string name,string_list args){
    string res=name+"(";
    if(args.size()==0) return res+")";
    auto it=args.begin();
    res+=*it;
    for(++it;it!=args.end();++it){
      res+=",";
      res+=*it;
    }
    return res+")";
  }

  //------------------------------------------
  // function_fullname(string,Vlaue**,size_t)
  //------------------------------------------
  
  string
  function_fullname(string name,Value** args,size_t nargs){
    string res=name+"(";
    if(nargs==0) return res+")";
    res+=args[0]->type->name;
    for(size_t i=1;i<nargs;++i){
      res+=",";
      res+=args[i]->type->name;
    }
    return res+")";
  }

  //--------------------------------------------
  // function_fullname(string,const Signature&)
  //--------------------------------------------
  
  string
  function_fullname(string name,const Signature& signature){
    string res=name+"(";
    if(signature.is_empty()) return res+")";
    res+=signature[0]->name;
    for(size_t i=1;i<signature.size();++i){
      res+=",";
      res+=signature[i]->name;
    }
    return res+")";
  }
    

  ostream&
  operator<<(ostream& os,const BracketType& bt){
    switch(bt){
    case bRound:
      return os<<"round";
      break;
    case bSquare:
      return os<<"square";
      break;
    case bCurly:
      return os<<"curly";
      break;
    default:
      return os<<"?";
      break;
    }
  }

  ostream&
  operator<<(ostream& os,const OperatorInfo& info){
    switch(info.type){
    case opPreUnitary:
      return os<<"_ "<<info.func;
    case opPostUnitary:
      return os<<info.func<<" _";
    case opBinary:
      return os<<"_ "<<info.func<<" _";
    default:
      return os<<"Operator type unkown";
    }
  }
}
