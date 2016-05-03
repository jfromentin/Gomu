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

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <iostream>
#include <deque>
#include <map>
#include <list>
#include <vector>
#include <dlfcn.h>
#include <initializer_list>
#include <cstdint>
#include "dictionnary.hpp"
#include "kernel.hpp"

using namespace std;

namespace Gomu{

  //*************
  //* Constants *
  //*************

  static const int assignement_precedence_level=98;
  static const int max_precedence_level=99;
  static const size_t max_nodes_number=1024;
  static const size_t max_arguments_number=8;

  //**********************
  //* Early declarations *
  //**********************
  
  class Completion;
  class Context;
  class ContextualFunction;
  class Function;
  class Node;
  class Interpreter;
  class OperatorInfo; 
  class Symbol;
  
  //************
  //* Typedefs *
  //************
  
  typedef Value (*CFunc0)(Context&);
  typedef Value (*CFunc1)(Context&,Value&);
  typedef Value (*CFunc2)(Context&,Value&,Value&);
  typedef Value (*CFunc3)(Context&,Value&,Value&,Value&);
  typedef Value (*CFunc4)(Context&,Value&,Value&,Value&,Value&);
  typedef Value (*CFunc5)(Context&,Value&,Value&,Value&,Value&,Value&);
  typedef Value (*CFunc6)(Context&,Value&,Value&,Value&,Value&,Value&,Value&);
  typedef Value (*CFunc7)(Context&,Value&,Value&,Value&,Value&,Value&,Value&,Value&);
  typedef Value (*CFunc8)(Context&,Value&,Value&,Value&,Value&,Value&,Value&,Value&,Value&);
  
  typedef void* (*Func0)();
  typedef void* (*Func1)(void*);
  typedef void* (*Func2)(void*,void*);
  typedef void* (*Func3)(void*,void*,void*);
  typedef void* (*Func4)(void*,void*,void*,void*);
  typedef void* (*Func5)(void*,void*,void*,void*,void*);
  typedef void* (*Func6)(void*,void*,void*,void*,void*,void*);
  typedef void* (*Func7)(void*,void*,void*,void*,void*,void*,void*);
  typedef void* (*Func8)(void*,void*,void*,void*,void*,void*,void*,void*);
  
  
  typedef const initializer_list<string>& string_list;
  

  //*********************
  //* Enumeration types *
  //*********************
  
  //! Enumeration of bracket types
  typedef enum{
    bCurly,
    bRound,
    bSquare
  } BracketType;

  //! Enumeration of expression types
  typedef enum{
    expArray,
    expArrayGet,
    expArraySet,
    expFunction,
    expMemberFunction,
    expMemberSymbol,
    expLeaf,
    expTuple,
    expSet
  } ExpressionType;

  //! Enumeration of token types
  typedef enum{
    tCloseBracket,
    tComma,
    tDot,
    tEnd,
    tInteger,
    tName,
    tOpenBracket,
    tOperator,
    tString,
    tUnkown
  } TokenType;

  //! Enumeration of operator types
  typedef enum{
    opBinary,
    opPreUnitary,
    opPostUnitary
  } OperatorType;

  
  //**********************
  //* Class declarations *
  //**********************

  //------------
  // Completion 
  //------------
  
  //! A class for completion information
  class Completion{
    
  public:
    
    //! Prefix to add to the word for looking symbol, used in case of a member call
    //! By example if s is ot type string then a.<tab> has prefix 'String.'
    string prefix;
    //! The word to complete
    string word;
    //! Type of the class for a member call, nullptr otherwise
    Type* type;
    //! An iterator for the context's symbols
    map<string,Symbol>::iterator it;
  };

  //---------
  // Context
  //---------
  
  //! A class for the interpretation context
  class Context{
  public:
    //! Pointer to an interpreter
    Interpreter* interpreter;
    
    //! Map for association name <-> symbol
    map<string,Symbol> symbols;
    
    //! The unique constructor
    Context(Interpreter* interpreter);

    //! Add a symbol for a contextual function
    //! \param name name of the symbol
    //! \param args list of the argument type strings
    //! \param ptr the contextual function pointer
    void add_contextual_function(string ret,string name,string_list args,void* ptr);
    
   //! Add a symbol for a function 
    //! \param ret return type string
    //! \param name name of the symbol
    //! \param args list of the argument type strings
    //! \param ptr the standard function pointer
    void add_function(string ret,string name,string_list args,void* ptr);
    
    //! Add a member function to the class given by the first argument
    //! \param ret return type string
    //! \param name name of the symbol
    //! \param args list of the argument type strings
    //! \param ptr the standard function pointer
    void add_member_function(string ret,string name,string_list args,void* ptr);

    //! Add a symbol named name
    //! \param name name of the symbol
    //! \param type type of the symbol
    //! \param ptr pointer to the data
    //! \param lock specify if the symbol is locked or not
    //! \return Return the new added symbol
    Symbol* add_symbol(string name,Type* type,void* ptr,bool lock=true);
    
    //! Test if we can add a symbol named name
    //! \param name the symbol name
    //! \return true if we can add the symbol false otherwise
    bool can_add(string name);
    
    //! Test if we can add a contextual function called name
    //! \param name function name
    //! \param targs type of function arguments
    //! \return true if we can add the symbol false otherwise
    bool can_add_contextual_function(string name,string_list targs);
    
    //! Test if we can add a function called name
    //! \param name function name
    //! \param targs type of function arguments
    //! \return true if we can add the symbol false otherwise
    bool can_add_function(string name,string_list targs);

    //! Test if we can add a member function called name
    //! \param name function name
    //! \param targs type of function arguments
    //! \return true if we can add the symbol false otherwise
    bool can_add_member_function(string name,string_list targs);

    //! Evaluate an array node
    //! \param size size of the array
    //! \param current the node of the array
    //! \param nodes array of all nodes
    void eval_array(size_t size,Node& current,Node* nodes);

    //! Evaluate a contextual function
    //! \param contextual pointer to a contextual function
    //! \param args arguments of the function call
    //! \param nargs number of argument for the function call
    //! \return value returned by the contextual function
    Value eval_contextual_function(ContextualFunction* contextual,Value** args,size_t nargs);

     //! Evaluate a function
    //! \param function pointer to a function
    //! \param args arguments of the function call
    //! \param nargs number of argument for the function call
    //! \return value returned by the function
    Value eval_function(Function* function,Value** args,size_t nargs);

    //! Evaluate a function node (contextual or not)
    //! \param current the node of the function to evaluate
    //! \param nodes array of all nodes
    //! \param size size of the array
    void eval_function(Node& current,Node* nodes);
    
    //! Evaluate a function given by a symbol (contextual or not)
    //! \param symbol symbol for the function
    //! \param args arguments of the function call
    //! \param nargs number of argument for the function call
    //! \return value returned by the function
    Value eval_function(Symbol* symbol,Value** args,size_t nargs);
    
    //! Evaluate a member function node (contextual or not)
    //! \param current the node of the function to evaluate
    //! \param nodes array of all nodes
     void eval_member_function(Node& current,Node* nodes);

    //! Evaluate a member symbol
    //! \param current the node of the symbol to evaluate
    //! \param nodes array of all nodes
    void eval_member_symbol(Node& current,Node* nodes);
    
    //! Evaluate a set node
    //! \param size size of the set
    //! \param current the node of the set
    //! \param nodes array of all nodes
    void eval_set(size_t size,Node& current,Node* nodes);
 
    //! Evaluate a tuple node
    //! \param size size of the tuple
    //! \param current the node of the tuple
    //! \param nodes array of all nodes
    void eval_tuple(size_t size,Node& current,Node* nodes);

    //! Return the signature of a string list of arguments
    //! \param args the string list
    //! \return The corresponding signature
    Signature get_signature(string_list args);

    //! Return the symbol named name
    //! \param name name to find
    //! \return Symbol corresponding to name if it exists, nullptr otherwise
    Symbol* get_symbol(string name);
    
    //! Return the symbol named name in class type
    //! \param ctype type to lookup
    //! \param name name to find
    //! \return Symbol corresponding to name if it exists, nullptr otherwise
    Symbol* get_symbol(Type* ctype,string name);
    
    //! Return the type named name
    //! \param name name to find
    //! \return Type corresponding to name if it exists, nullptr otherwise
    Type* get_type(string name);

    //! Load the module name
    //! \param name of the module to load
    void load_module(string name);

    //! Load functions of a module
    //! \param the module to load
    //! \param src specify the src to load 0:functions 1:member_functions 2:contextual_functions 
    void load_module_functions(Module* module,int src);

    //! Load symbols of a module
    //! \param the module to load
    void load_module_symbols(Module* module);
    
    //! Unload a function
    //! \param name the name of the function to delete
    //! \param args the string list of the function arguments
    void unload_function(string name,string_list args);

    //! Unload a module
    //! \param module the module to unload
    void unload_module(Module* module);

    //! Unload module functions
    //! \param module the module containint functions to unload
    void unload_module_functions(Module* module);

    //! Unload a symbol
    //! \param name of the symbol to delete
    void unload_symbol(string name);
    
    //! Unload a type
    //! \param type the type tu unload
    void unload_type(Type* type);

    //! Reload a module
    //! \param module the module to reload
    void reload_module(Module* module);

    //! Set arguments of a function node
    //! \param current node of the function to consider
    //! \param nodes an array of all nodes
    //! \args an array of Value* to store the arguments of the function
    //! \return number of arguments of the function call
    size_t set_arguments(Node& current,Node* nodes,Value** args);
  };

  //--------------------
  // ContextualFunction
  //--------------------

  //! A class for contextual function
  class ContextualFunction{
      
  public:
      
    //! Return type of the contextual function
    Type* tr;
    //! Pointer to the contextual function
    void* ptr;
    //! Signature of the contextual function
    Signature signature;

    //! The unique constructor
    ContextualFunction(Type* tr,const Signature& signature,void* ptr);
    
    //! Eval the contextual function
    //! \param args function call arguments
    //! \papam number of arguments of the function call
    //! \param context the context of the contextual function evaluation
    //! \return The returned value
    Value eval(Value* args[8],size_t nargs,Context& context);
  };

  //----------
  // Function
  //----------

  //! A class for function
  
  class Function{
    
  public:
    
    //! Return type of the function
    Type* tr;
    //! Signature of the function
    Signature signature;
    //! Pointer to the function
    void* ptr;

    //! The unique constructor
    Function(Type* tr,const Signature& signature,void* ptr);

    //! Evaluate the function
    //! \param args function call arguments
    //! \papam number of arguments of the function call
    //! \return The returned value
    Value eval(Value* args[8],size_t nargs);
  };

  //------
  // Node
  //------

  //! Class for expression node
  class Node{
    
  public:
    
    //! Token type of the node
    TokenType tokenType;
    //! Expression type of the node
    ExpressionType expressionType;
    //! Position in the command of the fisrt letter of the substring representing the node
    size_t pos;
    //! Substring of the command representing the node
    string str;
    union{
      //! Bracket type of the node (if any)
      BracketType bracketType;
      //! Operator information of the node (if any)
      OperatorInfo* operatorInfo;
    };
    //! Value of the node after evaluation
    Value value;
    //! Index of the node son
    slong son;
    //! Index of the node brother
    slong bro;
    //! Specify if the node can be erased or not
    //bool erase;
  };
  
  //-------------
  // Interpreter
  //-------------

  //! A class for the interpreter
  class Interpreter{
    
  protected:
    
    //! Information about completion
    Completion completion;
    //! Number of nodes in the expression
    size_t nodes_number;
    //! An array of nodes describing the expression obtained from the command string
    Node nodes[max_nodes_number];
    //! The dictionnary of all defined operator
    Dictionnary<OperatorInfo> operator_tree;
    
  public:
    
    //! The unique constructor
    Interpreter();
    
    //! Add an operator to the operator's dictionnary
    //! \param op operator identification (=,!=,...)
    //! \param name name of the operator function to call
    //! \param type operator type (binary,preunitary,postunitary)
    //! \pram p operator precedence
    void add_operator(const string& op,const string& name,OperatorType type,int p);

    //! Function called during completion. It return a non empty string for each
    //! possible coompleted word and an empty one if there is non more possibiliy.
    //! \param cmd command containing the word to complete
    //! \param word the word to complete
    //! \param pos position of the cursor when completion has been called
    //! \param state state of the completion process 0 for the first call and >0 for the others
    //! \param context context of the future command evaluation
    //! \return a possible completion or ""
    string complete(const string& cmd,const string& word,size_t pos,int state,Context& context);

    //! Construct a sequence from an expression
    //! \param first position of the first node of the sequence
    //! \param last position of the last node of the sequence
    //! \return size of the sequence
    size_t construct_sequence(size_t& first,size_t last);

    //! Construct the expression tree of the command from an array of tokens
    //! \param first position of the first token of the experssion
    //! \param last position of the last token of the expession
    //! \param precedence_level current precedence level
    //! \return position of the root of the constructed tree
    size_t construct_tree(size_t& first,size_t last,int precedence_level);

    //! Display an expression tree
    //! \param os the output stream for display
    //! \param i position of the tree root
    void display_expression_tree(ostream& os,size_t i) const;

    //! Display a token
    //! \param os the output stream for display
    //! \parap i index of the token to diplay
    void display_token(ostream& os,size_t i) const;

    //! Display the token array
    //! \param os the output stream for display
    void display_tokens(ostream& os) const;
    
    //! Evaluate a command
    //! \param cmd command to evaluate
    //! \param context context of the evaluation
    //! \param display specify if we display the last value
    void eval(string cmd,Context& context);

    //! Evaluate a command in very basic way
    //! \param cmd the check command
    //! \param context context of the evaluation
    Value* eval_basic(string cmd,Context& context);

    //! Evaluate an expression
    //! \param pos indice of the expression to evaluate
    //! \param context context of the evaluation
    void eval_expression(size_t pos,Context& context);

    //! Get an integer from a substring of a command
    //! \param pos indice of the substring of the command representing the integer
    //! \param cmd command
    //! \return pointer to the corresponding integer
    fmpz* get_integer(size_t& pos,const string& cmd);

    //! Get a name from a substring of a command
    //! \param pos indice of the substring of the command representing the name
    //! \param cmd command
    //! \return name
    string get_name(size_t& pos,const string& cmd);

    //! Try to get an operator from a substring of a command
    //! \param pos indice of the substring of the command representing the operator
    //! \param cmd command
    //! \return A pointer to the new created operator information, nullptr otherwise 
    OperatorInfo* get_operator(size_t& pos,const string& cmd);

    //! Get a string from a substring of a command
    //! \param pos indice of the substring of the command representing the string
    //! \param cmd command
    //! \return string
    string get_string(size_t& pos,const string& cmd);

    //! Purge the expression tree
    void purge_tree();
    
    //! Set node to be the token of command at position pos
    //! \param node destination node of the token
    //! \param pos position of the substring representing the token in comman
    //! \param command command to evaluate
    void set_token(Node& node,size_t& pos,const string& cmd);

    //! Create an array of tokens from a command
    //! \param command command to evaluate
    void split_to_tokens(const string& cmd);
  };

  //--------------
  // MetaFunction
  //--------------

  //! Class for meta function (used in case of overloaded functions)
  class MetaFunction{
  public:
    //! Fullname of overloaded functions
    set<string> functions;
  };
   
  //--------------
  // OperatorInfo
  //--------------

  //! Class for operator informtation
  class OperatorInfo{
    
  public:

    //! Name of the operator function to call
    string func;
    //! Type of the operator (unary,...)
    OperatorType type;
    //! Precedence of the operator
    int precedence;
    //! The unique constructor
    OperatorInfo(string func,OperatorType t,int p);
  };
  
  //--------
  // Symbol
  //--------
  
  //! Class for context symbol
  class Symbol:public Value{
  public:
    //! Specify if the symbol is hidden in completion
    bool hide;
    //! Specify if the symbol is locked (roughlt created by C++ call)
    bool locked;
    //! Name of the symbol
    string name;
    //! The unique constructor
    Symbol();
  };

  //***********************
  //* Auxiliary functions *
  //***********************

  //! Copy the value src to the value dst
  //! \param dst pointer to the destination value
  //! \param src pointer to the source value
  void copy_value(Value* dst,Value* src);

  //! Get the fullname of a function
  //! \param name short name of the function
  //! \param args string array of argument type
  //! \param member speicy if the fucntion is a member function or not
  //! \return fullname of the function
  string function_fullname(string name,string_list args);

  //! Get the fullname of a function call
  //! \param name short name of the function
  //! \param args arguments of function call
  //! \param nargs number of arguments of the function call
  //! \return fullname of the function
  string function_fullname(string name,Value** args,size_t nargs);

  //! Get the fullname of a function
  //! \param name short name of the function
  //! \param signature signature of the function
  //! \return fullname of the function
  string function_fullname(string name,const Signature& signature);

  //! Get the expression type of hthe sequence corresponding to the bracket type t
  //! \param t a bracket type
  //! \return Expression type cooresponding to t
  ExpressionType get_delimiter_expression_type(BracketType t);

  //! Set a signature for a list of argument type string
  //! \param signature signature to fullfill
  //! \param args list of argument type string
  void set_signature(Signature& signature,string_list args);
  
  ostream& operator<<(ostream& os,const BracketType& bt);
  ostream& operator<<(ostream& os,const OperatorInfo& oi);
  
  //**********************
  //* Inline definitions *
  //**********************
 
  //---------
  // Context
  //---------
  
  inline void
  Context::add_contextual_function(string ret,string name,string_list args,void* ptr){
    Signature signature=get_signature(args);
    Type* tr=get_type(ret);
    add_symbol(name,type_contextual_function,(void*)(new ContextualFunction(tr,signature,ptr))); 
  }

  inline  bool
  Context::can_add(string name){
    Symbol* symbol=get_symbol(name);
    return (symbol==nullptr or not symbol->locked); 
  }
  
  inline bool
  Context::can_add_contextual_function(string name,string_list targs){
    return can_add(name);
  }

  inline Symbol*
  Context::get_symbol(Type* ctype,string name){
    return get_symbol(ctype->name+"."+name);
  }

  inline void
  Context::unload_symbol(string name){
    symbols.erase(name);
  }
  
  //--------------------
  // ContextualFunction
  //--------------------
  
  inline
  ContextualFunction::ContextualFunction(Type* t,const Signature& s,void* p):tr(t),signature(s),ptr(p){
  }

  //----------
  // Function
  //----------
  
  inline
  Function::Function(Type* t,const Signature& s,void* p):tr(t),signature(s),ptr(p){
  }

   //-------------
  // Interpreter
  //-------------
  
  inline
  Interpreter::Interpreter(){}

  inline OperatorInfo*
  Interpreter::get_operator(size_t& pos,const string& cmd){return operator_tree.find_at(pos,cmd);}

  //--------
  // Symbol
  //--------
  
  inline
  Symbol::Symbol():Value(),locked(false){
  }

  //--------------
  // OperatorInfo
  //--------------
  
  inline
  OperatorInfo::OperatorInfo(string str,OperatorType t,int p):func(str),type(t),precedence(p){}

  //---------------------
  // Auxiliray functions
  //---------------------
  
  inline void copyValue(Value* dst,Value* src){
    dst->type=src->type;
    dst->ptr=src->type->copy(src->ptr);
  }
}

#endif
