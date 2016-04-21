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

#ifndef KERNEL_HPP
#define KERNEL_HPP

#include "module.hpp"
#include <set>

namespace Gomu{

  //************
  //* Typedefs *
  //************
  
  typedef Array<Type*> Signature;

  //***********************
  //* Auxiliary functions *
  //***********************
  
  //-------
  // Array
  //-------
  
  string array_disp(void*);
  void array_del(void*);
  void* array_copy(void*);
  int array_comp(void*,void*);
  
  //---------
  // Boolean
  //---------
  
  string boolean_disp(void*);
  void boolean_del(void*);
  void* boolean_copy(void*);
  int boolean_comp(void*,void*);
  
  //---------
  // Context
  //---------

  string context_disp(void*);
  void context_del(void*);
  void* context_copy(void*);
  int context_comp(void*,void*);
  
  //--------------------
  // ContextualFunction
  //--------------------

  string contextual_function_disp(void*);
  void contextual_function_del(void*);
  void* contextual_function_copy(void*);
  int contextual_function_comp(void*,void*);
  
  //----------
  // Function
  //----------
  
  string function_disp(void*);
  void function_del(void*);
  void* function_copy(void*);
  int function_comp(void*,void*);
  
  //---------
  // Integer
  //---------
  
  string integer_disp(void*);
  void integer_del(void*);
  void* integer_copy(void*);
  int integer_comp(void*,void*);
  
  //--------------
  // MetaFunction
  //--------------
  
  string meta_function_disp(void*);
  void meta_function_del(void*);
  void* meta_function_copy(void*);
  int meta_function_comp(void*,void*);
  
  //--------
  // Module
  //--------
  
  string module_disp(void*);
  void module_del(void*);
  void* module_copy(void*);
  int module_comp(void*,void*);

  //-----
  // Set
  //-----
  
  string set_disp(void*);
  void set_del(void*);
  void* set_copy(void*);
  int set_comp(void*,void*);
  
  //--------
  // String
  //--------
  
  string string_disp(void*);
  void string_del(void*);
  void* string_copy(void*);
  int string_comp(void*,void*);
  
  //-------
  // Tuple
  //-------
  
  string tuple_disp(void*);
  void tuple_del(void*);
  void* tuple_copy(void*);
  int tuple_comp(void*,void*);
  
  //------
  // Type
  //------
  
  string type_disp(void*);
  void type_del(void*);
  void* type_copy(void*);
  int type_comp(void*,void*);
  
  //------
  // Void
  //------
  
  string void_disp(void*);
  void void_del(void*);
  void* void_copy(void*);
  int void_comp(void*,void*);
  
  //***********************
  //* Auxiliary functions *
  //***********************

  //! Init the kernel
  //! \param context evaluation context
  //! \param interpreter the interpreter used for evaluation
  void init_kernel(Context& context,Interpreter& interpreter);

  //! Display Signature signature
  string disp_signature(const Signature& signaure);

  //! Load a module
  Value load_module(Context& context,Value& v);

  //! Unload a module
  Value unload(Context& context,Value& v);

  //! Reload a module
  Value reload(Context& context,Value& v);
  
  //***********************
  //* Inline declarations *
  //***********************

  //---------
  // Boolean
  //----------
  inline void
  boolean_del(void* v){delete (char*)v;}
  
  inline void*
  boolean_copy(void* v){return new char(*(char*)v);}

  //---------
  // Context
  //---------
    
  inline string
  context_disp(void*){return "Context";}

  inline void
  context_del(void*){}

  inline void*
  context_copy(void*){Bug("Not yet implemented");}

  inline int
  context_comp(void*,void*){Bug("Not yet implemented");}

  //--------------------
  // ContextualFunction
  //--------------------
  
  inline void
  contextual_function_del(void*){Bug("Not yet implemented");}
  
  inline void*
  contextual_function_copy(void*){Bug("Not yet implemented");}
  
  inline int
  contextual_function_comp(void*,void*){Bug("Not yet implemented");}

  //----------
  // Function
  //----------
  
  inline void
  function_del(void*){Bug("Not yet implemented");}

  inline void*
  function_copy(void*){Bug("Not yet implemented");}

  inline int
  function_comp(void*,void*){Bug("Not yet implemented");}
  
  //---------
  // Integer
  //---------
 
  inline void
  integer_del(void* v){fmpz_clear((fmpz*)v);}

  inline int
  integer_comp(void* v1,void* v2){return fmpz_cmp((fmpz*)v1,(fmpz*)v2);}
  
  //--------------
  // MetaFunction
  //--------------

  inline void
  meta_function_del(void*){}

  inline void*
  meta_function_copy(void*){Bug("Not yet implemented");}
  
  inline int
  meta_function_comp(void*,void*){Bug("Not yet implemented");}

  //--------
  // Module
  //--------
  
  inline void 
  module_del(void* v){delete (Module*)(v);}

  inline void*
  module_copy(void*){Bug("Not yet implemented");}

  inline string
  module_disp(void*){return "module";}

  inline int
  module_comp(void*,void*){Bug("Not yet implemented");}

  //--------
  // String
  //--------

  inline string
  string_disp(void* v){
    return "\033[32m"+*(string*)v+"\033[0m";
  }

  inline void
  string_del(void* v){delete (string*)v;}

  inline void*
  string_copy(void* v){return new string(*(string*)v);}

  //------
  // Type
  //------

  
  inline string
  type_disp(void* T){
    return "\033[1;32m"+((Type*)T)->name+"\033[0m";
  }
  inline void
  type_del(void*){}

  inline void*
  type_copy(void*){Bug("Not yet implemented");}

  //------
  // Void
  //------

  inline string
  void_disp(void*){return "";}

  inline void
  void_del(void*){}

  inline int
  void_comp(void*,void*){return 0;}

  inline void*
  void_copy(void*){return nullptr;}

}
#endif
