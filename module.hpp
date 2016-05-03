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

#ifndef MODULES_HPP
#define MODULES_HPP

#include <iostream>
#include <cstdint>
#include <map>
#include <set>
#include "array.hpp"
#include "flint/fmpz.h"

using namespace std;

//*****************
//* Integer types *
//*****************

typedef uint64_t uint64;
typedef int64_t int64;

//**********
//* Macros *
//**********
  
#define FUNC(f) (void*)(f)
#define CONT_FUNC_SENTINEL {"",{""},nullptr}
#define FUNC_SENTINEL {"","",{""},nullptr}
#define SYMB_SENTINEL {"","",nullptr}
#define TYPE_SENTINEL {"",nullptr,nullptr,nullptr,nullptr}

#define SyntaxError(msg,first,last) throw Error(errSyntax,(msg),(first),(last),__FILE__,__LINE__,__PRETTY_FUNCTION__);
#define Bug(msg) throw Gomu::Error(Gomu::errBug,(msg),0,0,__FILE__,__LINE__,__PRETTY_FUNCTION__);
#define ContextError(msg) throw Error(errContext,(msg),0,0,__FILE__,__LINE__,__PRETTY_FUNCTION__);
#define RuntimeError(msg) throw Gomu::Error(Gomu::errRuntime,(msg),0,0,__FILE__,__LINE__,__PRETTY_FUNCTION__);
namespace Gomu{
  
  //*********************
  //* Early declaration *
  //*********************

  class ArrayValue;
  class Context;
  class Interpreter;
  class Node;
  class SetValue;
  class SetValueComp;
  class Type;
  class Value;

  //************
  //* Typedefs *
  //************

  typedef string (*DispFunc)(void*);
  typedef void (*DelFunc)(void*);
  typedef void* (*CopyFunc)(void*);
  typedef int (*CompFunc)(void*,void*);

  
  //*********************
  //* Enumeration types *
  //*********************

  //! Enumeration of error type
  typedef enum{
    errBug,
    errSyntax,
    errContext,
    errRuntime,
    errUnkown
  } ErrorType;

  //******************
  //* Global objects *
  //******************
  
  extern Type *type_array;
  extern Type *type_boolean;
  extern Type *type_context;
  extern Type *type_generic;
  extern Type *type_integer;
  extern Type *type_function;
  extern Type *type_contextual_function;
  extern Type *type_meta_function;
  extern Type *type_module;
  extern Type *type_set;
  extern Type *type_string;
  extern Type *type_symbol;
  extern Type *type_tuple;
  extern Type *type_type;
  extern Type *type_void;

  //**********************
  //* Class declarations *
  //**********************

  //------------
  // ArrayValue
  //------------

  //! Class for array of value of same type
  class  ArrayValue{
  public:
    //! size of the array
    size_t size;
    //! type of stored values
    Type* type;
    //! array of C++ pointer of value
    void** tab;
    //! Contrust an ArrayValue of a given size
    //! \param desired size
    ArrayValue(size_t s);
  };

  //! Class for interpreter error
  class Error{
  public:
    //! Type of the error
    ErrorType type;
    //! Error message
    string msg;
    //! Filename of the source file that have launched the error
    string file;
    //! Name of the function that have launched the error
    string function;
    //! First character of the corresponding token
    size_t first;
    //! Last  character of the corresponding token
    size_t last;
    //! Number of the line that have launched the error
    size_t line;

    //! The empty constructor
    Error();

    //! The full constructor
    Error(ErrorType type,string msg,size_t first,size_t last,string file,int line,string function);

    //! Display the error
    //! \param os output streamm for display
    //! \param cmd command where error was found
    void disp(ostream& os,const string& cmd) const;
  };
  
  //--------
  // Module
  //--------
  
  //! Class for module
  class Module{
  public:
    
    class Function;
    class Symbol;
    class Type;
    
    //! Name of the module
    string name;
    
    //! Number of defined types
    size_t ntype;

    //! Number of defined functions
    size_t nfunc;

    //! Number of defined member functions
    size_t nmfunc;

    //! Number of defined contextual functions
    size_t ncfunc;

    //! Nomber of defined symbols
    size_t nsymb;

    //! Handle to the dinamic library object associated to the module
    void* handle;

    //! Name of defined types
    string* types;

    //! Defined functions
    Function *functions;

    //! Defined member functions
    Function *member_functions;

    //! Defined contextual function
    Function* contextual_functions;

    //! Defined symbols
    Symbol* symbols;
    
    //! Destructor
    ~Module();

    //! Unique constructor
    Module();

    //! Init module from types
    //! \param types types defined in the library
    void init(Module::Type* types);
  };

  //------------------
  // Module::Function
  //------------------

  //! Class for the module function declarations
  class Module::Function{
  public:
    //! Returned type
    string tr;

    //! Name of the function
    string name;

    //! Arguments type of the fnction 
    initializer_list<string> targs;

    //! Pointer to the C++ function
    void* ptr;

    //! Specify if the function is loaded or not
    bool loaded;
  };

  //----------------
  // Module::Symbol 
  //----------------

  //! Class for the module symbol declarations
  class Module::Symbol{
  public:
    //! Name of the symbol
    string name;

    //! Typre of the symbol
    string type;

    //! Pointer to the C++ data
    void* ptr;

    //! Specify if the symbol is loaded or not
    bool loaded;
  };

  //--------------
  // Module::Type
  //--------------

  //! Class for the module type declarations
  class Module::Type{
  public:
    //! Name of the type
    string name;

    //! Display function of the type
    DispFunc disp;

    //! Delete function of the type
    DelFunc del;

    //! Copy function of the type
    CopyFunc copy;

    //! Compeare function of the type
    CompFunc comp;

    //! Poniter to the type
    Gomu::Type** ptr;
  };
  
  //--------------
  // SetValueComp
  //--------------
  
  //! Class for comparison of SetValue objects
  class SetValueComp{
  public:

    //! Type of values in SetValue
    Type* type;

    //! The unsique constructor
    SetValueComp(Type* type);

    //! Function called for the comparison
    //! \param lhs pointer to a SetValue of type type
    //! \param rhs pointer to a SetValue of type type
    //! \return true if lhs<rhs and false otherwise
    bool operator()(void* lhs,void* rhs) const;
  };

  //----------
  // SetValue
  //----------
  
  //! Class for set of value of same type
  class SetValue{
  public:

    //! Set of value
    set<void*,SetValueComp> data;

    //! Construct a set of value of specified type
    //! \param type desired type
    SetValue(Type* type);
  };

  //------------
  // TupleValue
  //------------
  
  //! Class for tuple of Values
  class TupleValue{
  public:

    //! Tuple size
    size_t size;

    //! An array of Value
    Value* tab;

    //! Construt a TupleValue of size s
    //! \param s the desired size
    TupleValue(size_t s);
  };

  //------
  // Type
  //------
  
  //! Class for type
  class Type{
  public:
    //! Name of the type
    string name;

    //! Display function of the type
    DispFunc disp;

    //! Delete function of the type
    DelFunc del;

    //! Copy function of the type
    CopyFunc copy;

    //! Compeare function of the type
    CompFunc comp;

    //! Empty constructor
    Type();

    //! Full constructor
    Type(string,DispFunc disp,DelFunc del,CopyFunc copy,CompFunc comp);

    //! Recopy constructor
    Type(const Type&);

    //! Construct a type from a Module::Type
    Type(const Module::Type&);
  };

  //-------
  // Value
  //-------

  //! Class for value of the intepreter
  class Value{
  public:

    //! Type of the value
    Type* type;

    //! Pointer to theCc++ value
    void* ptr;

    //! Empty constructor
    Value();

    //! Constructor
    //! \param type type of the value
    //! \param ptr pointer to C++ value
    Value(Type* type,void* ptr);
    
    //! Delete the current value with protextions
    void pdel();
    
    //! Display the current value
    //! \return a string of the diplay
    string disp();

    //! Evaluate the current value (use for Symbolic value)
    //! \return pointed value if the current one is symbolic or the the current ont otherwise
    Value* eval();

    //! Return a copy of the current value
    Value copy();
  };
  
  //***********************
  //* Auxiliary functions *
  //***********************

  //! Return an slong from a value ptr
  int64 get_slong(void* v);
  
  //! Return a value ptr for a bool 
  void* to_boolean(bool n);

  //! Return a value ptr from a slong integer
  void* to_integer(slong s);

  //! Undefined copy function for type
  void* no_copy(void*);

  //! Undefined compare function for type
  int no_comp(void*,void*);

  //**********************
  //* Inline definitions *
  //**********************

  //-------
  // Error
  //-------
  
  inline
  Error::Error(){}
  
  inline
  Error::Error(ErrorType _type,string _msg,size_t _first,size_t _last,string _file,int _line,string _function):type(_type),msg(_msg),first(_first),last(_last),file(_file),line(_line),function(_function){}
  //------
  // Type
  //------
  
  inline
  Type::Type(){}
  
  //--------
  // Module
  //--------
 
  inline
  Module::~Module(){}

  //--------------
  // SetValueComp
  //--------------

  inline
  SetValueComp::SetValueComp(Type* t){
    type=t;
  }

  inline bool
  SetValueComp::operator()(void* lhs,void* rhs) const{return type->comp(lhs,rhs)==-1;}

  //----------
  // SetValue
  //----------

  inline
  SetValue::SetValue(Type* typeinfo):data(SetValueComp(typeinfo)){}
  
  //-------
  // Value
  //-------
  
  inline
  Value::Value():type(nullptr),ptr(nullptr){}

  inline
  Value::Value(Type* t,void* p):type(t),ptr(p){}

  inline void
  Value::pdel(){
    if(ptr!=nullptr and type!=nullptr and type!=type_symbol){
      // cout<<"Delete value of type "<<type->name<<endl;
      type->del(ptr);
    }
    ptr=nullptr;
  }

  inline Value*
  Value::eval(){
    if(type!=type_symbol) return this;
    return (Value*)ptr;
  }

  //---------------------
  // Auxiliary Functions
  //---------------------

}


#endif
